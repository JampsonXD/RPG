// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGAssetManager.h"

#include "AbilitySystemGlobals.h"

const FPrimaryAssetType URPGAssetManager::CharacterType = TEXT("Character");
const FPrimaryAssetType URPGAssetManager::GameModeType = TEXT("GameMode");

URPGAssetManager& URPGAssetManager::Get()
{
	check(GEngine);

	if(URPGAssetManager* obj = Cast<URPGAssetManager>(GEngine->AssetManager))
	{
		return *obj;
	}

	UE_LOG(LogTemp, Fatal, TEXT("The current asset manager is not of type URPGAssetManager in DefaultEngine.ini. This must be set to URPGAssetManager!"))
	return *NewObject<URPGAssetManager>();
}

void URPGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
