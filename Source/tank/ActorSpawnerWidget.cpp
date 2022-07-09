// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorSpawnerWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TankController.h"
#include "Components/TextBlock.h"
#include <CollisionQueryParams.h>
#include <Engine/World.h>
#include <GameFramework/Actor.h>
#include <Math/Quat.h>

void UActorSpawnerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PLayerController = Cast<ATankController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PLayerController->OnMouseButtonUp.AddUObject(this, &ThisClass::OnMouseButtonUp);
}

void UActorSpawnerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (SpawnedActor && PLayerController) {
		PLayerController->DeprojectMousePositionToWorld(WorldMousePostion, WorldMouseDirection);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(SpawnedActor);
		Params.AddIgnoredActor(PLayerController->GetPawn());



		constexpr float dist = 100000;
		GetWorld()->LineTraceSingleByChannel(OutHit, WorldMousePostion, WorldMousePostion + WorldMouseDirection * dist, ECollisionChannel::ECC_WorldStatic, Params);
		if (OutHit.bBlockingHit) {
			SpawnedActor->SetActorLocation(OutHit.Location);
		}
	}
}

void UActorSpawnerWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (DraggedText) {
		DraggedText->SetText(DraggedName); 
	}
}

FReply UActorSpawnerWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{	
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) {
		PLayerController->DeprojectMousePositionToWorld(WorldMousePostion, WorldMouseDirection);


		SpawnedActor = GetWorld()->SpawnActorDeferred<ABasicClass>(SpawnerClass, SpawnTransform, PLayerController->GetPawn(), nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		//SpawnedActor = GetWorld()->SpawnActor<APawn>(SpawnerClass);
	}

	return FReply::Handled();
}

void UActorSpawnerWidget::OnMouseButtonUp()
{
	if (SpawnedActor) {
		SpawnedActor->FinishSpawning(SpawnTransform);
		SpawnedActor->SetFriendness(true);
		SpawnedActor = nullptr;
	}
}
