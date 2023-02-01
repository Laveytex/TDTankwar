// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/Actor.h"

#include "MapLoader.generated.h"


UCLASS()
class TDTANKWAR_API AMapLoader : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* GateMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* GateBody;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TrafficLight;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* TriggerCollider;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UPointLightComponent* ActivateLight;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UPointLightComponent* DeadctivateLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UAnimationAsset* GateAnimOpen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UAnimationAsset* GateAnimClose;
	


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level loading params");
	FName LoadLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level loading params");
	bool IsActivate = false;
	
public:	
	// Sets default values for this actor's properties
	AMapLoader();

	void SetIsActivated(bool NewIsActivated);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetActivateLight();

	UFUNCTION()
	void OnTriggetOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* otherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult&SweepResult);

public:	

};
