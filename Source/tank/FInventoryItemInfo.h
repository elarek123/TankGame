// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <UObject/Object.h>
#include <Engine/DataTable.h>
#include <FInventoryItemInfo.generated.h>

/**
 * 
 */

class UInventoryCellWidget;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemDrop, UInventoryCellWidget*, UInventoryCellWidget*)

UENUM()
enum class EItemType : uint8
{
	It_Mics,
	It_Currency,
	It_Equip,
	It_Consumable,
};


UENUM()
enum class EItemRarity : uint8
{
	Ir_Common,
	Ir_Rare,
	Ir_Epic,
	Ir_All,
};

UENUM()
enum class EEquipSlot : uint8
{
	Es_Head,
	Es_LeftHand,
	Es_RightHand,
};

USTRUCT(BlueprintType)
struct FInventoryItemsInfo : public FTableRowBase {
public:
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,Category = "General")
	FName Id;

	UPROPERTY(EditDefaultsOnly, Category = "General")
	FText Name;

	UPROPERTY(EditDefaultsOnly, Category = "General")
	EItemType Type;

	UPROPERTY(EditDefaultsOnly, Category = "General")
	EItemRarity Rarity;

	FString Vision = FString("SelfHitTestInvisible");

	UPROPERTY(EditDefaultsOnly, Category = "General")
	EEquipSlot EquipSlot;

	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	TSoftObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int Armor;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int Intelligence;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int HealthBuff;
};

USTRUCT(BlueprintType)
struct FInventorySlotInfo : public FTableRowBase {
public:
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "General")
	FName Id = NAME_None;

	UPROPERTY(EditDefaultsOnly, Category = "General")
	int Count;
};