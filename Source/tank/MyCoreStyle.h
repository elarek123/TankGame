// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Slate.h"
#include <Styling/ISlateStyle.h>
#include <Templates/SharedPointer.h>
/**
 *
 */
class TANK_API MyCoreStyle
{
public:
	static void Initialize();

	static void ShutDown();

	static const ISlateStyle& Get();

	static TSharedPtr<ISlateStyle>FindStyle();

private:
	static TSharedPtr<ISlateStyle> StylePtr;
};