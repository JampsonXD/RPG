// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/RPG_SuperItem.h"

URPG_SuperItem::URPG_SuperItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ItemAbilitySet = nullptr;
}

void URPG_SuperItem::AddAbilitySet(UAbilitySystemComponent* AbilitySystemComponent, bool bShouldReactivate)
{
	if(!ItemAbilitySet)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability Set of item s% is invalid!"), *GetNameSafe(this));
		return;
	}
	
	// Check if our ability set is already active
	if(ItemAbilitySetHandle.IsActive())
	{
		if(!bShouldReactivate)
		{
			return;
		}

		ItemAbilitySetHandle.Remove();
	}

	ItemAbilitySetHandle = ItemAbilitySet->AddAbilitySet(AbilitySystemComponent, this);
}

bool URPG_SuperItem::CanUse() const
{
	return Super::CanUse();
}

void URPG_SuperItem::Use()
{
	K2_Use(GetOwningInventorySystemComponent(), GetOwningActor());
}

void URPG_SuperItem::Remove()
{
	K2_Remove(GetOwningInventorySystemComponent(), GetOwningActor());
}

void URPG_SuperItem::RemoveAbilitySet()
{
	ItemAbilitySetHandle.Remove();
}
