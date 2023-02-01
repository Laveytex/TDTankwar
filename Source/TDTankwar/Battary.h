// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageTaker.h"
#include "HealthComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MapLoader.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Battary.generated.h"

UCLASS()
class TDTANKWAR_API ABattary : public AActor, public IDamageTaker
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BuildingMeshComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* HitCollider;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* ExplosionEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* AudioEffect;
	


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level transit params")
	AMapLoader* LikedMapLoader;
	
public:
	// Sets default values for this actor's properties
	ABattary();
	void TakeDamage(FDamageData DamageData);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void Die();

	UFUNCTION()
	void DamageTaked(float DamageValue);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
