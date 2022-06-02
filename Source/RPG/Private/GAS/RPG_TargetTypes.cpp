// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/RPG_TargetTypes.h"
#include "RPG_Character.h"

void URPG_TargetTypes::GetTargets_Implementation(ARPG_Character* TargetingCharacter, AActor* TargetingActor,
	FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	return;
}

void URPG_TargetType_Self::GetTargets_Implementation(ARPG_Character* TargetingCharacter, AActor* TargetingActor,
                                                     FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	OutActors.Add(TargetingActor);
}
