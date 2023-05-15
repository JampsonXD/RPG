// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameMode/DefaultGameModeData.h"
#include "RPG_GameMode.generated.h"

UCLASS(minimalapi)
class ARPG_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ARPG_GameMode();
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDefaultGameModeData* DefaultGameModeData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bShouldOverrideCharacterDataAsset = true;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	
};



