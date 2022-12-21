// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"

#include "Components/ArrowComponent.h"


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
	if( Type==ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(10,1,FColor::Green,"Fire - Projectile");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(10,1,FColor::Green,"Fire - Trace");
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

