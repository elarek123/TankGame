// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"


UCLASS()
class TANK_API ATankPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

protected:

	UPROPERTY(VisibleDefaultsOnly, BluePrintReadWrite, Category = "Components")
	class UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleDefaultsOnly, BluePrintReadWrite, Category = "Components")
	class UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleDefaultsOnly, BluePrintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BluePrintReadWrite, Category = "Components")
	class UCameraComponent* Camera;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* CannonSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Movement | Speed")
	float RotationSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Movement | Speed")
	float MoveSpeed = 1000.f;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Movement | Speed")
	float MovementSmoothness = 0.2;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Movement | Speed")
	float RotationSmoothness = 0.2;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Turret")
	float TurretRotationSmoothness = 0.1;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Turret")
	TSubclassOf<class ACannon> DefaultCannonClass;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	 
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float InAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void RotateRight(float InAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SetTurretTargetPosition(FVector& TargetPosition);

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SpecialFire();

private:
	void SetupCannon();

	UPROPERTY()
	class ACannon* Cannon = nullptr;

	float TargetMoveForwardAxis = 0;
	float CurrentMoveForwardAxis = 0;
	float CurrentRotateRightAxis = 0;
	float TargetRotateRightAxis = 0;
	
	FVector TurretTargetPosition;
};
