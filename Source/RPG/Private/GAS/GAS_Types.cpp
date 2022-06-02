// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GAS_Types.h"

#include "AbilitySystemComponent.h"
#include "GAS/RPG_GameplayAbility.h"

bool FGameplayEffectContainerSpec::HasValidTargets() const
{
	return TargetData.Num() > 0;
}

bool FGameplayEffectContainerSpec::HasValidEffects() const
{
	return TargetGameplayEffectSpecs.Num() > 0;
}

void FGameplayEffectContainerSpec::AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors)
{
	for(const FHitResult& HitResult : HitResults)
	{
		FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
		TargetData.Add(Data);
	}

	if(TargetActors.Num() > 0)
	{
		FGameplayAbilityTargetData_ActorArray* Data = new FGameplayAbilityTargetData_ActorArray();
		Data->TargetActorArray.Append(TargetActors);
		TargetData.Add(Data);
	}
}
