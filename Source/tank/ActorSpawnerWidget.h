// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Turret.h"
#include <UObject/NoExportTypes.h>
#include <Engine/EngineTypes.h>
#include "ActorSpawnerWidget.generated.h"

/**
 * 
 */
UCLASS()
class TANK_API UActorSpawnerWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativePreConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABasicClass> SpawnerClass;

	UPROPERTY()
	class ABasicClass* SpawnedActor = nullptr;

	UPROPERTY()
	class ATankController* PLayerController;

	FVector WorldMousePostion;

	FVector WorldMouseDirection;

	FHitResult OutHit;

	FTransform SpawnTransform = FTransform(FRotator(0, 0, 0), OutHit.Location, FVector(1));

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* DraggedText;

	UPROPERTY(EditAnywhere)
	FText DraggedName;

	void OnMouseButtonUp();


};
