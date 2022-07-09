// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FInventoryItemInfo.h"
#include "HealthComponent.h"


#include "InventoryCellWidget.generated.h"

class UInventoryComponent;


UCLASS()
class TANK_API UInventoryCellWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	 
	UPROPERTY(BluePrintReadOnly, Meta = (BindWidget))
	class UImage* ItemImage;

	UPROPERTY(BluePrintReadOnly, Meta = (BindWidget))
	class UTextBlock* CountText;

	bool bHasItem;

	UPROPERTY()
	FInventorySlotInfo InventorySlot;

	UPROPERTY()
	FInventoryItemsInfo ItemsInfo;

	UPROPERTY()
	class UHealthComponent* HealthComponent;

public:

	UImage* GetImage() {
		return ItemImage;
	}

	bool HasItem() { return bHasItem; }

	bool AddItem(const FInventorySlotInfo& SlotInfo, const FInventoryItemsInfo& ItemInfo);

	void Clear();

	void Collapse();

	void UnCollapse();
	
	FInventoryItemsInfo* GetItemsInfo() {
		return &ItemsInfo;
	}

	FInventorySlotInfo* GetItem() { return &InventorySlot; }

	UPROPERTY(EditAnywhere)
	int IndexInInventory = INDEX_NONE;

	FOnItemDrop OnItemDrop;

	UPROPERTY()
	class UInventoryWidget* ParentInventoryWidget;

	UInventoryComponent* GetParentInventory() const;

protected:

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UPROPERTY(EditAnywhere)
	bool bCanDrag = true;

	
};
