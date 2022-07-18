// Copyright Epic Games, Inc. All Rights Reserved.

#include "PCGame.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "MORPGCharacter.h"
#include "Engine/World.h"

APCGame::APCGame()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void APCGame::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if(bInputPressed)
	{
		FollowTime += DeltaTime;

		// Look for the touch location
		FVector HitLocation = FVector::ZeroVector;
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, true, Hit);
		HitLocation = Hit.Location;

		// Direct the Pawn towards that location
		APawn* const MyPawn = GetPawn();
		if(MyPawn)
		{
			FVector WorldDirection = (HitLocation - MyPawn->GetActorLocation()).GetSafeNormal();
			MyPawn->AddMovementInput(WorldDirection, 1.f, false);
		}
	}
	else
	{
		FollowTime = 0.f;
		
	}
}

void APCGame::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &APCGame::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &APCGame::OnSetDestinationReleased);

	// // support touch devices 
	// InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &APCGame::OnTouchPressed);
	// InputComponent->BindTouch(EInputEvent::IE_Released, this, &APCGame::OnTouchReleased);

}

void APCGame::OnSetDestinationPressed()
{
	// We flag that the input is being pressed
	bInputPressed = true;
	StopMovement();
	FVector HitLocation = FVector::ZeroVector;
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	HitLocation = Hit.Location;

	// We move there and spawn some particles
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, HitLocation);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, HitLocation, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	
}

void APCGame::OnSetDestinationReleased()
{
	// Player is no longer pressing the input
	bInputPressed = false;

// 	// If it was a short press
	// if(FollowTime <= ShortPressThreshold)
	// {
	// 	// We look for the location in the world where the player has pressed the input
	// 	FVector HitLocation = FVector::ZeroVector;
	// 	FHitResult Hit;
	// 	GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	// 	HitLocation = Hit.Location;
	//
	// 	// We move there and spawn some particles
	// 	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, HitLocation);
	// 	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, HitLocation, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	// }
}

// void APCGame::OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location)
// {
// 	bIsTouch = true;
// 	OnSetDestinationPressed();
// }
//
// void APCGame::OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location)
// {
// 	bIsTouch = false;
// 	OnSetDestinationReleased();
// }
