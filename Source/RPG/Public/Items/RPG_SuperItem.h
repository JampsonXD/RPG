// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "GAS/AbilitySet.h"
#include "RPG_SuperItem.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPG_SuperItem : public UItem
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly)
	UAbilitySet* ItemAbilitySet;

	UPROPERTY(BlueprintReadWrite, Category = "RPG Item | Ability Set")
	FAbilitySetActiveHandle ItemAbilitySetHandle;

public:

	URPG_SuperItem(const FObjectInitializer& ObjectInitializer);

	void AddAbilitySet(UAbilitySystemComponent* AbilitySystemComponent, bool bShouldReactivate = false);
	
	void RemoveAbilitySet();
};
