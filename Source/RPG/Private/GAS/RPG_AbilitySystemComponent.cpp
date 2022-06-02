// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/RPG_AbilitySystemComponent.h"
#include "RPG_GameSingleton.h"
#include "GAS/RPG_AttributeSet.h"

FAbilitySetActiveHandle URPG_AbilitySystemComponent::AddAbilitySet(UAbilitySet* AbilitySet,
	UObject* SourceObject)
{
	return AbilitySet->AddAbilitySet(this, SourceObject);
}

void URPG_AbilitySystemComponent::RemoveAbilitySetWithHandle(FAbilitySetActiveHandle& ActiveHandle)
{
	ActiveHandle.Remove();
}

float URPG_AbilitySystemComponent::GetExperienceForNextLevel() const
{
	return ExperienceNeeded;
}

void URPG_AbilitySystemComponent::SetNextLevelExperience()
{
	ExperienceNeeded *= ExperienceModifier;
}

void URPG_AbilitySystemComponent::GrantLevelUpAttributes(float LevelsGained, float CurrentLevel)
{
	// Get our singleton, return early if we can't get an instance
	URPG_GameSingleton* Singleton = URPG_GameSingleton::GetSingleton();
	if(!Singleton) return;

	// Get our AttributeLevelMap from our Singleton and return early if we dont have a valid map
	TMap<FGameplayAttribute, UCurveFloat*>* AttributeMap = Singleton->GetAttributeLevelUpMap();
	if(!AttributeMap) return;

	/*
	 * Creates a dynamic instant gameplay effect for our player that is used for granting all attribute gains on level up.
	 * First adds our character levels based on the LevelsGained param passed through, then we iterate over our
	 * Attribute Map and get the total float value for each of our gained levels combined before adding those
	 * to our effect.
	 **/
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("GE_LevelUp")));
	Effect->DurationPolicy = EGameplayEffectDurationType::Instant;

	int32 Index = Effect->Modifiers.Num();
	// Add enough for our Attribute Map plus an additional for our Character Level
	Effect->Modifiers.SetNum(Index + AttributeMap->Num() + 1);

	FGameplayModifierInfo& LevelInfo = Effect->Modifiers[Index];
	LevelInfo.ModifierMagnitude = FScalableFloat(LevelsGained);
	LevelInfo.ModifierOp = EGameplayModOp::Additive;
	LevelInfo.Attribute = URPG_AttributeSet::GetCharacterLevelAttribute();

	Index++;

	// Only iterate through our Attribute Map if we one or more pairs
	if(AttributeMap->Num() > 0)
	{
		for(const TPair<FGameplayAttribute, UCurveFloat*>& Pair : *AttributeMap)
		{
			float TotalValue = 0;
		
			for(int CurrentGainedLevel = CurrentLevel + 1; CurrentGainedLevel <= CurrentLevel + LevelsGained; CurrentGainedLevel++)
			{
				TotalValue += Pair.Value->GetFloatValue(CurrentGainedLevel);
			}
		
			FGameplayModifierInfo& ModInfo = Effect->Modifiers[Index];
			ModInfo.ModifierMagnitude = FScalableFloat(TotalValue);
			ModInfo.ModifierOp = EGameplayModOp::Additive;
			ModInfo.Attribute = Pair.Key;
			Index++;
		}
	}

	ApplyGameplayEffectToSelf(Effect, 1.0f, MakeEffectContext());

}
