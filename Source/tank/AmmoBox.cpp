// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBox.h"
#include <Components/StaticMeshComponent.h>
#include <Components/PrimitiveComponent.h>
#include "TankPawn.h"
#include "tank.h"

//
AAmmoBox::AAmmoBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnMeshOverlapBegin);
	Mesh->SetCollisionProfileName(FName("OverlapOnlyPawn"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetGenerateOverlapEvents(true); 

}
//
void AAmmoBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* PlayerPawn = Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (OtherActor == PlayerPawn)
	{
		PlayerPawn->SetupCannon(CannonClass);
		//UE_LOG(LogTank, Verbose, TEXT("CannonClass: %s"), *CannonClass->GetName());
		Destroy();
	}
}



