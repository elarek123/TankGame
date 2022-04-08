// Fill out your copyright notice in the Description page of Project Settings.


#include "SRadioButtons.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SRadioButtons::Construct(const FArguments& InArgs)
{
	CheckBoxStyle = &InArgs._Style->CheckBoxStyle;
	TextBlockStyle = &InArgs._Style->TextBlockStyle;

	OnRadioButtonChanged = InArgs._OnRadioButtonChanged;

	ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()[
				CreateCheckBox(0, "Option 1")
			]
			+ SVerticalBox::Slot()[
				CreateCheckBox(1, "Option 2")
			]
			+ SVerticalBox::Slot()[
				CreateCheckBox(2, "Option 3")
			]

		];

}

ECheckBoxState SRadioButtons::IsChecked(int InIndex) const
{
	return InIndex == CurrentIndex ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SRadioButtons::OnCheckboxStateChanged(ECheckBoxState NewState, int Index)
{
	if (NewState == ECheckBoxState::Checked) {
		CurrentIndex = Index;
		OnRadioButtonChanged.ExecuteIfBound(CurrentIndex);
	}
}

TSharedRef<SWidget> SRadioButtons::CreateCheckBox(int Index, FString InText)
{
	return SNew(SCheckBox)
		.IsChecked_Raw(this, &SRadioButtons::IsChecked, Index)
		.OnCheckStateChanged_Raw(this, &SRadioButtons::OnCheckboxStateChanged, Index)
		.Style(CheckBoxStyle)
		[
			SNew(STextBlock)
			.Text(FText::FromString(InText))
		.TextStyle(TextBlockStyle)
		];
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
