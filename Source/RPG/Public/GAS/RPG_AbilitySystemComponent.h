// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySet.h"
#include "AbilitySystemComponent.h"
#include "GAS_Types.h"
#include "RPG_AbilitySystemComponent.generated.h"

/**
 * 
 */

class UAbilitySet;

UCLASS()
class RPG_API URPG_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "RPG Ability System Component")
	TArray<FActiveGameplayEffectHandle> ApplyGameplayEffectContainerSpec(const FGameplayEffectContainerSpec& ContainerSpec);
	
	UFUNCTION(BlueprintCallable, Category = "Ability Set")
	FAbilitySetActiveHandle AddAbilitySet(UAbilitySet* AbilitySet, UObject* SourceObject);

	UFUNCTION(BlueprintCallable, Category = "Ability Set")
	void RemoveAbilitySetWithHandle(FAbilitySetActiveHandle& ActiveHandle);

	UFUNCTION(BlueprintCallable, Category = "Experience")
	float GetExperienceForNextLevel() const;

	UFUNCTION()
	void SetNextLevelExperience();

	UFUNCTION()
	void GrantLevelUpAttributes(float LevelsGained, float CurrentLevel);

protected:

	// Whether we have granted our default abilities
	bool bDefaultAbilitiesGiven = false;

public:
	// Experience needed for our next character level up
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Experience")
	float ExperienceNeeded = 50.0f;

	// Modifier that multiplies experience needed for the current level to get the next level experience
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Experience")
	float ExperienceModifier = 1.25f;
	
};
