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
	static void Initialize(int InStyle);

	static void ShutDown();

	static void ChangeStyle(int InStyle);

	static int GetStatus() {
		return CurrentStyle;
	}

	static const ISlateStyle& Get();

	static TSharedPtr<ISlateStyle>FindStyle(int InStyle);

protected:
	static int CurrentStyle;

	static TSharedPtr<ISlateStyle> StylePtr;
};