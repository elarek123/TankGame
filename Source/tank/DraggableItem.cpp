// Fill out your copyright notice in the Description page of Project Settings.


#include "DraggableItem.h"
#include <Blueprint/WidgetBlueprintLibrary.h>
#include "TankDragDropOperation.h"
#include <Components/VerticalBox.h>
#include <Types/SlateEnums.h>
#include <Components/VerticalBoxSlot.h>

void UDraggableItem::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (TankText) {
		TankText->SetText(TankName);
	}
}

FReply UDraggableItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) {
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	return FReply::Handled();
}

void UDraggableItem::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UTankDragDropOperation::StaticClass());
	UTankDragDropOperation* TankOperation = Cast<UTankDragDropOperation>(OutOperation);
	
	if (TankOperation) {
		TankOperation->DefaultDragVisual = this;

		SetColorAndOpacity(FLinearColor(1, 1, 1, 0.7));
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		TankOperation->OnDragCancelled.AddDynamic(this, &ThisClass::ReleaseVisual);
	}
	else {
		Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	}
}

void UDraggableItem::ReleaseVisual(UDragDropOperation* InOperation)
{
	UDraggableItem* Item = Cast<UDraggableItem>(InOperation->DefaultDragVisual);
	if (Item) {
		SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
		SetVisibility(ESlateVisibility::Visible);
	}
}

bool UDraggableItem::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UVerticalBox* VerticalBox = Cast<UVerticalBox>(GetParent());
	UTankDragDropOperation* TankOperation = Cast<UTankDragDropOperation>(InOperation);

	if (VerticalBox && TankOperation && TankOperation->DefaultDragVisual && this != TankOperation->DefaultDragVisual) {
		int OverIndex = VerticalBox->GetChildIndex(TankOperation->DefaultDragVisual);
		if (OverIndex >= 0) {
			VerticalBox->RemoveChildAt(OverIndex);

			TArray<UWidget*> Children = VerticalBox->GetAllChildren();
			VerticalBox->ClearChildren();

			for (auto &i : Children) {
				VerticalBox->AddChildToVerticalBox(i)->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
				if (i == this) {
					VerticalBox->AddChildToVerticalBox(TankOperation->DefaultDragVisual)->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
				}
			}
			return true;
		}
	}
	return Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
}

bool UDraggableItem::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (InOperation && InOperation->DefaultDragVisual) {
		ReleaseVisual(InOperation);
	}
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
