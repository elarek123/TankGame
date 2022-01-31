// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "tank.h"
#include "ActorPoolSubsystem.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.005;


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->OnComponentHit.AddDynamic(this, &AProjectile::OnMeshHit);
	RootComponent = Mesh;
}

void AProjectile::Start()
{
	StartPosition = GetActorLocation();
}

// Called when the game starts or when spawned


// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * DeltaTime;
	SetActorLocation(NextPosition, true); 
	
	if (FVector::Dist(GetActorLocation(), StartPosition) > FireRange) {
		UActorPoolSubsystem* Pool = GetWorld()->GetSubsystem<UActorPoolSubsystem>();
		Pool->ReturnActor(this);
	}
}

void AProjectile::OnMeshHit(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	UE_LOG(LogTank, Warning, TEXT("Projectile %s collided with %s"), *GetName(), *OtherActor->GetName());

	if (OtherActor && OtherComp && OtherComp->GetCollisionObjectType() == ECC_Destructible)
		OtherActor->Destroy();
	UActorPoolSubsystem* Pool = GetWorld()->GetSubsystem<UActorPoolSubsystem>();
	Pool->ReturnActor(this);
}

