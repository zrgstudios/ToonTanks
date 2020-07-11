// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	ProjectileMovement->InitialSpeed = MovementSpeed;
	ProjectileMovement->MaxSpeed = MovementSpeed;

	InitialLifeSpan = 3.0f;

	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Trail"));
	ParticleTrail->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	}
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();

	if (!MyOwner)
	{
		return;
	}

	if (OtherActor != nullptr && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, DamageType);
	
		if (HitParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation(), FRotator::ZeroRotator);
		}
		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		}
		if (HitShake)
		{
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(HitShake, 1);
		}
		Destroy();
	}
}
