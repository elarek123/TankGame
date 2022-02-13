// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicClass.h"
#include "tank.h"

// Sets default values
ABasicClass::ABasicClass()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	RootComponent = BodyMesh;

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(BodyMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->OnHealthChanged.AddDynamic(this, &ABasicClass::OnHealthChanged);
	HealthComponent->OnDie.AddDynamic(this, &ABasicClass::OnDie);

}



void ABasicClass::OnHealthChanged_Implementation(float Damage)
{
	UE_LOG(LogTank, Log, TEXT("%s has taken damage: %f"), *GetName(), Damage);
}

void ABasicClass::OnDie_Implementation()
{
	Destroy();

}

void ABasicClass::TakeDamage(const FDamageData& DamageData)
{
	HealthComponent->TakeDamage(DamageData);

}




