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
	SetupCannon();
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

	SetActorLocation(GetActorLocation() + MoveVector * DeltaTime * MoveSpeed);

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
	if (Cannon)
		Cannon->Fire();
}

void ATankPawn::SpecialFire()
{
	if (Cannon)
		Cannon->SpecialFire();
}

void ATankPawn::SetupCannon()
{
	if (Cannon)
		Cannon->Destroy();

	FActorSpawnParameters Params;
	Params.Instigator = this;
	Params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(DefaultCannonClass, Params);
	Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

