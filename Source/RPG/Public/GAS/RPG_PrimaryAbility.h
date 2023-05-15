// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/RPG_GameplayAbility.h"
#include "Interfaces/RPG_PrimaryAttackInterface.h"
#include "RPG_PrimaryAbility.generated.h"

/**
 * 
 */

UCLASS()
class RPG_API URPG_PrimaryAbility : public URPG_GameplayAbility
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AttackCounterTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag NormalAttackSpeedTag;

	UFUNCTION(BlueprintCallable)
	int GetCurrentPrimaryAnimationMontageIndex();

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetPrimaryAttackMontage();

	UFUNCTION(BlueprintCallable)
	TMap<FGameplayTag, FAttackMontageContainer> GetPrimaryAttackAnimationMontages();
	
};
