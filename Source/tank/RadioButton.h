// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Templates/SharedPointer.h>
#include "SRadioButtons.h"
#include <Widgets/SWidget.h>
#include <Components/Widget.h>
#include <Delegates/DelegateCombinations.h>
#include "RadioButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRadioButtonChangedEvent, int, NewSelectedButton);

UCLASS()
class TANK_API URadioButton : public UWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintAssignable)
		FOnRadioButtonChangedEvent OnRadioButtonsChanged;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UPROPERTY(EditAnywhere, Category = Appearance, Meta = (DisplayName = Style))
		FElarekWStyle WidgetStyle;

protected:

	virtual TSharedRef<SWidget> RebuildWidget() override;

	TSharedPtr<SRadioButtons> RadioButtons;

	void OnRadioButtonsChangedFunc(int NewSelectButton);
};