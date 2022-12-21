// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameStructs.generated.h"

UENUM(BlueprintType)
enum class ECannonType : uint8
{
	FireProjectile = 0 UMETA(DisplayName = "Use projectile"),
	FireTrace = 1 UMETA(DisplayName = "UseTracr")
};


UCLASS()
class TDTANKWAR_API UGameStructs : public UObject
{
	GENERATED_BODY()
};
