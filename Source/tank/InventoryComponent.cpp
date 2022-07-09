// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"




void UInventoryComponent::SetItem(int SlotIndex, const FInventorySlotInfo& Item)
{
	ClearItem(SlotIndex);
	Items.Add(SlotIndex, Item);
}

void UInventoryComponent::ClearItem(int SlotIndex)
{
	Items.Remove(SlotIndex); 
}

int UInventoryComponent::GetMaxItemAmount(int SlotIndex, const FInventoryItemsInfo& InItem)
{
	FInventorySlotInfo* SlotPtr = Items.Find(SlotIndex);
	if (SlotPtr == nullptr || SlotPtr->Id == InItem.Id) {
		return 0;
	}
	return -1;
}
