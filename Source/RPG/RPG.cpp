// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG.h"

#include "GameplayTagsManager.h"
#include "GameplayTag/RPG_TagLibrary.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FRPGGameModule, RPG, "RPG" );

void FRPGGameModule::StartupModule()
{
	UGameplayTagsManager::Get().OnLastChanceToAddNativeTags().AddRaw(this, &FRPGGameModule::RegisterNativeGameplayTags);
}

void FRPGGameModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

void FRPGGameModule::RegisterNativeGameplayTags()
{
	UGameplayTagsManager& TagManager = UGameplayTagsManager::Get();

	FRPG_TagLibrary::Startup(TagManager);

	TagManager.OnLastChanceToAddNativeTags().RemoveAll(this);
}
