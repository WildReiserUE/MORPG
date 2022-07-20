// Copyright Epic Games, Inc. All Rights Reserved.

#include "PCGame.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "MORPGCharacter.h"

APCGame::APCGame()
{
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
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
			auto Target = Hit.GetActor();
			if(Target)
				Target->NotifyActorOnClicked();
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
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &APCGame::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &APCGame::OnSetDestinationReleased);

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
	bInputPressed = false;
}