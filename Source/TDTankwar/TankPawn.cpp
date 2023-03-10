// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPlayerController.h"
#include "Components/ArrowComponent.h"


DECLARE_LOG_CATEGORY_EXTERN(TankLog, All, All);
DEFINE_LOG_CATEGORY(TankLog);


// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TankBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank Body"));
	//TankBody->SetMaterial(0, TankMaterial);
	RootComponent = TankBody;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret"));
	TurretMesh->SetupAttachment(TankBody);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(TankBody);
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->OnDie.AddUObject(this, &ATankPawn::Die);
	HealthComponent->OnDamage.AddUObject(this, &ATankPawn::DamageTaked);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(TankBody);

	//TankMaterial->SetVectorParameterValueEditorOnly(FName ("Color"), FVector(1,0.43,0));
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	TankController = Cast<ATankPlayerController>(GetController());
	
	SetupCannon(CannonClass);
	CreateDynamicMaterial();
	
}

void ATankPawn::Die()
{
	Destroy();
}

void ATankPawn::DamageTaked(float DamageValue)
{
	UE_LOG(TankLog, Warning,
		TEXT("Tank %s taked damage: %f Health: %f"), *GetName(), DamageValue, HealthComponent->GetHeath());
}

void ATankPawn::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

TArray<FVector> ATankPawn::GetPatrollingPoints()
{
	TArray<FVector> points;
	for (ATargetPoint* point: PatrollingPoints)
	{
		points.Add(point->GetActorLocation());
	}

	return points;
}

void ATankPawn::SetPatrollingPoints(TArray<ATargetPoint*> NewPatrollingPoints)
{
	PatrollingPoints = NewPatrollingPoints;
}

FVector ATankPawn::GetTurretForwardVector()
{
	return TurretMesh->GetForwardVector();
}

void ATankPawn::SetupCannon(TSubclassOf<ACannon> CannonClassGet)
{
	if(Cannon)
	{
		Cannon->Destroy();
	}
	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClassGet, params);
	Cannon->AttachToComponent(CannonSetupPoint,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ATankPawn::Fire()
{
	if(Cannon)
	{
		Cannon->Fire();
	}
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Tank Movement
	FVector currentLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector NewPosition = currentLocation + ForwardVector*MovementSpeed*TargetForwardAxisValue*DeltaTime;
	SetActorLocation(NewPosition, true);

	// Tank Rotation
	CurrentRightAxisValue = FMath::Lerp(CurrentRightAxisValue, TargetRightAxisValue, InterpolationKey);
	//UE_LOG(TankLog, Warning, TEXT("CurrentRightAxisValue = %f TargetRightAxisValue = %f"), CurrentRightAxisValue, TargetRightAxisValue);
	float YawRotation = RotationSpeed*DeltaTime*CurrentRightAxisValue;
	FRotator currentRotation = GetActorRotation();
	YawRotation = currentRotation.Yaw + YawRotation;
	FRotator newRotate = FRotator( 0, YawRotation, 0);
	SetActorRotation(newRotate);

	// TurrenRotation
	if(TankController)
	{
		FVector MousePos = TankController->GetMousePos();
		RotateTurretTo(MousePos);
	}
}

void ATankPawn::RotateTurretTo(FVector TargetPosition)
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),TargetPosition);
	FRotator currRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = currRotation.Pitch;
	targetRotation.Roll = currRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(currRotation,targetRotation,TurretRotationInterpolationKey));
}

FVector ATankPawn::GetEyesPosition()
{
	return CannonSetupPoint->GetComponentLocation();
}

void ATankPawn::CreateDynamicMaterial()
{
	//Tank Color
	TankBobyMaterialInstance = TankBody->GetMaterial(0);
	TankBobyDynamicMaterials = UMaterialInstanceDynamic::Create(TankBobyMaterialInstance, this);
	TankBody->SetMaterial(0,TankBobyDynamicMaterials);
	TurretMesh->SetMaterial(0,TankBobyDynamicMaterials);
	if(TankController)
	{
		TankBobyDynamicMaterials->SetVectorParameterValue(FName ("Color"), FVector(0.1,0.1,0.1));
	}
	else
	{
		TankBobyDynamicMaterials->SetVectorParameterValue(FName ("Color"), FVector(1,0.44,0));
	}
	
	//Wheel Color
	TankWheelMaterialInstance = TankBody->GetMaterial(1);
	TankwheelDynamicMaterials = UMaterialInstanceDynamic::Create(TankWheelMaterialInstance, this);
	TankBody->SetMaterial(1,TankwheelDynamicMaterials);
	TankwheelDynamicMaterials->SetScalarParameterValue(FName ("TextureOffset"), TextureOffset);
	
}

void ATankPawn::WheelTextureOffset()
{
	TextureOffset = TextureOffset - 0.01;
	TankwheelDynamicMaterials->SetScalarParameterValue(FName ("TextureOffset"), TextureOffset);
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATankPawn::MoveForward(float Value)
{
	TargetForwardAxisValue = Value;
	if(Value>0)
	WheelTextureOffset();
}

void ATankPawn::RotateRight(float Value)
{
	TargetRightAxisValue = Value;
}

