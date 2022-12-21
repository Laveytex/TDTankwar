﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "GameFramework/Actor.h"
#include "Cannon.generated.h"

class UArrowComponent;

UCLASS()
class TDTANKWAR_API ACannon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACannon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ProjectileSpawnPont;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	float FireRate=1;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	float FireRange = 1000;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "FireParams")
	ECannonType Type = ECannonType::FireProjectile;

	FTimerHandle ReloadTimerHandle;

	bool ReadyToFire = false;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Fire();

	bool IsReadyToFire();

	void Reload();
};
