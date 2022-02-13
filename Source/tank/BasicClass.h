// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include "Cannon.h"
#include "Damageable.h"
#include "GameStructs.h"
#include <Components/BoxComponent.h>
#include "HealthComponent.h"
#include "BasicClass.generated.h"

UCLASS()
class TANK_API ABasicClass : public APawn, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasicClass();

	virtual void TakeDamage(const FDamageData& DamageData) override;

protected:

	UPROPERTY(VisibleDefaultsOnly, BluePrintReadWrite, Category = "Components")
		class UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleDefaultsOnly, BluePrintReadWrite, Category = "Components")
		class UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* CannonSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* HitCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Turret")
		float TurretRotationSmoothness = 0.1;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Turret")
		TSubclassOf<class ACannon> DefaultCannonClass;

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
		void OnHealthChanged(float Damage);

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
		void OnDie();





};
