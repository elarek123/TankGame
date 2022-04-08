// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TANK_API AProjectile : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* HitEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UAudioComponent* AudioHitEffect;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Movement | Speed")
	float MoveSpeed = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float Damage = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float FireRange = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
	float Mass = 1;

public:	
	// Sets default values for this actor's properties
	AProjectile();

	virtual void Start();
	virtual void Tick(float DeltaTime) override;
	virtual void Stop();

protected:
	UFUNCTION()
	virtual void OnMeshHit(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);
public:
	void SetMoveSpeed(float InMoveSpeed)
	{
		MoveSpeed = InMoveSpeed;
	}

	float GetMoveSpeed()
	{
		return MoveSpeed;
	}
private:
	FVector StartPosition;
};
