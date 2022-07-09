// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicClass.h"
#include "tank.h"
#include <Particles/ParticleSystemComponent.h>
#include <Components/AudioComponent.h>
#include <TimerManager.h>
#include "ActorPoolSubsystem.h"
#include <UObject/NoExportTypes.h>
#include "HealthComponent.h"
#include <GameFramework/Actor.h>
#include <Components/ArrowComponent.h>
#include "FrenPawn.h"
#include "MenuHud.h"

// Sets default values
ABasicClass::ABasicClass()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	RootComponent = BodyMesh;

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(BodyMesh);
	AmmoBoxSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Ammo box spawn point"));
	AmmoBoxSpawnPoint->SetupAttachment(BodyMesh);
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->OnHealthChanged.AddDynamic(this, &ABasicClass::OnHealthChanged);
	HealthComponent->OnDie.AddDynamic(this, &ABasicClass::OnDie);

	DestroyEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Destroy Effect"));
	DestroyEffect->SetupAttachment(BodyMesh);

	AudioDestroyEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Effect"));
	AudioDestroyEffect->SetupAttachment(BodyMesh);
	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(BodyMesh);

	
	
	//RootComponent = Widget;
}






void ABasicClass::OnHealthChanged_Implementation(float Damage)
{
	//UE_LOG(LogTank, Log, TEXT("%s has taken damage: %f"), *GetName(), Damage);

}
void ABasicClass::OnDie_Implementation()
{
	if (bIsDead)
		return;
	bIsDead = true;
	DestroyEffect->Activate();
	AudioDestroyEffect->Play();


	UActorPoolSubsystem* Pool = GetWorld()->GetSubsystem<UActorPoolSubsystem>();
	FTransform SpawnTransform(AmmoBoxSpawnPoint->GetComponentRotation(), AmmoBoxSpawnPoint->GetComponentLocation(), FVector::OneVector);
	AAmmoBox* Ammo = Cast<AAmmoBox>(Pool->RetreiveActor(Ammo_Box, SpawnTransform));

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ABasicClass::Destroying, DestroyRate, false);
}




void ABasicClass::Destroying()
{
	for (auto i : CannonPack)
		i->Destroy();
	Destroy();
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);

}

void ABasicClass::BeginPlay()
{
	Super::BeginPlay();
	bIsPlayer = GetController() == GetWorld()->GetFirstPlayerController();
	Health = Cast<UHealthBar>(HealthBar->GetWidget());

	if(Health)
		Health->SetHealthComponent(HealthComponent);

	SetFriendness(GetWorld()->GetFirstPlayerController()->GetPawn() == this);
	if (bIsPlayer)
	{
		TanksHud = Cast<AMenuHud>(GetWorld()->GetFirstPlayerController()->GetHUD());
	}

	FrenPawn::AddPawn(this);
}


void ABasicClass::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	FrenPawn::DelPawn(this);
	if (bIsPlayer) {
		TanksHud->ShowWindow(EWidgetID::GameOver);
	}
}

void ABasicClass::TakeDamage(const FDamageData& DamageData)
{
	HealthComponent->TakeDamage(DamageData);

}




