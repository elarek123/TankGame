// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <GameFramework/Pawn.h>
#include "EquipInterface.h"
#include "InventoryCharacter.generated.h"



UCLASS()
class TANK_API AInventoryCharacter : public APawn, public IEquipInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AInventoryCharacter();
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class UInventoryComponent* LocalInventory;

	UPROPERTY(EditDefaultsOnly)
	class UEquipInventoryComponent* EquipInventory;

	UPROPERTY(EditDefaultsOnly)
	class UInventoryManagerComponent* InventoryManager;

public:
	virtual void EquipItem(EEquipSlot InSlot, FName ItemId) override;

	virtual void UnEquipItem(EEquipSlot InSlot, FName ItemId) override;
protected:

	UStaticMeshComponent* GetEquipComponent(EEquipSlot EquipSlot);
};
