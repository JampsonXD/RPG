// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FXManager.h"
#include "GameplayTagContainer.h"
#include "RPG_Types.h"
#include "UObject/NoExportTypes.h"
#include "RPG_GameSingleton.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RPG_API URPG_GameSingleton : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	UFXManager* FXManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Singleton")
	TMap<FGameplayTag, FDamageTypeData> DamageTypeMap;

	/* Stores a map of attributes linked to a curve table to determine how much they level up at the designated level */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Experience")
	TMap<FGameplayAttribute, UCurveFloat*> AttributeLevelUpMap;
	
public:

	URPG_GameSingleton(const FObjectInitializer& ObjectInitializer);

	/**
	 * @return Returns the instance of our Game Singleton as an URPG_GameSingleton if valid
	 **/
	UFUNCTION(BlueprintCallable, Category = "RPG Singleton")
	static URPG_GameSingleton* GetSingleton();

	/**
	 * @return Returns our local instance of the Effect Manager
	 */
	UFUNCTION(BlueprintCallable, Category = "RPG Singleton")
	UFXManager* GetFXManager();

	/**
	 * @return Returns a pointer to our Attribute Level Up Map
	 **/
	TMap<FGameplayAttribute, UCurveFloat*>* GetAttributeLevelUpMap();

	/**
	 * @param Container - The GameplayTagContainer we will be checking against our TMaps GameplayTags
	 * @return Returns the first FDamageTypeData from our Map that shares the same Tag, else returns a default struct
	 **/
	FDamageTypeData GetDamageTypeDataFromContainer(const FGameplayTagContainer& Container) const;
	
};
