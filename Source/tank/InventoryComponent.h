// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FInventoryItemInfo.h"
#include "InventoryComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANK_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	 UPROPERTY(EditAnywhere)
	 TMap<int, FInventorySlotInfo> Items;
	 
public:

	const FInventorySlotInfo* GetItem(int SlotIndex) const {
		return Items.Find(SlotIndex);
	}

	virtual void SetItem(int SlotIndex, const FInventorySlotInfo& Item);

	virtual void ClearItem(int SlotIndex);

	virtual int GetMaxItemAmount(int SlotIndex, const FInventoryItemsInfo& InItem);

	const TMap<int, FInventorySlotInfo>& GetItems() const {
		return Items;
	}
	
	int GetItemsNum() {
		return Items.Num();
	}
};
