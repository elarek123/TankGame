// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "TankDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class TANK_API UTankDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
	UPROPERTY()
	class UInventoryCellWidget* SourceCell;
};
