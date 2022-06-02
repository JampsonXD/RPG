// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_PlayerState.h"

#include "GAS/RPG_AbilitySystemComponent.h"
#include "GAS/RPG_AttributeSet.h"

ARPG_PlayerState::ARPG_PlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<URPG_AbilitySystemComponent>(TEXT("Ability System Component"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Full Replication - Singleplayer - Every GE is replicated to every client
	AbilitySystemComponent->ReplicationMode = EGameplayEffectReplicationMode::Full;

	AttributeSet = CreateDefaultSubobject<URPG_AttributeSet>(TEXT("Attribute Set"));
}

UAbilitySystemComponent* ARPG_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
