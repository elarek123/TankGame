// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include <GameFramework/SpringArmComponent.h>
#include "tank.h"
#include <Kismet/KismetMathLibrary.h>
#include <Components/ArrowComponent.h>
#include "Cannon.h"
#include <Math/UnrealMathUtility.h>
#include <Containers/Array.h>
#include <Templates/SubclassOf.h>
#include "GameStructs.h"




bool limit = 0;
bool count = 1;

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank Body"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank Turret"));
	TurretMesh->SetupAttachment(BodyMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	CannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	CannonSpawnPoint->SetupAttachment(TurretMesh);
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	SetupCannon(DefaultCannonClass);
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentMoveForwardAxis = FMath::FInterpTo(CurrentMoveForwardAxis, TargetMoveForwardAxis, DeltaTime, MovementSmoothness);

	FVector MoveVector = GetActorForwardVector() * CurrentMoveForwardAxis;

	/*if (MoveVector.SizeSquared() != 1.f)
	{
		MoveVector.Normalize();
	}*/

	SetActorLocation(GetActorLocation() + MoveVector * DeltaTime * MoveSpeed, true);

	CurrentRotateRightAxis = FMath::FInterpTo(CurrentRotateRightAxis, TargetRotateRightAxis, DeltaTime, MovementSmoothness);


	float Rotation = GetActorRotation().Yaw + CurrentRotateRightAxis * RotationSpeed * DeltaTime;
	SetActorRotation(FRotator(0.f, Rotation, 0.f));

	//UE_LOG(LogTank, Verbose, TEXT("CurrentRotateAxis: %d"), CurrentRotateRightAxis);
	
	FRotator TargetRotation = UKismetMathLibrary :: FindLookAtRotation(GetActorLocation(), TurretTargetPosition);
	FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Roll = CurrentRotation.Roll;
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, TurretRotationSmoothness));
}

void ATankPawn::MoveForward(float InAxisValue)
{
	TargetMoveForwardAxis = InAxisValue;
}

void ATankPawn::RotateRight(float InAxisValue)
{
	TargetRotateRightAxis = InAxisValue;
}

void ATankPawn::SetTurretTargetPosition(FVector& TargetPosition)
{
	TurretTargetPosition = TargetPosition;
}

void ATankPawn::Fire()
{
	if (CannonPack[CurrentCannonIndex] && CannonPack[CurrentCannonIndex]->GetAmmoParam(count)) {
		//UE_LOG(LogTank, Verbose, TEXT("Current Ammo: %d"), CannonPack[CurrentCannonIndex]->GetAmmoParam(count))
		CannonPack[CurrentCannonIndex]->Fire();
		//UE_LOG(LogTank, Verbose, TEXT("After Fire Ammo: %d"), CannonPack[CurrentCannonIndex]->GetAmmoParam(count))
	}
}

void ATankPawn::SpecialFire()
{
	if (CannonPack[CurrentCannonIndex] && CannonPack[CurrentCannonIndex]->GetAmmoParam(count) && CannonPack[CurrentCannonIndex]->GetType() == ECannonType::FireProjectile)
		CannonPack[CurrentCannonIndex]->SpecialFire();
}

void ATankPawn::WeaponSwap()
{	
	CannonPack[CurrentCannonIndex]->SetVisibility(false);
	CurrentCannonIndex = (CurrentCannonIndex + 1) % CurrentCannonNumber;
	CannonPack[CurrentCannonIndex]->SetVisibility(true);
}

void ATankPawn::SetupCannon(TSubclassOf<class ACannon> InCannonClass)
{
	if (CannonPackNames.Contains(InCannonClass->GetName())) {
		int CannonIndex;
		CannonPackNames.Find(InCannonClass->GetName(), CannonIndex);
		CannonPack[CannonIndex]->ReplenishAmmo();
		return;
	}
	if (CurrentCannonNumber == CannonLimit) {

		//UE_LOG(LogTank, Verbose, TEXT("CannonNumber1 : %d"), CannonPack.Num());
		CannonPack[CurrentCannonIndex]->Destroy();
		//UE_LOG(LogTank, Verbose, TEXT("CannonNumber2 : %d"), CannonPack.Num());
		CannonPack.RemoveAt(CurrentCannonIndex);
		CannonPackNames.RemoveAt(CurrentCannonIndex);
		CurrentCannonNumber--;
	}
	if (InCannonClass) {
		FActorSpawnParameters Params;
		Params.Instigator = this;
		Params.Owner = this;
		if (CannonPack.Num() && CurrentCannonIndex < CannonPack.Num())
			CannonPack[CurrentCannonIndex]->SetVisibility(false);
		CannonPack.Insert(GetWorld()->SpawnActor<ACannon>(InCannonClass, Params), CurrentCannonIndex);
		CannonPackNames.Insert(InCannonClass->GetName(), CurrentCannonIndex);
		CurrentCannonNumber++;
		CannonPack[CurrentCannonIndex]->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		CannonPack[CurrentCannonIndex]->SetVisibility(true);
	}
	UE_LOG(LogTank, Verbose, TEXT("CannonNumber3 : %d"), CannonPack.Num());
}

