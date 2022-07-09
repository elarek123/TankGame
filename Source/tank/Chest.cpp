// Fill out your copyright notice in the Description page of Project Settings.


#include "Chest.h"
#include "InventoryManagerComponent.h"
#include <Components/StaticMeshComponent.h>
#include <Components/PrimitiveComponent.h>
#include "BasicClass.h"
// Sets default values
// Sets default values
AChest::AChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestMesh"));
	SetRootComponent(BaseMesh);
}

void AChest::Show(UPrimitiveComponent* pComponent, FKey inKey)
{

	
	InventoryManager->InitChest(LocalInventory);
	
}

// Called when the game starts or when spawned
void AChest::BeginPlay()
{

	InventoryManager = Cast<ABasicClass>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetInventoryManager();
	BaseMesh->OnClicked.AddUniqueDynamic(this, &AChest::Show);
	
}