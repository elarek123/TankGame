// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include <Containers/Array.h>
#include <DrawDebugHelpers.h>
#include <Math/Vector.h>
#include <GameFramework/Pawn.h>
#include <Math/UnrealMathUtility.h>
#include <GameFramework/Actor.h>
#include "TankPawn.h"
#include <Engine/TargetPoint.h>
#include "tank.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	//PawnLocation = TankPawn->GetActorLocation();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TankPawn = Cast<ATankPawn>(GetPawn());

	if (TankPawn) {
		MoveToNextPoint();
		//PawnLocation = TankPawn->GetActorLocation();
		Targeting();
	}
}

void ATankAIController::MoveToNextPoint()
{
	const TArray<class ATargetPoint*>& PatrollingPoints = TankPawn->GetPatrollingPoints();
	if (PatrollingPoints.Num() == 0)
		return;

	TankPawn->MoveForward(1);
	FVector PawnLocation = TankPawn->GetActorLocation();
	FVector CurrentPoint = PatrollingPoints[CurrentPatrolPointIndex]->GetActorLocation();
	if (FVector::DistSquared(PawnLocation, CurrentPoint) <= FMath::Square(TankPawn->GetMovementAccuracy())) {
		CurrentPatrolPointIndex++;
		if (!PatrollingPoints.IsValidIndex(CurrentPatrolPointIndex))
			CurrentPatrolPointIndex = 0;
	}
	FVector MoveDirection = CurrentPoint - PawnLocation;
	MoveDirection.Normalize();
	FVector ForwardDirection = TankPawn->GetActorForwardVector();
	FVector RightDirection = TankPawn->GetActorRightVector();

	DrawDebugLine(GetWorld(), PawnLocation, CurrentPoint, FColor::Green, false, 0.1f, 0, 5);

	float ForwardAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardDirection, MoveDirection)));
	float RightAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(RightDirection, MoveDirection)));

	float RotationValue = 0;
	if (ForwardAngle > 5)
		RotationValue = 1;
	if (RightAngle > 90)
		RotationValue *= -1;

	//UE_LOG(LogTemp, Warning, TEXT("AI Rotation forwardAngle: %f rightAngle: %f rotationValue: %f"), forwardAngle, rightAngle, rotationValue);
	TankPawn->RotateRight(RotationValue);

}

void ATankAIController::Targeting()
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	
	if (PlayerPawn) {
		UE_LOG(LogTank, Verbose, TEXT("rofl"));
		if (FVector::DistSquared(PlayerPawn->GetActorLocation(), TankPawn->GetActorLocation()) > FMath::Square(TargetingRange))
		{
			UE_LOG(LogTank, Verbose, TEXT("rofl1"));
			return;
		}
		UE_LOG(LogTank, Verbose, TEXT("rofl2"));

		FHitResult HitResult;
		FVector TraceStart = TankPawn->GetActorLocation();
		FVector TraceEnd = PlayerPawn->GetActorLocation() ;
		FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
		TraceParams.bReturnPhysicalMaterial = false;

		if (!GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams) || HitResult.Actor != PlayerPawn) {
			UE_LOG(LogTank, Verbose, TEXT("rofl3"));
			DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Purple, false, 0.1f, 0, 5);
			return;
		}
		DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Red, false, 0.1f, 0, 5);


		TankPawn->SetTurretTargetPosition(PlayerPawn->GetActorLocation());

		FVector TargetingDir = TankPawn->GetTurretForwardVector();
		FVector DirToPlayer = PlayerPawn->GetActorLocation() - TankPawn->GetActorLocation();
		DirToPlayer.Normalize();
		float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDir, DirToPlayer)));
		if (AimAngle <= Accurency) {
			TankPawn->Fire();
		}
	}
	
}


