// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cannon.h"
#include "DamageTaker.h"
#include "HealthComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class ATankPlayerController;
class ACannon;

UCLASS()
class TDTANKWAR_API ATankPawn : public APawn, public  IDamageTaker
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Component)
	UStaticMeshComponent* TankBody;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Component)
	UStaticMeshComponent* Turret;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Component)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Component)
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	float MovementSpeed = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	float RotationSpeed = 100;
	
	float TargetForwardAxisValue;
	float TargetRightAxisValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	float InterpolationKey = 0.1f;
	float CurrentRightAxisValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret/Speed")
	float TurretRotationInterpolationKey=0.5f;

	UPROPERTY()
	ATankPlayerController* TankController;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Component)
	UArrowComponent* CannonSetupPoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret/Speed")
	TSubclassOf<ACannon> CannonClass;


	UPROPERTY()
	ACannon*Cannon;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* HitCollider;
	
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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	void MoveForward(float Value);
	UFUNCTION()
	void RotateRight(float Value);

	UFUNCTION()
	void Fire();

	void SetupCannon(TSubclassOf<ACannon> CannonClassGet);

	UFUNCTION()
	void TakeDamage(FDamageData DamageData);
	

};
