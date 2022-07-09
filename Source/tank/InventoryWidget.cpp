// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "InventoryCellWidget.h"
#include <Components/Button.h>
#include "InventoryCellWidget.h"
#include <Components/Image.h>
#include <Components/UniformGridPanel.h>
#include <Containers/Array.h>
#include "InventoryManagerComponent.h"


void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	for (auto* Cell : CellWidgets) {
		InitCell(Cell);
	}

	for (auto* Cell : ChestCellWidgets) {
		InitCell(Cell);
	}

	if (CommonButton)
	{
		CommonButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnCommonButtonClicked);
	}

	if (RareButton)
	{
		RareButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnRareButtonClicked);
	}

	if (EpicButton) {
		EpicButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnEpicButtonClicked);
	}
	if (AllButton) {
		AllButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnAllButtonClicked);
	}
}

void UInventoryWidget::Init(int ItemsCount)
{
	if (CellsPanel) {
		CellsPanel->ClearChildren();

		for (int i = 0; i < ItemsCount; i++) {

			if (auto* Cell = CreateCell()) {
				Cell->IndexInInventory = i;
				CellsPanel->AddChildToUniformGrid(Cell, i / ItemsInRow, i % ItemsInRow);
			}
		}
	}
}

void UInventoryWidget::InitChest(int ItemsCount)
{
	if (InventoryPanel) {
		InventoryPanel->ClearChildren();

		for (int i = 0; i < ItemsCount; i++) {

			if (auto* Cell = CreateChestCell()) {
				Cell->IndexInInventory = i;
				InventoryPanel->AddChildToUniformGrid(Cell, i / ItemsInRow, i % ItemsInRow);
			}
		}
	}
}

void UInventoryWidget::Sort()
{

	for (auto* i : CellsPanel->GetAllChildren()) {
		for (auto* j : CellsPanel->GetAllChildren()) {
			if (i > j)
				continue;
			auto* a = Cast<UInventoryCellWidget>(i);
			auto* b = Cast<UInventoryCellWidget>(j);
			if (a->GetImage()->GetVisibility() != ESlateVisibility::Collapsed)
				break;
			if (b->GetImage()->GetVisibility() == ESlateVisibility::SelfHitTestInvisible) {
				MyManager->MySwap(a, b);
			}
		}
	}
}

void UInventoryWidget::CollapseByRarity(EItemRarity ItemRarity)
{
	for (auto* Cell : CellsPanel->GetAllChildren())
	{
		UInventoryCellWidget* NewCell = Cast<UInventoryCellWidget>(Cell);
		if (ItemRarity != EItemRarity::Ir_All && NewCell->GetItemsInfo()->Rarity != ItemRarity)
		{
			NewCell->Collapse();
		}
		else
		{
			NewCell->UnCollapse();
		}
	}
	Sort();
}

void UInventoryWidget::InitCell(UInventoryCellWidget* NewCell)
{
	if (NewCell) {
		NewCell->OnItemDrop.AddUObject(this, &ThisClass::OnItemDropFunc);
		NewCell->ParentInventoryWidget = this;
	}
}

bool UInventoryWidget::AddItem(const FInventorySlotInfo& SlotInfo, const FInventoryItemsInfo& ItemInfo, int SlotIndex)
{
	if (ItemInfo.Type == EItemType::It_Currency) {
		if (GoldCll) {
			return GoldCll->AddItem(SlotInfo, ItemInfo);
		}
	}

	if (CellsPanel) {
		UInventoryCellWidget* Found = nullptr;
		UInventoryCellWidget** FoundPtr = CellWidgets.FindByPredicate([SlotIndex](UInventoryCellWidget* Cell)
			{
				return Cell && Cell->IndexInInventory == SlotIndex;
			});
		if (FoundPtr) {
			Found = *FoundPtr;
		}
		else {
			for (auto* Cell : CellWidgets) {
				if (!Cell->HasItem()) {
					Found = Cell;
					break;
				}
			}
		}
		if (Found) {
			return Found->AddItem(SlotInfo, ItemInfo);
		}
	}
	return false;
}


bool UInventoryWidget::AddToChestItem(const FInventorySlotInfo& SlotInfo, const FInventoryItemsInfo& ItemInfo, int SlotIndex)
{
	if (InventoryPanel) {
		UInventoryCellWidget* Found = nullptr;
		UInventoryCellWidget** FoundPtr = ChestCellWidgets.FindByPredicate([SlotIndex](UInventoryCellWidget* Cell)
			{
				return Cell && Cell->IndexInInventory == SlotIndex;
			});
		if (FoundPtr) {
			Found = *FoundPtr;
		}
		else {
			for (auto* Cell : ChestCellWidgets) {
				if (!Cell->HasItem()) {
					Found = Cell;
					break;
				}
			}
		}
		if (Found) {
			return Found->AddItem(SlotInfo, ItemInfo);
		}
	}
	return false;
}


UInventoryCellWidget* UInventoryWidget::CreateCell()
{
	if (CellWidgetClass) {
		auto* Cell = CreateWidget<UInventoryCellWidget>(this, CellWidgetClass);
		CellWidgets.Add(Cell);

		InitCell(Cell);

		return Cell;
	}
	return nullptr;
}

UInventoryCellWidget* UInventoryWidget::CreateChestCell()
{
	if (CellWidgetClass) {
		auto* Cell = CreateWidget<UInventoryCellWidget>(this, CellWidgetClass);
		ChestCellWidgets.Add(Cell);

		Cell->OnItemDrop.AddUObject(this, &ThisClass::OnItemDropFunc);
		Cell->ParentInventoryWidget = this;

		return Cell;
	}
	return nullptr;
}

void UInventoryWidget::OnItemDropFunc(UInventoryCellWidget* From, UInventoryCellWidget* To)
{
	if (OnItemDrop.IsBound())
	{
		OnItemDrop.Broadcast(From, To);
	}
}

void UInventoryWidget::OnCommonButtonClicked()
{
	CollapseByRarity(EItemRarity::Ir_Common);
}

void UInventoryWidget::OnRareButtonClicked()
{
	CollapseByRarity(EItemRarity::Ir_Rare);
}

void UInventoryWidget::OnEpicButtonClicked()
{
	CollapseByRarity(EItemRarity::Ir_Epic);
}

void UInventoryWidget::OnAllButtonClicked()
{
	CollapseByRarity(EItemRarity::Ir_All);
}

void UInventoryWidget::SetManager(UInventoryManagerComponent* Manager)
{
	MyManager = Manager;
}

