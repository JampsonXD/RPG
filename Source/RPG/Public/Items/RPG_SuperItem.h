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

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG Item")
	UAbilitySet* ItemAbilitySet;

	URPG_SuperItem(const FObjectInitializer& ObjectInitializer);
};
