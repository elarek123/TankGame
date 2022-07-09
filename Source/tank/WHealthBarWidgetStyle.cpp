// Fill out your copyright notice in the Description page of Project Settings.


#include "WHealthBarWidgetStyle.h"

FWHealthBarStyle::FWHealthBarStyle()
{
	
}

FWHealthBarStyle::~FWHealthBarStyle()
{
}

const FName FWHealthBarStyle::TypeName(TEXT("FWHealthBarStyle"));

const FWHealthBarStyle& FWHealthBarStyle::GetDefault()
{
	static FWHealthBarStyle Default;
	return Default;
}

void FWHealthBarStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	Super::GetResources(OutBrushes);

	HealthBarStyle.GetResources(OutBrushes);
}

