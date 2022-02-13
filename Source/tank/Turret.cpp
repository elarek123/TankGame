// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include <Components/BoxComponent.h>
#include <Engine/StaticMesh.h>
#include <GameFramework/Actor.h>
#include <Kismet/KismetMathLibrary.h>
#include <UObject/NoExportTypes.h>
#include <Components/SceneComponent.h>
#include <Math/UnrealMathUtility.h>
#include "GameStructs.h"
#include "tank.h"
#include "HealthComponent.h"
#include <DrawDebugHelpers.h>

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.005;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret turret"));
	TurretMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

	CannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSpawnPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters Params;
	Params.Owner = this;
	CannonPack.Add(GetWorld()->SpawnActor<ACannon>(DefaultCannonClass, Params));
	CannonPack[0]->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	/*FTimerHandle _TargetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(_TargetingTimerHandle, this, &ATurret::Targeting, TargetingRate, true, TargetingRate);*/
	
}



void ATurret::Targeting()
{
	if (FVector::DistSquared(PlayerPawn->GetActorLocation(), GetActorLocation()) > FMath::Square(TargetingRange))
	{
		return;
	}
	FHitResult HitResult;
	FVector TraceStart = TurretMesh->GetComponentLocation();
	FVector TraceEnd = PlayerPawn->GetActorLocation();
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	TraceParams.bReturnPhysicalMaterial = false;

	if (!GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Pawn, TraceParams) || HitResult.Actor != PlayerPawn) {
		DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Purple, false, 0.1f, 0, 5);
		return;
	}
	DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Red, false, 0.1f, 0, 5);

	RotateToPlayer();
	FVector TargetingDir = TurretMesh->GetForwardVector();
	FVector DirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	DirToPlayer.Normalize();
	float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDir, DirToPlayer)));
	if (AimAngle <= Accurency) {
		Fire();
	}
}

void ATurret::RotateToPlayer()
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator CurrRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Pitch = CurrRotation.Pitch;
	TargetRotation.Roll = CurrRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(CurrRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), TargetingSpeed));
}


void ATurret::Fire()
{
	if (CannonPack[0])
		CannonPack[0]->Fire();

}


void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerPawn)
		Targeting();
}





