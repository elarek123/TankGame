// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include <Components/BoxComponent.h>
#include "Cannon.h"
#include "Damageable.h"
#include "BasicClass.h"
#include "Turret.generated.h"

UCLASS()
class TANK_API ATurret : public ABasicClass
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRange = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingSpeed = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float Accuracy = 10;


public:
	ATurret();

	class ABasicClass* CurTarget;

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
	void Targeting();
	bool CanFire(ABasicClass* InTarget);
	bool IsTargetInRange(ABasicClass* InTarget);
	void RotateTo(ABasicClass* InTarget);

	void Fire();

};
