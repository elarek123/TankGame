

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FInventoryItemInfo.h"
#include "InventoryManagerComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANK_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	
	UPROPERTY()
	class UInventoryComponent* LocalInventoryComponent;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* ItemsData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UInventoryWidget> InventoryWidgetClass;

	UPROPERTY(EditAnywhere)
	int MinInventorySize = 20;

	UPROPERTY()
	class UInventoryWidget* InventoryWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UInventoryWidget> EquipWidgetClass;

	UPROPERTY()
	class UInventoryWidget* EquipWidget;

public:	
	 void OnItemDropFunc(UInventoryCellWidget* From, UInventoryCellWidget* To);
	// Sets default values for this component's properties
	
	void Init(UInventoryComponent* InInventoryComponent);

	void InitChest(UInventoryComponent* InInventoryComponent);

	void InitEquip(UInventoryComponent* InInventoryComponent);

	const FInventoryItemsInfo* GetItemData(const FName& Id);

	//void CollapseByRarity(EItemRarity ItemRarity);

	void MySwap(UInventoryCellWidget* From, UInventoryCellWidget* To);

};
