// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"

#include "EquipInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANK_API UEquipInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:	
	
	UEquipInventoryComponent();

	UPROPERTY(EditAnywhere)
	TMap<int, EEquipSlot> EquipSlots;

	virtual int GetMaxItemAmount(int SlotIndex, const FInventoryItemsInfo& InItem) override;


	virtual void SetItem(int SlotIndex, const FInventorySlotInfo& Item) override;

	virtual void ClearItem(int SlotIndex) override;

};
