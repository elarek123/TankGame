// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include "tank.h"
#include "Projectile.h"
#include <DrawDebugHelpers.h>
#include "ActorPoolSubsystem.h"
#include "Damageable.h"

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
	AmmoCount--;
	bIsReadyToFire = false;
	//UE_LOG(LogTank, Verbose, TEXT("Bullet: %d"), bulletcnt);
	if (Type == ECannonType::FireProjectile) {
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2, FColor::Green, TEXT("Fire projectile"));
		UActorPoolSubsystem* Pool = GetWorld()->GetSubsystem<UActorPoolSubsystem>();
		FTransform SpawnTransform(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation(), FVector::OneVector);
		AProjectile* Projectile = Cast<AProjectile>(Pool->RetreiveActor(ProjectileClass, SpawnTransform));
		if (Projectile)
			Projectile->Start();
	}
	else if (Type == ECannonType::FireTrace) {
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2, FColor::Green, TEXT("Fire trace"));
		FHitResult HitResult;
		FVector TraceStart = ProjectileSpawnPoint->GetComponentLocation();
		FVector TraceEnd = ProjectileSpawnPoint->GetComponentLocation() + ProjectileSpawnPoint->GetForwardVector() * FireRange;
		FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
		TraceParams.bReturnPhysicalMaterial = false;

		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams)) {
			DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Green, false, 0.5, 0, 5);
			if (HitResult.Actor.IsValid() && HitResult.Component.IsValid(), HitResult.Component->GetCollisionObjectType() == ECC_Destructible)
				HitResult.Actor->Destroy();
			else if (IDamageable* Damageable = Cast<IDamageable>(HitResult.Actor)) {
				FDamageData DamageData;
				DamageData.DamageValue = TraceDamage;
				DamageData.Instigator = GetInstigator();
				DamageData.DamageMaker = this;
				Damageable->TakeDamage(DamageData);
			}
		}
		else
			DrawDebugLine(GetWorld(), TraceStart, HitResult.TraceEnd, FColor::Green, false, 0.5, 0, 5);

	}
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, FireRate, false);
}



bool ACannon::IsReadyToFire()
{
	return bIsReadyToFire;
}

void ACannon::SpecialFire()
{
	UE_LOG(LogTank, VeryVerbose, TEXT("Hello pidor"))
	if (!bIsReadyToFire) {
		SpecialFireLimit = 3;
		Reload();
		return;
	}
	AmmoCount--;
	SpecialFireLimit--;
	if (SpecialFireLimit == 0)
		bIsReadyToFire = false;
	//UE_LOG(LogTank, Verbose, TEXT("Bullet: %d"), SpecialFireLimit);
	if (Type == ECannonType::FireProjectile) { 
		
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2, FColor::Green, TEXT("Fire projectile"));
		UActorPoolSubsystem* Pool = GetWorld()->GetSubsystem<UActorPoolSubsystem>();
		FTransform SpawnTransform(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation(), FVector::OneVector);
		AProjectile* Projectile = Cast<AProjectile>(Pool->RetreiveActor(ProjectileClass, SpawnTransform));
		if (Projectile)
			Projectile->Start();
	}
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::SpecialFire, SpecialFireRate, false);
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
	bIsReadyToFire = true;
}

void ACannon::SetVisibility(bool bIsVisible)
{
	Mesh->SetHiddenInGame(!bIsVisible);
}

int ACannon::GetAmmoParam(bool factor)
{
	if (factor)
		return AmmoCount;
	else
		return AmmoLimit;
}

void ACannon::ReplenishAmmo()
{
	AmmoCount += AmmoBaseNumber;
}

ECannonType ACannon::GetType()
{
	return Type;
}

