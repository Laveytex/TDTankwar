// Fill out your copyright notice in the Description page of Project Settings.


#include "MapLoader.h"

#include <solver/PxSolverDefs.h>

#include "Engine/Polys.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMapLoader::AMapLoader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	USceneComponent* scenComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = scenComp;

	GateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateMesh"));
	GateMesh->SetupAttachment(scenComp);

	ActivateLight=CreateDefaultSubobject<UPointLightComponent>(TEXT("Activated light"));
	ActivateLight->SetupAttachment(scenComp);

	DeadctivateLight=CreateDefaultSubobject<UPointLightComponent>(TEXT("Deactivate light"));
	DeadctivateLight->SetupAttachment(scenComp);

	TriggerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigget collider"));
	TriggerCollider->SetupAttachment(scenComp);
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

