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
#include "BasicClass.h"
#include "FrenPawn.h"

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


	/*FTimerHandle _TargetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(_TargetingTimerHandle, this, &ATurret::Targeting, TargetingRate, true, TargetingRate);*/
	
}



void ATurret::Targeting()
{
	for (auto& i : FrenPawn::GetUnFriendly(this))
	{
		if (IsTargetInRange(i.Value))
		{
			CurTarget = i.Value;
			break;
		}
	}

	if (CurTarget && IsTargetInRange(CurTarget))
	{
		RotateTo(CurTarget);
	}

	if (CurTarget && CanFire(CurTarget) && IsTargetInRange(CurTarget))
	{
		Fire();
	}
}

bool ATurret::CanFire(ABasicClass* InTarget)
{
	FVector TargetingDir = TurretMesh->GetForwardVector();
	FVector DirToPlayer = InTarget->GetActorLocation() - GetActorLocation();
	DirToPlayer.Normalize();
	float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDir, DirToPlayer)));

	return AimAngle <= Accuracy;
}

bool ATurret::IsTargetInRange(ABasicClass* InTarget)
{
	FHitResult HitResult;
	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = InTarget->GetActorLocation();
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("AI Vission Trace")), true, this);
	TraceParams.bReturnPhysicalMaterial = false;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
	{
		DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Red, false, 0.1f, 0, 5);
		if (HitResult.Actor != InTarget)
		{
			return false;
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 0.1f, 0, 5);
	}

	return FVector::DistSquared(InTarget->GetActorLocation(), GetActorLocation()) <= FMath::Square(TargetingRange);
}

void ATurret::RotateTo(ABasicClass* InTarget)
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InTarget->GetActorLocation());
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

	if (FrenPawn::GetUnFriendly(this).Num())
		Targeting();
}





