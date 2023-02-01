// Fill out your copyright notice in the Description page of Project Settings.


#include "MapLoader.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Polys.h"
#include "Kismet/GameplayStatics.h"

// Sets default valuesв
AMapLoader::AMapLoader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	USceneComponent* sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceneComponent;

	GateMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GateMesh"));
	GateMesh->SetupAttachment(sceneComponent);

	GateBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateBodyMesh"));
	GateBody->SetupAttachment(sceneComponent);

	TrafficLight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrafficLightMesh"));
	TrafficLight->SetupAttachment(sceneComponent);

	GateAnimOpen = CreateDefaultSubobject<UAnimationAsset>(TEXT("GateAnim"));
	GateAnimClose = CreateDefaultSubobject<UAnimationAsset>(TEXT("GateAnim"));

	ActivateLight=CreateDefaultSubobject<UPointLightComponent>(TEXT("Activated light"));
	ActivateLight->SetupAttachment(sceneComponent);

	DeadctivateLight=CreateDefaultSubobject<UPointLightComponent>(TEXT("Deactivate light"));
	DeadctivateLight->SetupAttachment(sceneComponent);

	TriggerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigget collider"));
	TriggerCollider->SetupAttachment(sceneComponent);
	TriggerCollider->OnComponentBeginOverlap.AddDynamic(this, &AMapLoader::OnTriggetOverlapBegin);

	SetActivateLight();

}

void AMapLoader::SetIsActivated(bool NewIsActivated)
{
	IsActivate = NewIsActivated;

	SetActivateLight();
	
}

void AMapLoader::SetActivateLight()
{
	ActivateLight->SetHiddenInGame(!IsActivate);
	DeadctivateLight->SetHiddenInGame(IsActivate);
	if (IsActivate)
	{
		GateMesh->PlayAnimation(GateAnimOpen, false);
	}
	else
	{
		GateMesh->PlayAnimation(GateAnimClose, false);
	}
	
	
}

void AMapLoader::OnTriggetOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* otherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!IsActivate)
		return;
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if(otherActor == PlayerPawn)
	{
		UGameplayStatics::OpenLevel(GetWorld(), LoadLevelName);
	}
}

// Called when the game starts or when spawned
void AMapLoader::BeginPlay()
{
	Super::BeginPlay();
	SetActivateLight();
}

