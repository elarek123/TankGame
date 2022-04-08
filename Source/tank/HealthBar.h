// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "BasicClass.h"
#include <Components/ProgressBar.h>
#include "HealthComponent.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class TANK_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UHealthComponent* Health;
	void SetHealthComponent(UHealthComponent* HealthComponent) {
		Health = HealthComponent;
	}

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UProgressBar* HealthBar;



};
