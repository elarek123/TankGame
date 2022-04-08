// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "MyCoreStyle.h"
#include "ElarekWWidgetStyle.h"
#include <Components/ButtonSlot.h>


void UMainMenu::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (OptionButton) {
		OptionButton->WidgetStyle = MyCoreStyle::Get().GetWidgetStyle<FElarekWStyle>("WS_RadioButton");
	}

	if (NewGameButton)
	{
		NewGameButton->ColorAndOpacity = MyCoreStyle::Get().GetColor("Red");
	}

	if (FrogImage)
	{
		FrogImage->SetBrush(*MyCoreStyle::Get().GetBrush("frog"));
		Cast<UButtonSlot>(FrogImage->Slot)->SetPadding(MyCoreStyle::Get().GetMargin("DefaultPadding"));
	}


}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (NewGameButton)
	{
		NewGameButton->OnClicked.AddDynamic(this, &UMainMenu::OnNewGameClicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenu::OnQuitClicked);
	}
}

void UMainMenu::OnNewGameClicked()
{
	if (InvisAnimation)
	{
		DelayDuration = InvisAnimation->GetEndTime();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMainMenu::LoadingLevel, DelayDuration, false);
		PlayAnimation(InvisAnimation);
		
	}


}

void UMainMenu::OnQuitClicked()
{
	GEngine->Exec(GetWorld(), TEXT("Exit"));
}

void UMainMenu::LoadingLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), LoadLevelName);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	RemoveFromParent();

}
