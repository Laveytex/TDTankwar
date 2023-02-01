// Fill out your copyright notice in the Description page of Project Settings.


#include "Battary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
ABattary::ABattary()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceneComponent;
	
	BuildingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	BuildingMeshComponent->SetupAttachment(sceneComponent);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(sceneComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddUObject(this, &ABattary::Die);
	HealthComponent->OnDamage.AddUObject(this, &ABattary::DamageTaked);

	ExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Exposion"));
	ExplosionEffect->SetupAttachment(sceneComponent);

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioEffect"));
}

void ABattary::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ABattary::Die()
{
	if(LikedMapLoader)
		LikedMapLoader->SetIsActivated(true);
	ExplosionEffect->ActivateSystem();
	AudioEffect->Play();
}

void ABattary::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Factory %s, taked damage:%f, Heath:%f"),
		*GetName(), DamageValue, HealthComponent->GetHeath());
}

// Called when the game starts or when spawned
void ABattary::BeginPlay()
{
	Super::BeginPlay();
	
	if(LikedMapLoader)
		LikedMapLoader->SetIsActivated(false);
}

// Called every frame
void ABattary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

