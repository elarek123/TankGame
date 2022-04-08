// Fill out your copyright notice in the Description page of Project Settings.


#include "TankFactory.h"
#include "tank.h"
#include "TankPawn.h"
#include <Particles/ParticleSystemComponent.h>
#include <GameFramework/Actor.h>

// Sets default values
ATankFactory::ATankFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComp;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Mesh"));
	BuildingMesh->SetupAttachment(SceneComp);

	DestroyedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Destroyed Mesh"));
	DestroyedMesh->SetupAttachment(SceneComp);

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	TankSpawnPoint->SetupAttachment(SceneComp);

	SpawnTankEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Spawn Tank Effect"));
	SpawnTankEffect->SetupAttachment(TankSpawnPoint);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(SceneComp);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddDynamic(this, &ATankFactory::Die);
	HealthComponent->OnHealthChanged.AddDynamic(this, &ATankFactory::DamageTaked);

}

// Called when the game starts or when spawned
void ATankFactory::BeginPlay()
{
	Super::BeginPlay();
	DestroyedMesh->SetHiddenInGame(true);
	GetWorld()->GetTimerManager().SetTimer(SpawnTankTimerHandle, this, &ATankFactory::SpawnEffect, SpawnTankRate, true, SpawnTankRate);

	
}

void ATankFactory::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame


void ATankFactory::TakeDamage(const FDamageData& DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ATankFactory::SpawnEffect()
{	
	if (MaxTankNumber-- > 0) {
		SpawnTankEffect->Activate();
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ATankFactory::SpawnNewTank, SpawnEffectRate, false);
	}
	else
		return;
}

void ATankFactory::SpawnNewTank()
{
	SpawnTankEffect->Deactivate();
	FTransform SpawnTransform(TankSpawnPoint->GetComponentRotation(), TankSpawnPoint->GetComponentLocation(), FVector(1));
	ATankPawn* NewTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTankClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	//
	NewTank->SetPatrollingPoints(TankWayPoints);
	//
	NewTank->FinishSpawning(SpawnTransform);

}

void ATankFactory::Die()
{	
	if(MapLoader)
		MapLoader->SetIsActivated(true);
	BuildingMesh->SetHiddenInGame(true);
	DestroyedMesh->SetHiddenInGame(false);
	HitCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BuildingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorld()->GetTimerManager().ClearTimer(SpawnTankTimerHandle);
	MaxTankNumber = 0;
}

void ATankFactory::DamageTaked(float Damage)
{
	UE_LOG(LogTank, Log, TEXT("%s has taken damage: %f"), *GetName(), Damage);
}

