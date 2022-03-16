// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankPawn.h"
#include <FramePro/FramePro.h>
#include <UObject/NoExportTypes.h>
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class TANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

protected:

	UPROPERTY()
		class ATankPawn* TankPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float TargetingRange = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float Accurency = 20;

	int32 CurrentPatrolPointIndex = 0;
public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	APawn* PlayerPawn;

	void MoveToNextPoint();

	void CannonTargeting();

	void Targeting();

};
