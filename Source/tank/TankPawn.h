// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicClass.h"
#include "TankPawn.generated.h"


UCLASS()
class TANK_API ATankPawn : public ABasicClass
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

protected:


	UPROPERTY(VisibleDefaultsOnly, BluePrintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BluePrintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Movement | Speed")
	float RotationSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Movement | Speed")
	float MoveSpeed = 1000.f;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Movement | Speed")
	float MovementSmoothness = 0.2;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Movement | Speed")
	float RotationSmoothness = 0.2;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Turret")
	int CannonLimit = 2;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Turret")
	int CurrentCannonNumber = 0;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Turret")
	int CurrentCannonIndex = 0;

	

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Turret")
	TArray <FString>  CannonPackNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params", Meta = (MakeEditWidget = true))
	TArray<class ATargetPoint*> PatrollingPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params")
	float MovementAccuracy = 50;



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
	void SetTurretTargetPosition(const FVector& TargetPosition);

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SpecialFire();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void WeaponSwap();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SetupCannon(TSubclassOf<class ACannon> InCannonClass);

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "AI|Move params")
	void SetPatrollingPoints(TArray<class ATargetPoint*>& InPoints) {
		PatrollingPoints = InPoints;
	}

	UFUNCTION(BlueprintPure, Category = "AI|Move params")
	FVector GetTurretForwardVector();

	UFUNCTION(BlueprintPure, Category = "AI|Move params")
	TArray<class ATargetPoint*>& GetPatrollingPoints() {
		return PatrollingPoints; 
	};

	UFUNCTION(BlueprintPure, Category = "AI|Move params")
	float GetMovementAccuracy() {
		return MovementAccuracy;
	};

	ACannon* GetCannon();

	UArrowComponent* GetCannonSpawnPoint()
	{
		return CannonSpawnPoint;
	}

private:


	float TargetMoveForwardAxis = 0;
	float CurrentMoveForwardAxis = 0;
	float CurrentRotateRightAxis = 0;
	float TargetRotateRightAxis = 0;
	
	FVector TurretTargetPosition;
};
