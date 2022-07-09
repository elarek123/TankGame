// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"
#include "MyCoreStyle.h"








void UHealthBar::NativePreConstruct()
{
	if (HealthBar) {
		HealthBar->FillColorAndOpacity = MyCoreStyle::Get().GetColor("Green");
	}
}

void UHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (HealthBar) {
		HealthBar->SetPercent(Health->GetHealth() / Health->MaxHealth);
	}
}
