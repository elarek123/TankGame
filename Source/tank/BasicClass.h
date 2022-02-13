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
#include "AmmoBox.h"
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
	class UArrowComponent* AmmoBoxSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* HitCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* DestroyEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UAudioComponent* AudioDestroyEffect;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Turret")
	float TurretRotationSmoothness = 0.1;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Turret")
	TSubclassOf<class ACannon> DefaultCannonClass;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Turret")
	TArray <class ACannon*>  CannonPack;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Turret")
	float DestroyRate = 0.75;

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnHealthChanged(float Damage);

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDie();

	void Destroying();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo Box")
	TSubclassOf<class AAmmoBox> Ammo_Box;

	FTimerHandle ReloadTimerHandle;
public:

	FVector GetTurretLocation() {
		return TurretMesh->GetComponentLocation();
	}




};
