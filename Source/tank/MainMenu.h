// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include <Animation/WidgetAnimation.h>
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class TANK_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	FTimerHandle TimerHandle;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UButton* NewGameButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UButton* QuitButton;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* InvisAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level loading params")
	FName LoadLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Params")
	float DelayDuration;

	UFUNCTION()
	void OnNewGameClicked();

	UFUNCTION()
	void OnQuitClicked();

	UFUNCTION()
	void LoadingLevel();
};
