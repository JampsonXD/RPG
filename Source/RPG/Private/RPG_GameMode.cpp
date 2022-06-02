// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_GameMode.h"
#include "RPG_Character.h"
#include "RPG_PlayerController.h"
#include "RPG_PlayerState.h"
#include "UObject/ConstructorHelpers.h"

ARPG_GameMode::ARPG_GameMode()
{
	// Set our Default Player State Class to our RPG Player State
	PlayerStateClass = ARPG_PlayerState::StaticClass();

	// Set our Default Player Controller Class to our RPG Player Controller
	PlayerControllerClass = ARPG_PlayerController::StaticClass();
}
