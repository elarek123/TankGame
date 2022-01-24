// Fill out your copyright notice in the Description page of Project Settings.

#include "TankController.h"
#include "TankPawn.h"

void ATankController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankController::SetupInputComponent() {
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ATankController::MoveForward);
}

void ATankController::MoveForward(float InAxisValue)
{
	if (TankPawn)
		TankPawn->MoveForward(InAxisValue);
}
