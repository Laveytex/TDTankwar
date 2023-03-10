// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SkeletalMeshComponent.h"
#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "MapLoader.h"
#include "TankPawn.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Actor.h"
#include "TankFactory.generated.h"


UCLASS()
class TDTANKWAR_API ATankFactory : public AActor, public IDamageTaker
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* BuildingMeshComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* TankSpawnPoint;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* HitCollider;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UAnimationAsset* GateAnimOpen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	TSubclassOf<ATankPawn> SpawnTankClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn tanks params")
	float SpawnTankRate = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	TArray<ATargetPoint*> TankWayPoint; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tank params")
	AMapLoader* LikedMapLoader;
	
public:	
	// Sets default values for this actor's properties
	ATankFactory();
	void TakeDamage(FDamageData DamageData);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnNewTank();

	UFUNCTION()
	void Die();

	UFUNCTION()
	void DamageTaked(float DamageValue);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
