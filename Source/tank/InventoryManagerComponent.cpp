// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerComponent.h"
#include "InventoryComponent.h"
#include "InventoryWidget.h"
#include "InventoryCellWidget.h"
#include <Components/UniformGridPanel.h>
#include "EquipInventoryComponent.h"
// Sets default values for this component's properties

void UInventoryManagerComponent::Init(UInventoryComponent* InInventoryComponent)
{
	LocalInventoryComponent = InInventoryComponent;

	if (LocalInventoryComponent && ItemsData && InventoryWidgetClass) {

		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
		InventoryWidget->AddToViewport();
		InventoryWidget->ParentInventory = InInventoryComponent;

		InventoryWidget->Init(FMath::Max(MinInventorySize, LocalInventoryComponent->GetItemsNum()));
		InventoryWidget->OnItemDrop.AddUObject(this, &ThisClass::OnItemDropFunc);


		for (const auto& Item : LocalInventoryComponent->GetItems()) {
			
			auto* Data = GetItemData(Item.Value.Id);

			if (Data) {
				InventoryWidget->AddItem(Item.Value, *Data, Item.Key);
			}

		}
		InventoryWidget->SetManager(this);
	}
}

void UInventoryManagerComponent::InitChest(UInventoryComponent* InInventoryComponent)
{
	LocalInventoryComponent = InInventoryComponent;

	if (LocalInventoryComponent && ItemsData && InventoryWidgetClass) {

		InventoryWidget->ParentInventory = InInventoryComponent;

		InventoryWidget->InitChest(FMath::Max(MinInventorySize, LocalInventoryComponent->GetItemsNum()));
		InventoryWidget->OnItemDrop.AddUObject(this, &ThisClass::OnItemDropFunc);


		for (const auto& Item : LocalInventoryComponent->GetItems()) {

			auto* Data = GetItemData(Item.Value.Id);

			if (Data) {
				InventoryWidget->AddToChestItem(Item.Value, *Data, Item.Key);
			}

		}
	}
	InventoryWidget->SetManager(this);
}

void UInventoryManagerComponent::InitEquip(UInventoryComponent* InInventoryComponent)
{
	if (InInventoryComponent && EquipWidgetClass) {
		EquipWidget = CreateWidget<UInventoryWidget>(GetWorld(), EquipWidgetClass);
		EquipWidget->OnItemDrop.AddUObject(this, &ThisClass::OnItemDropFunc);
		EquipWidget->AddToViewport();
		EquipWidget->ParentInventory = InInventoryComponent;
	}
}

const FInventoryItemsInfo* UInventoryManagerComponent::GetItemData(const FName& Id) 
{
	return ItemsData ? ItemsData->FindRow<FInventoryItemsInfo>(Id, "") : nullptr;
}


void UInventoryManagerComponent::MySwap(UInventoryCellWidget* From, UInventoryCellWidget* To)
{
	if (From == nullptr || To == nullptr)
	{
		return;
	}

	auto* FromInventory = From->GetParentInventory();
	auto* ToInventory = To->GetParentInventory();

	if (FromInventory == nullptr || ToInventory == nullptr)
	{
		return;
	}

	FInventorySlotInfo FromItem = *From->GetItem();
	if (FromItem.Count == 0)
	{
		return;
	}

	FInventorySlotInfo ToItem = *To->GetItem();

	const FInventoryItemsInfo* FromInfo = GetItemData(FromItem.Id);
	const FInventoryItemsInfo* ToInfo = ToItem.Count > 0 ? GetItemData(ToItem.Id) : nullptr;


	FInventorySlotInfo NewFromItem = ToItem;
	FInventorySlotInfo NewToItem = FromItem;

	if (ToItem.Count == 0)
	{
		ToItem.Count += FromItem.Count;

		From->Clear();

		To->Clear();
		To->AddItem(FromItem, *FromInfo);

		FromInventory->ClearItem(From->IndexInInventory);
		ToInventory->SetItem(From->IndexInInventory, FromItem);
		return;
	}

	if (ToItem.Count > 0)
	{
		if (NewFromItem.Id == NewToItem.Id)
		{
			NewToItem.Count = FromItem.Count + ToItem.Count;
			NewFromItem.Count = 0;
		}
		else if (!ToInfo)
		{
			UE_LOG(LogTemp, Error, TEXT("ProblemHere"));

		}
	}


	const FInventoryItemsInfo* NewFromInfo = NewFromItem.Count > 0 ? GetItemData(NewFromItem.Id) : nullptr;
	const FInventoryItemsInfo* NewToInfo = GetItemData(NewToItem.Id);

	if (Cast<UEquipInventoryComponent>(From->GetParentInventory()) && NewFromInfo)
	{
		if (NewToInfo->Type == EItemType::It_Equip && NewToInfo->EquipSlot == NewFromInfo->EquipSlot)
		{
			From->Clear();
			From->AddItem(NewFromItem, *NewFromInfo);

			To->Clear();
			To->AddItem(NewToItem, *NewToInfo);

			FromInventory->SetItem(From->IndexInInventory, NewFromItem);
			ToInventory->SetItem(To->IndexInInventory, NewToItem);
		}
	}
	else
	{
		From->Clear();
		if (NewFromInfo)
		{
			int b = 100;
			From->AddItem(NewFromItem, *NewFromInfo);
		}

		To->Clear();
		To->AddItem(NewToItem, *NewToInfo);

		FromInventory->SetItem(From->IndexInInventory, NewFromItem);
		ToInventory->SetItem(To->IndexInInventory, NewToItem);
	}
}

void UInventoryManagerComponent::OnItemDropFunc(UInventoryCellWidget* From, UInventoryCellWidget* To)
{
	MySwap(From, To);
}

