// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsProjectile.h"
#include "Projectile.h"
#include "Damageable.h"
#include "GameStructs.h"



APhysicsProjectile::APhysicsProjectile()
{
	MoveComponent = CreateDefaultSubobject<UPhysicsMovementComponent>(TEXT("Movement Component"));
}

void APhysicsProjectile::Start()
{
	Super::Start();

	MoveComponent->Velocity = GetActorForwardVector() * MoveSpeed;
	MoveComponent->SetComponentTickEnabled(true);
}

void APhysicsProjectile::Stop()
{
	MoveComponent->Velocity = FVector::ZeroVector;
	MoveComponent->SetComponentTickEnabled(false);
	Super::Stop();
}

void APhysicsProjectile::Tick(float DeltaTime)
{
	if (GetActorLocation().Z < -1000)
		Stop();
}

void APhysicsProjectile::OnMeshHit(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	Super::OnMeshHit(OverlappedComp, OtherActor, OtherComp, NormalImpulse, HitResult);

	FVector StartPos = GetActorLocation();
	FVector EndPos = StartPos + FVector(0.1f);

	FCollisionShape Shape = FCollisionShape::MakeSphere(ExplodeRadius);
	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.AddIgnoredActor(this);
	Params.bTraceComplex = true;
	Params.TraceTag = "Explode Trace";
	TArray<FHitResult> AttackHit;

	FQuat Rotation = FQuat::Identity;

	bool bSweepResult = GetWorld()->SweepMultiByChannel
	(
		AttackHit,
		StartPos,
		EndPos,
		Rotation,
		ECollisionChannel::ECC_Visibility,
		Shape,
		Params
	);

	GetWorld()->DebugDrawTraceTag = "Explode Trace";

	if (bSweepResult)
	{
		for (FHitResult HitResult : AttackHit)
		{
			AActor* HitActor = HitResult.GetActor();
			if (!HitActor)
				continue;

			IDamageable* DamageTakerActor = Cast<IDamageable>(HitActor);
			if (DamageTakerActor && HitActor != OtherActor)
			{
				FDamageData DamageData;
				DamageData.DamageValue = Damage;
				DamageData.Instigator = GetOwner();
				DamageData.DamageMaker = this;

				DamageTakerActor->TakeDamage(DamageData);
			}
			else
			{
				UPrimitiveComponent* HitMesh = Cast<UPrimitiveComponent>(HitActor->GetRootComponent());
				if (HitMesh)
				{
					if (HitMesh->IsSimulatingPhysics())
					{
						FVector ForceVector = HitActor->GetActorLocation() - GetActorLocation();
						ForceVector.Normalize();
						HitMesh->AddImpulse(ForceVector * PushForce, NAME_None, true);
					}
				}
			}

		}
	}
}
