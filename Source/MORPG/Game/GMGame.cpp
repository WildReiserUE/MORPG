// Copyright Epic Games, Inc. All Rights Reserved.

#include "GMGame.h"
#include "PCGame.h"
#include "MORPGCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMORPGGameMode::AMORPGGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = APCGame::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/MORPG/Blueprints/Character/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	// static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/MORPG/Blueprints/PController/BP_TopDownPlayerController"));
	// if(PlayerControllerBPClass.Class != NULL)
	// {
	// 	PlayerControllerClass = PlayerControllerBPClass.Class;
	// }
}