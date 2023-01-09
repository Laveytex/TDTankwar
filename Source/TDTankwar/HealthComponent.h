// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TDTANKWAR_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_EVENT(UHealthComponent, FOnDie)
	DECLARE_EVENT_OneParam(UHealthComponent,FOnHealthChanged, float) 

public:
	// Sets default values for this component's properties
	UHealthComponent();

	FOnDie OnDie;
	FOnHealthChanged OnDamage;

	void TakeDamage(FDamageData DamageData);

	float GetHeath() const;

	float GetHealthState() const;

	void AddHealth(float AddiditionalHeathValue);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health values")
	float MaxHealth = 10;

	UPROPERTY()
	float CurrentHealth;

public:
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	//                           FActorComponentTickFunction* ThisTickFunction) override;
};
