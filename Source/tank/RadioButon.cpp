// Fill out your copyright notice in the Description page of Project Settings.


#include "RadioButton.h"
#include <Widgets/DeclarativeSyntaxSupport.h>

void URadioButton::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	RadioButtons.Reset();
}

TSharedRef<SWidget> URadioButton::RebuildWidget()
{
	RadioButtons = SNew(SRadioButtons)
		.OnRadioButtonChanged_UObject(this, &ThisClass::OnRadioButtonsChangedFunc)
		.Style(&WidgetStyle);
	return RadioButtons.ToSharedRef();
}

void URadioButton::OnRadioButtonsChangedFunc(int NewSelectButton)
{
	OnRadioButtonsChanged.Broadcast(NewSelectButton);
}