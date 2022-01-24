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

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Movement | Speed")
	float RotationSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Movement | Speed")
	float MoveSpeed = 1000.f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	 
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float InAxisValue);
private:
	float TargetMoveForwardAxis = 0;
	
};
