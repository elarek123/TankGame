// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include <Templates/SharedPointer.h>
#include "ElarekWWidgetStyle.h"

DECLARE_DELEGATE_OneParam(FOnRadioButtonChanged, int /*New SelectedIndex*/)

class TANK_API SRadioButtons : public SCompoundWidget
{

public:
	SLATE_BEGIN_ARGS(SRadioButtons)
	{}

	SLATE_STYLE_ARGUMENT(FElarekWStyle, Style);

	SLATE_EVENT(FOnRadioButtonChanged, OnRadioButtonChanged);

	SLATE_END_ARGS()

		FOnRadioButtonChanged OnRadioButtonChanged;

public:
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
private:

	int CurrentIndex = 0;

	ECheckBoxState IsChecked(int InIndex) const;

	void OnCheckboxStateChanged(ECheckBoxState NewState, int Index);

	TSharedRef<SWidget> CreateCheckBox(int Index, FString InText);

	const FCheckBoxStyle* CheckBoxStyle = nullptr;

	const FTextBlockStyle* TextBlockStyle = nullptr;

};