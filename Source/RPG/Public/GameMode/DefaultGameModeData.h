// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RPG_PlayerCharacter.h"
#include "DataAssets/RPG_CharacterDataAsset.h"
#include "DefaultGameModeData.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UDefaultGameModeData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ARPG_PlayerCharacter> PlayerCharacterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URPG_CharacterDataAsset* CharacterData;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
