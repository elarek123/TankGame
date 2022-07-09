// Fill out your copyright notice in the Description page of Project Settings.

#include "TankController.h"
#include "TankPawn.h"
#include <DrawDebugHelpers.h>



void ATankController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
	bShowMouseCursor = true;
	bEnableClickEvents = true;
}

void ATankController::SetupInputComponent() {
	Super::SetupInputComponent();
	if(InputComponent){
		InputComponent->BindAxis("MoveForward", this, &ATankController::MoveForward);
		InputComponent->BindAxis("RotateRight", this, &ATankController::RotateRight);
		InputComponent->BindAction("Fire", IE_Pressed, this, &ATankController::Fire);
		InputComponent->BindAction("SpecialFire", IE_Pressed, this, &ATankController::SpecialFire);
		InputComponent->BindAction("WeaponSwap", IE_Pressed, this, &ATankController::WeaponSwap);
		InputComponent->BindKey(EKeys::LeftMouseButton, IE_Released, this, &ThisClass::OnLeftMouseButtonUp);
	}
	

}

void ATankController::Tick(float Deltaseconds)
{
	Super::Tick(Deltaseconds);
	
	if (!TankPawn)
		return;
	FVector WorldMousePosition, WorldMouseDirection;
	DeprojectMousePositionToWorld(WorldMousePosition, WorldMouseDirection);

	FVector TargetTurretDirection = WorldMousePosition - TankPawn->GetActorLocation();
	TargetTurretDirection.Z = 0;
	TargetTurretDirection.Normalize();
	FVector TurretTargetPosition = TankPawn->GetActorLocation() + TargetTurretDirection * 100;
	DrawDebugLine(GetWorld(), TankPawn->GetActorLocation(), TurretTargetPosition, FColor::Green, false, 0.1, 0, 5);
	TankPawn->SetTurretTargetPosition(TurretTargetPosition);
}


void ATankController::OnLeftMouseButtonUp()
{
	OnMouseButtonUp.Broadcast();
}

void ATankController::MoveForward(float InAxisValue)
{
	if (TankPawn)
		TankPawn->MoveForward(InAxisValue);
}

void ATankController::RotateRight(float InAxisValue)
{
	if (TankPawn)
		TankPawn->RotateRight(InAxisValue);
}

void ATankController::Fire()
{
	if (TankPawn)
		TankPawn->Fire();
}

void ATankController::SpecialFire()
{
	if (TankPawn)
		TankPawn->SpecialFire();
}

void ATankController::WeaponSwap()
{
	if(TankPawn)
		TankPawn->WeaponSwap();
}
