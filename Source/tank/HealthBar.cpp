// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"






void UHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (HealthBar) {
		HealthBar->SetPercent(Health->GetHealth() / Health->MaxHealth);
	}
}
