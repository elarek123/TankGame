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
#include "PhysicsProjectile.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	//PawnLocation = TankPawn->GetActorLocation();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TankPawn = Cast<ATankPawn>(GetPawn());
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
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

void ATankAIController::CannonTargeting()
{
	if (PlayerPawn) {
		ACannon* Cannon = TankPawn->GetCannon();
		Cannon->GetActorRotation();
	}
}

void ATankAIController::Targeting()
{
	
	
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
		FVector TraceEnd = PlayerPawn->GetActorLocation();
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
			if (APhysicsProjectile* PhysicsProjectile = Cast<APhysicsProjectile>(TankPawn->GetCannon()->GetProjectile()->GetDefaultObject()))
			{
				float Gravity = PhysicsProjectile->GetGravity();
				float MoveSpeed = PhysicsProjectile->GetMoveSpeed();
				float MaxLenght = FMath::Square(MoveSpeed) / Gravity;

				FVector From = TankPawn->GetCannonSpawnPoint()->GetComponentLocation();

				FVector FromWithoutZ = From;
				FromWithoutZ.Z = 0;

				FVector To = PlayerPawn->GetActorLocation();

				FVector ToWithoutZ = To;
				ToWithoutZ.Z = 0;

				float X = FVector::Dist(FromWithoutZ, ToWithoutZ);
				float Y = To.Z - From.Z;

				if (MaxLenght < X)
				{
					MoveSpeed = FMath::Sqrt(Gravity * X) + 100;
					TankPawn->GetCannon()->SetMoveSpeed(MoveSpeed);
				}

				float Angle = FMath::RadiansToDegrees(FMath::Atan(((FMath::Square(MoveSpeed) + FMath::Sqrt(FMath::Square(FMath::Square(MoveSpeed)) -
					Gravity * (Gravity * FMath::Square(X) + 2 * FMath::Square(MoveSpeed) * Y))) / (Gravity * X))));

				TankPawn->GetCannon()->SetCannonRotation(Angle);

				FVector Vector = TankPawn->GetTurretForwardVector();
				Vector.Normalize();
				Vector *= 100;

				//DrawDebugLine(GetWorld(), From, From + Vector, FColor::Emerald, false, 0.1f, 0, 5);

				FVector Axis = FVector(0, 1, 0);

				Vector = Vector.RotateAngleAxis(Angle, Axis);

				//DrawDebugLine(GetWorld(), From, From + Vector, FColor::Red, false, 0.1f, 0, 5);

				//UE_LOG(LogTanks, Warning, TEXT("%f %f %f"), TankPawn->GetCannon()->GetActorRotation().Roll, TankPawn->GetCannon()->GetActorRotation().Pitch, TankPawn->GetCannon()->GetActorRotation().Yaw);

			}
			TankPawn->Fire();
		}
	}
	
}


