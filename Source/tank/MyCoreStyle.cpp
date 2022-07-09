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

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush(Style->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

TSharedPtr<ISlateStyle>MyCoreStyle::StylePtr;
int MyCoreStyle::CurrentStyle;

TSharedPtr<ISlateStyle>MyCoreStyle::FindStyle(int InStyle) {


	FString Path = FPaths::ProjectContentDir() / TEXT("/UI");
	TSharedPtr<FSlateStyleSet> Style = FSlateGameResources::New(FName("MyCoreStyle"), Path, Path);

	if (!InStyle)
	{
		Style->Set("Background", FLinearColor::Yellow);
		Style->Set("PlayerImage", new IMAGE_BRUSH("OrdinaryPlayerImage", FVector2D(64.f)));
		Style->Set("BordersImage", new IMAGE_BRUSH("OrdinaryBordersImage", FVector2D(256.f)));
	}
	else
	{
		Style->Set("Background", FLinearColor::Gray);
		Style->Set("PlayerImage", new IMAGE_BRUSH("BlackWhitePlayerImage", FVector2D(64.f)));
		Style->Set("BordersImage", new IMAGE_BRUSH("BlackWhiteBordersImage", FVector2D(256.f)));
	}
	return Style;
}

#undef IMAGE_BRUSH

void MyCoreStyle::Initialize(int InStyle)
{
	ShutDown();
	StylePtr = FindStyle(InStyle);
	FSlateStyleRegistry::RegisterSlateStyle(*StylePtr);
}

void MyCoreStyle::ShutDown()
{
	if (StylePtr.IsValid()) {
		FSlateStyleRegistry::UnRegisterSlateStyle(*StylePtr);
		StylePtr.Reset();
	}
}

void MyCoreStyle::ChangeStyle(int InStyle)
{
	CurrentStyle = InStyle;
	Initialize(InStyle);
}

const ISlateStyle& MyCoreStyle::Get()
{
	if (!StylePtr.IsValid()) {
		Initialize(CurrentStyle);
	}

	return *StylePtr;
}

