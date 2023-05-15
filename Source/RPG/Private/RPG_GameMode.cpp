// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_GameMode.h"
#include "RPG_Character.h"
#include "RPG_PlayerController.h"
#include "RPG_PlayerState.h"
#include "Kismet/GameplayStatics.h"

ARPG_GameMode::ARPG_GameMode()
{
	// Set our Default Player State Class to our RPG Player State
	PlayerStateClass = ARPG_PlayerState::StaticClass();

	// Set our Default Player Controller Class to our RPG Player Controller
	PlayerControllerClass = ARPG_PlayerController::StaticClass();
}

UClass* ARPG_GameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if(DefaultGameModeData)
	{
		return DefaultGameModeData->PlayerCharacterClass;
	}
	
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* ARPG_GameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
	const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save default player pawns into a map
	UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer);
	APawn* ResultPawn = GetWorld()->SpawnActorDeferred<APawn>(PawnClass, SpawnTransform);
	if (!ResultPawn)
	{
		UE_LOG(LogGameMode, Warning, TEXT("SpawnDefaultPawnAtTransform: Couldn't spawn Pawn of type %s at %s"), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
	}
	
	ARPG_Character* Character = Cast<ARPG_Character>(ResultPawn);
	
	// Check if our default pawn is an RPG character and setup its default values using the passed in character data
	if(bShouldOverrideCharacterDataAsset && Character && DefaultGameModeData && DefaultGameModeData->CharacterData)
	{
		Character->CharacterData = DefaultGameModeData->CharacterData;
	}
	
	UGameplayStatics::FinishSpawningActor(ResultPawn, SpawnTransform);
	return ResultPawn;
}
