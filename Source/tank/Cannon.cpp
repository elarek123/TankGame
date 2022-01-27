// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include "tank.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}

void ACannon::Fire()
{ 
	if (!bIsReadyToFire)
		return;
	bulletcnt--;
	bIsReadyToFire = false;
	UE_LOG(LogTank, Verbose, TEXT("Bullet: %d"), bulletcnt);
	if (Type == ECannonType::FireProjectile) {
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2, FColor::Green, TEXT("Fire projectile"));
	}
	else if (Type == ECannonType::FireTrace) {
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2, FColor::Green, TEXT("Fire trace"));
	}
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
}



bool ACannon::IsReadyToFire()
{
	return bIsReadyToFire;
}

void ACannon::SpecialFire()
{
	if (!bIsReadyToFire) {
		bulletcnt--;
		SpecialFireLimit = 3;
		Reload();
		return;
	}
	SpecialFireLimit--;
	if (SpecialFireLimit == 0)
		bIsReadyToFire = false;
	UE_LOG(LogTank, Verbose, TEXT("Bullet: %d"), SpecialFireLimit);
	if (Type == ECannonType::FireProjectile) {
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2, FColor::Green, TEXT("Fire projectile"));
	}
	else if (Type == ECannonType::FireTrace) {
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2, FColor::Green, TEXT("Fire trace"));
	}
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::SpecialFire, 1 / FireRate);
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
	bIsReadyToFire = true;
}

void ACannon::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
}

void ACannon::Reload()
{	
	if(bulletcnt > 0)
		bIsReadyToFire = true;
}


