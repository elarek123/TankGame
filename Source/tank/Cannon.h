// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/ArrowComponent.h>
#include "GameStructs.h"
#include "Projectile.h"
#include <Components/AudioComponent.h>
#include <Camera/CameraShakeBase.h>
#include <GameFramework/ForceFeedbackEffect.h>
#include "Cannon.generated.h"

UCLASS()
class TANK_API ACannon : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* ShootEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UAudioComponent* AudioEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRate = 0.7;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRange = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireDamage = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	ECannonType Type = ECannonType::FireProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "Type == ECannonType::FireProjectile", EditConditionHides),  Category = "Fire params")
	TSubclassOf<class AProjectile> ProjectileClass; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	int SpecialFireLimit = 3;
	 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float SpecialFireRate = 0.5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	int AmmoLimit = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	int AmmoCount = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	int AmmoBaseNumber = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float TraceDamage = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	class UForceFeedbackEffect* ShootForceEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	TSubclassOf<class UCameraShakeBase> ShootShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 1000;

private:

	FTimerHandle ReloadTimerHandle;

	bool bIsReadyToFire = false;

public:
	ACannon();

	void Fire();

	bool IsReadyToFire();
	void SpecialFire();

	int bulletcnt = 10;
	void SetVisibility(bool bIsVisible);
	int GetAmmoParam(bool factor);
	void ReplenishAmmo();
	ECannonType GetType();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	void Reload();
public:
	void SetCannonRotation(float Angle)
	{
		FRotator DesiredRotation = GetActorRotation();
		DesiredRotation.Pitch = Angle;
		SetActorRotation(DesiredRotation);
	}
	TSubclassOf<class AProjectile> GetProjectile()
	{
		return ProjectileClass;
	}
	UArrowComponent* GetProjectileSpawnPoint()
	{
		return ProjectileSpawnPoint;
	}
	void SetMoveSpeed(float InMoveSpeed)
	{
		MoveSpeed = InMoveSpeed;
	}

};
