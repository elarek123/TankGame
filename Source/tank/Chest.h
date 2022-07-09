// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryCharacter.h"
#include "Chest.generated.h"

UCLASS()
class TANK_API AChest : public AInventoryCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChest();

	UFUNCTION()
		void Show(UPrimitiveComponent* pComponent, FKey inKey);

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* BaseMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};