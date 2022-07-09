// Fill out your copyright notice in the Description page of Project Settings.


#include "ElarekWWidgetStyle.h"
#include <Styling/ISlateStyle.h>
#include <Styling/CoreStyle.h>
#include "MyCoreStyle.h"


FElarekWStyle::FElarekWStyle()
{
	//CheckBoxStyle = FCoreStyle::Get().GetWidgetStyle<FCheckBoxStyle>("RadioButton");
	//TextBlockStyle = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalUnderlinedText");

}

FElarekWStyle::~FElarekWStyle()
{
}

const FName FElarekWStyle::TypeName(TEXT("FElarekWStyle"));

const FElarekWStyle& FElarekWStyle::GetDefault()
{
	static FElarekWStyle Default;
	return Default;
}

void FElarekWStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	Super::GetResources(OutBrushes);

	//CheckBoxStyle.GetResources(OutBrushes);
//	TextBlockStyle.GetResources(OutBrushes);
}
