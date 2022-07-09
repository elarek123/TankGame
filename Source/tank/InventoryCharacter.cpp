// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryCharacter.h"
#include "InventoryComponent.h"
#include "InventoryManagerComponent.h"
#include "EquipInventoryComponent.h"


// Sets default values
AInventoryCharacter::AInventoryCharacter()
{
	LocalInventory = CreateDefaultSubobject<UInventoryComponent>("LocalInventory");
	EquipInventory = CreateDefaultSubobject<UEquipInventoryComponent>("EquipInventory");
	InventoryManager = CreateDefaultSubobject<UInventoryManagerComponent>("InventoryManager");

}

void AInventoryCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GetController() == GetWorld()->GetFirstPlayerController()) {
		InventoryManager->Init(LocalInventory);
		InventoryManager->InitEquip(EquipInventory);
	}
}

void AInventoryCharacter::EquipItem(EEquipSlot InSlot, FName ItemId)
{
	UStaticMeshComponent* Comp = GetEquipComponent(InSlot);

	if (Comp) {
		auto* ItemInfo = InventoryManager->GetItemData(ItemId);
		Comp->SetStaticMesh(ItemInfo->Mesh.LoadSynchronous());
		Comp->SetHiddenInGame(false);

	}
}

void AInventoryCharacter::UnEquipItem(EEquipSlot InSlot, FName ItemId)
{
	UStaticMeshComponent* Comp = GetEquipComponent(InSlot);

	if (Comp) {
		Comp->SetHiddenInGame(true);
	}
}

UStaticMeshComponent* AInventoryCharacter::GetEquipComponent(EEquipSlot EquipSlot)
{
	FName Tag;

	switch (EquipSlot)
	{
		case EEquipSlot::Es_Head :
			Tag = "Head";
			break;

		case EEquipSlot::Es_LeftHand:
			Tag = "LeftHand";
			break;

		case EEquipSlot::Es_RightHand:
			Tag = "RightHand";
			break;

	}

	TArray<UActorComponent*> Found = GetComponentsByTag(UStaticMeshComponent::StaticClass(), Tag);

	return Found.IsValidIndex(0) ? Cast<UStaticMeshComponent>(Found[0]) : nullptr;
}
