// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuHud.h"
#include <Blueprint/UserWidget.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

UUserWidget* AMenuHud::ShowWindow(const EWidgetID WidgetId, const int ZOrder)
{
	HideWidget();

	TSubclassOf<UUserWidget>* ClassPtr = WidgetClasses.Find(WidgetId);
	if (ClassPtr && *ClassPtr)
	{
		CreateWidgetByClass(*ClassPtr, ZOrder);
	}
	return CurrentWidget;
}

void AMenuHud::HideWidget()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}
}

UUserWidget* AMenuHud::GetCurrentWidget() const
{
	return CurrentWidget;
}

void AMenuHud::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld())
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PC, nullptr, EMouseLockMode::DoNotLock, false);
			PC->bShowMouseCursor = true;
		}
	}
}

UUserWidget* AMenuHud::CreateWidgetByClass(const TSubclassOf<UUserWidget> WidgetClass, const int ZOrder)
{
	CurrentWidget = CreateWidget(GetWorld(), WidgetClass);
	CurrentWidget->AddToViewport(ZOrder);
	return CurrentWidget;
}