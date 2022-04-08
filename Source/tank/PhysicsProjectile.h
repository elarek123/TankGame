// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "PhysicsMovementComponent.h"
#include "PhysicsProjectile.generated.h"

/**
 * 
 */
UCLASS()
class TANK_API APhysicsProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	APhysicsProjectile();
	virtual void Start() override;
	virtual void Stop() override;
	virtual void Tick(float DeltaTime) override;

	virtual void OnMeshHit(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult) override;
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UPhysicsMovementComponent* MoveComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage Params")
	float ExplodeRadius = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage Params")
	float PushForce = 100;
public:
	float GetGravity()
	{
		return -MoveComponent->Gravity.Z;
	}




};
