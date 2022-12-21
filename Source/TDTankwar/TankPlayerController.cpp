// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "DrawDebugHelpers.h"



ATankPlayerController::ATankPlayerController()
{
	bShowMouseCursor = true;
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATankPlayerController::RotateRight);
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	//Super::Tick(DeltaSeconds);
	FVector MouseDirection;
	DeprojectMousePositionToWorld(MousePos, MouseDirection);
	FVector PawnPos = TankPawn->GetActorLocation();
	MousePos.Z = PawnPos.Z;
	FVector dir = MousePos - PawnPos;
	dir.Normalize();
	MousePos = PawnPos+dir*1000;
	DrawDebugLine(GetWorld(), PawnPos, MousePos, FColor::Green, false, 0.1f,0,5);
	
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPlayerController::MoveForward(float Value)
{
	TankPawn->MoveForward(Value);
}

void ATankPlayerController::RotateRight(float Value)
{
	TankPawn->RotateRight(Value);
}
