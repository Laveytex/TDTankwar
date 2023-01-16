// Fill out your copyright notice in the Description page of Project Settings.


#include "TankFactory.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATankFactory::ATankFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceneComponent;
	
	BuildingMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BuildingMesh"));
	BuildingMeshComponent->SetupAttachment(sceneComponent);

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon spawn point"));
	TankSpawnPoint->AttachToComponent(sceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(sceneComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddUObject(this, &ATankFactory::Die);
	HealthComponent->OnDamage.AddUObject(this, &ATankFactory::DamageTaked);

	GateAnimOpen = CreateDefaultSubobject<UAnimationAsset>(TEXT("GateAnim"));
}

// Called when the game starts or when spawned
void ATankFactory::BeginPlay()
{
	Super::BeginPlay();

	if(LikedMapLoader)
		LikedMapLoader->SetIsActivated(false);

	FTimerHandle _targetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(_targetingTimerHandle, this,
		&ATankFactory::SpawnNewTank, SpawnTankRate, true, SpawnTankRate);
}

// Called every frame
void ATankFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATankFactory::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ATankFactory::SpawnNewTank()
{
	UE_LOG(LogTemp, Warning, TEXT("TankSpawn"));
	
	FTransform spawnTransform(TankSpawnPoint->GetComponentRotation(),
		TankSpawnPoint->GetComponentLocation(), FVector(1));
	ATankPawn* newTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTankClass, spawnTransform,
		this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	newTank->SetPatrollingPoints(TankWayPoint);
	UGameplayStatics::FinishSpawningActor(newTank, spawnTransform);
	BuildingMeshComponent->PlayAnimation(GateAnimOpen, false);
}

void ATankFactory::Die()
{
	if(LikedMapLoader)
		LikedMapLoader->SetIsActivated(true);
	Destroy();
}

void ATankFactory::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Factory %s, taked damage:%f, Heath:%f"),
		*GetName(), DamageValue, HealthComponent->GetHeath());
}

