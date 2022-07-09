// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Delegates/Delegate.h"
#include "GameFramework/PlayerController.h"
#include "TankController.generated.h"

/**
 * 
 */
UCLASS()
class TANK_API ATankController : public APlayerController
{
	GENERATED_BODY()

		
public:
	virtual void BeginPlay() override;
	// Called to bind functionality to input
	virtual void SetupInputComponent() override;
	virtual void Tick(float Deltaseconds) override;
	void OnLeftMouseButtonUp();

	FSimpleMulticastDelegate OnMouseButtonUp;

private:
	void MoveForward(float InAxisValue);
	void RotateRight(float InAxisValue); 
	void Fire();
	void SpecialFire();
	void WeaponSwap();
	
	UPROPERTY()
	class ATankPawn* TankPawn;
};
