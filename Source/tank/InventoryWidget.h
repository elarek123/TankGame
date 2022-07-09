// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FInventoryItemInfo.h"


#include "InventoryWidget.generated.h"



class UInventoryManagerComponent;
/**
 * 
 */
UCLASS()
class TANK_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;

	void Init(int ItemsCount);
	
	void InitChest(int ItemsCount);

	void Sort();

	void CollapseByRarity(EItemRarity ItemRarity);

	void InitCell(UInventoryCellWidget* NewCell);

	bool AddItem(const FInventorySlotInfo& SlotInfo, const FInventoryItemsInfo& ItemInfo, int SlotIndex);

	bool AddToChestItem(const FInventorySlotInfo& SlotInfo, const FInventoryItemsInfo& ItemInfo, int SlotIndex);


	FOnItemDrop OnItemDrop;

	UPROPERTY()
	class UInventoryComponent* ParentInventory;

	void SetManager(UInventoryManagerComponent* Manager);

protected:

	UPROPERTY(EditDefaultsOnly)
	int ItemsInRow = 5;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	class UUniformGridPanel* CellsPanel;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	class UUniformGridPanel* InventoryPanel;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	class UButton* CommonButton;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	class UButton* RareButton;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	class UButton* EpicButton;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	class UButton* AllButton;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInventoryCellWidget> CellWidgetClass;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	UInventoryCellWidget* GoldCll;

	UPROPERTY(BlueprintReadWrite)
	TArray<UInventoryCellWidget*> CellWidgets;

	TArray<UInventoryCellWidget*> ChestCellWidgets;

	UInventoryCellWidget* CreateCell();

	UInventoryCellWidget* CreateChestCell();

	void OnItemDropFunc(UInventoryCellWidget* From, UInventoryCellWidget* To);

	UFUNCTION()
	void OnCommonButtonClicked();

	UFUNCTION()
	void OnRareButtonClicked();

	UFUNCTION()
	void OnEpicButtonClicked();

	UFUNCTION()
	void OnAllButtonClicked();

	class UInventoryManagerComponent* MyManager;

};
