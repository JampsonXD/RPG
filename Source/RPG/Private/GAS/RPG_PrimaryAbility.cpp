// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/RPG_PrimaryAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interfaces/RPG_PrimaryAttackInterface.h"

int URPG_PrimaryAbility::GetCurrentPrimaryAnimationMontageIndex()
{
	const FGameplayTagContainer Container = AttackCounterTag.GetSingleTagContainer();
	const FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(Container);
	return GetAbilitySystemComponentFromActorInfo()->GetAggregatedStackCount(Query);
}

UAnimMontage* URPG_PrimaryAbility::GetPrimaryAttackMontage()
{
	auto Map = GetPrimaryAttackAnimationMontages();
	const int CurrentAttackCounter = GetCurrentPrimaryAnimationMontageIndex();

	// Determine which mapping to use based on our attack speed
	if(FAttackMontageContainer* Container = Map.Find(NormalAttackSpeedTag))
	{
		return (Container->Montages)[FMath::Clamp(CurrentAttackCounter, 0, Container->Montages.Num() - 1)];
	}

	return nullptr;
}

TMap<FGameplayTag, FAttackMontageContainer> URPG_PrimaryAbility::GetPrimaryAttackAnimationMontages()
{
	if(IRPG_PrimaryAttackInterface* Interface = Cast<IRPG_PrimaryAttackInterface>(GetAvatarActorFromActorInfo()))
	{
		return Interface->GetPrimaryAttackMap();
	}

	TMap<FGameplayTag, FAttackMontageContainer> Empty = TMap<FGameplayTag, FAttackMontageContainer>();
	return Empty;
}
