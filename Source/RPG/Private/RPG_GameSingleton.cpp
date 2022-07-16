// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_GameSingleton.h"

#include "BlueprintGameplayTagLibrary.h"

URPG_GameSingleton::URPG_GameSingleton(const FObjectInitializer& ObjectInitializer)
{
	
}

URPG_GameSingleton* URPG_GameSingleton::GetSingleton()
{
	if(GEngine)
	{
		URPG_GameSingleton* Instance = Cast<URPG_GameSingleton>(GEngine->GameSingleton);
		return Instance;
	}

	return nullptr;
}

UFXManager* URPG_GameSingleton::GetFXManager()
{
	if(!FXManager)
	{
		FXManager = NewObject<UFXManager>(this, UFXManager::StaticClass());
		return FXManager;
	}

	return FXManager;
}

TMap<FGameplayAttribute, UCurveFloat*>* URPG_GameSingleton::GetAttributeLevelUpMap()
{
	return &AttributeLevelUpMap;
}

FDamageTypeData URPG_GameSingleton::GetDamageTypeDataFromContainer(const FGameplayTagContainer& Container) const
{
	TArray<FGameplayTag> Tags;
	DamageTypeMap.GetKeys(Tags);
	const FGameplayTagContainer TagMapContainer = FGameplayTagContainer::CreateFromArray(Tags).FilterExact(Container);
	if(TagMapContainer.Num() > 0)
	{
		return *DamageTypeMap.Find(TagMapContainer.First());
	}

	return FDamageTypeData();
}
