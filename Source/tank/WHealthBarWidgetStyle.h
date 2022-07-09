// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "Styling/SlateWidgetStyleContainerBase.h"

#include "WHealthBarWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct TANK_API FWHealthBarStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FWHealthBarStyle();
	virtual ~FWHealthBarStyle();

	UPROPERTY(EditAnywhere, Category = Appearance)
		FProgressBarStyle HealthBarStyle;

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FWHealthBarStyle& GetDefault();
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UWHealthBarWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FWHealthBarStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};
