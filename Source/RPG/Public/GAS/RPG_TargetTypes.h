// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "UObject/Object.h"
#include "RPG_TargetTypes.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class RPG_API URPG_TargetTypes : public UObject
{
	GENERATED_BODY()

public:
	URPG_TargetTypes() { }
	
	UFUNCTION(BlueprintNativeEvent)
	void GetTargets(class ARPG_Character* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};

UCLASS(NotBlueprintable)
class URPG_TargetType_Self : public URPG_TargetTypes
{
	GENERATED_BODY()
public:
	URPG_TargetType_Self() { }

	virtual void GetTargets_Implementation(ARPG_Character* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};
