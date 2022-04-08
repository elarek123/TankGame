// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCoreStyle.h"
#include "Engine.h"
#include "SlateBasics.h"
#include "Slate/SlateGameResources.h"
#include <UObject/NameTypes.h>
#include <Brushes/SlateImageBrush.h>
#include <UObject/NoExportTypes.h>
#include <Styling/SlateStyle.h>
#include <Misc/Paths.h>

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush(Style->RootToContentDir(RelativePath, TEXT(".jpg")), __VA_ARGS__)

TSharedPtr<ISlateStyle>MyCoreStyle::StylePtr;

TSharedPtr<ISlateStyle>MyCoreStyle::FindStyle() {


	FString Path = FPaths::ProjectContentDir() / TEXT("/UI");

	TSharedPtr<FSlateStyleSet> Style = FSlateGameResources::New(FName("MyCoreStyle"), Path, Path);

	Style->Set("Red", FLinearColor::Red);
	Style->Set("DefaultPadding", FMargin(5));
	Style->Set("frog", new IMAGE_BRUSH("frog", FVector2D(256)));

	return Style;
}

#undef IMAGE_BRUSH

void MyCoreStyle::Initialize()
{
	ShutDown();
	StylePtr = FindStyle();
	FSlateStyleRegistry::RegisterSlateStyle(*StylePtr);
}

void MyCoreStyle::ShutDown()
{
	if (StylePtr.IsValid()) {
		FSlateStyleRegistry::UnRegisterSlateStyle(*StylePtr);
		StylePtr.Reset();
	}
}

const ISlateStyle& MyCoreStyle::Get()
{
	if (!StylePtr.IsValid()) {
		Initialize();
	}

	return *StylePtr;
}

