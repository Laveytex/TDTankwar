// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"

#include "DrawDebugHelpers.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ACannon::ACannon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComp;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPont = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPont->SetupAttachment(Mesh);

	ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Shoot Effect"));
	ShootEffect->SetupAttachment(ProjectileSpawnPont);

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioEffect"));

	
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACannon::Fire()
{
	if(!ReadyToFire)
	{
		return;
	}
	ReadyToFire = false;

	ShootEffect->ActivateSystem();
	AudioEffect->Play();

	if(GetOwner() && GetOwner()==GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if(ShootForceEffect)
		{
			FForceFeedbackParameters shootForceEffectParams;
			shootForceEffectParams.bLooping = false;
			shootForceEffectParams.Tag = "shootForceEffectParams";
			GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(ShootForceEffect, shootForceEffectParams);
		}

		if(ShootShake)
		{
			GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(ShootShake);
		}
	}
	
	if(Type==ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(10,1,FColor::Green,"Fire - Projectile");
		
		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,
			ProjectileSpawnPont->GetComponentLocation(),
			ProjectileSpawnPont->GetComponentRotation());
		if(projectile)
		{
			projectile->Start();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(10,1,FColor::Green,"Fire - Trace");

		FHitResult hitResult;
		FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")),true, this);
		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = false;

		FVector start = ProjectileSpawnPont->GetComponentLocation();
		FVector end = ProjectileSpawnPont->GetForwardVector()*FireRange+start;
		if(GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, TraceParams))
		{
			DrawDebugLine(GetWorld(), start, hitResult.Location, FColor::Red,false,0.5f,0,5);
			if(hitResult.Actor.Get())
			{
				hitResult.Actor.Get()->Destroy();
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.5f,0,5);
		}
	}
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1/FireRate, false);
}

bool ACannon::IsReadyToFire()
{
	return ReadyToFire;
}

void ACannon::Reload()
{
	ReadyToFire = true;
}

