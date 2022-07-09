// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryCellWidget.h"
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include "FInventoryItemInfo.h"
#include <Internationalization/Text.h>
#include <Blueprint/WidgetBlueprintLibrary.h>
#include "InventoryWidget.h"
#include <Components/SlateWrapperTypes.h>
#include "TankDragDropOperation.h"
#include "BasicClass.h"


FReply UInventoryCellWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bCanDrag && bHasItem && InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	if (bHasItem && InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) && ItemsInfo.Type == EItemType::It_Consumable)
	{	
		int Health = ItemsInfo.HealthBuff;
		if (InventorySlot.Count > 1) {
			InventorySlot.Count--;
			CountText->SetText(FText::FromString(FString::FromInt(InventorySlot.Count)));
		}
		else {
			Clear();
		}
		ABasicClass* Player = Cast<ABasicClass>(GetWorld()->GetFirstPlayerController()->GetPawn());
		FDamageData HealthChange;
		HealthChange.DamageValue = -Health;
		Player->GetHealtComponent()->TakeDamage(HealthChange);
	}

	return FReply::Handled();
}

void UInventoryCellWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UTankDragDropOperation::StaticClass());

	auto* DragVisual = CreateWidget<UInventoryCellWidget>(GetWorld(), GetClass());

	if (DragVisual && DragVisual->ItemImage)
	{
		DragVisual->ItemImage->SetBrushFromTexture(ItemsInfo.Icon.LoadSynchronous());
		DragVisual->ItemImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CountText->SetText(FText::FromString(FString::FromInt(InventorySlot.Count)));
		DragVisual->CountText->SetText(CountText->Text);
	}

	if (OutOperation)
	{
		auto InventoryDragDropOperation = Cast<UTankDragDropOperation>(OutOperation);

		if (InventoryDragDropOperation)
		{
			InventoryDragDropOperation->SourceCell = this;
			InventoryDragDropOperation->DefaultDragVisual = DragVisual;

		}
	}
}

bool UInventoryCellWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	auto InventoryDragDropOperation = Cast<UTankDragDropOperation>(InOperation);

	if (InventoryDragDropOperation && InventoryDragDropOperation->SourceCell != this)
	{

		if (OnItemDrop.IsBound())
		{
			OnItemDrop.Broadcast(InventoryDragDropOperation->SourceCell, this);
			GLog->Log(ELogVerbosity::Warning, TEXT("OnDrop TRUE!"));
			return true;

		}
	}
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}




bool UInventoryCellWidget::AddItem(const FInventorySlotInfo& InSlot, const FInventoryItemsInfo& Info)
{
	if (bHasItem)
	{
		return false;
	}

	if (ItemImage)
	{
		ItemImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ItemImage->SetBrushFromTexture(Info.Icon.LoadSynchronous());
	}

	if (CountText)
	{
		CountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CountText->SetText(FText::FromString(FString::FromInt(InSlot.Count)));
	}

	ItemsInfo = Info;

	bHasItem = true;
	InventorySlot = InSlot;

	return true;
}

void UInventoryCellWidget::Clear()
{
	Collapse();
	ItemsInfo = FInventoryItemsInfo();
	InventorySlot = FInventorySlotInfo();
	bHasItem = false;
}

void UInventoryCellWidget::Collapse()
{
	if (!bHasItem)
	{
		return;
	}

	if (ItemImage)
	{
		ItemImage->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (CountText)
	{
		CountText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInventoryCellWidget::UnCollapse()
{
	if (!bHasItem)
	{
		return;
	}

	if (ItemImage)
	{
		ItemImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (CountText)
	{
		CountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}


UInventoryComponent* UInventoryCellWidget::GetParentInventory() const
{
	return ParentInventoryWidget ? ParentInventoryWidget->ParentInventory : nullptr;
}