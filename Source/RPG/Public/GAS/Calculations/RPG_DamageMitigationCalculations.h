// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RPG_DamageMitigationCalculations.generated.h"

struct RPGDamageStatics;
struct FAggregatorEvaluateParameters;
struct FGameplayEffectCustomExecutionParameters;
/**
 * 
 */
UCLASS(Blueprintable)
class RPG_API URPG_DamageMitigationCalculations : public UObject
{
	GENERATED_BODY()

public:
	virtual float GetModifiedValue(float Damage, const FGameplayEffectCustomExecutionParameters& ExecutionParams, FAggregatorEvaluateParameters EvaluateParameters, RPGDamageStatics Statics);
};

UCLASS(NotBlueprintable)
class URPG_PhysicalDamageCalc : public URPG_DamageMitigationCalculations
{
	GENERATED_BODY()

public:
	virtual float GetModifiedValue(float Damage, const FGameplayEffectCustomExecutionParameters& ExecutionParams, FAggregatorEvaluateParameters EvaluateParameters, RPGDamageStatics Statics) override;
};

UCLASS(NotBlueprintable)
class URPG_MagicDamageCalc : public URPG_DamageMitigationCalculations
{
	GENERATED_BODY()

public:
	virtual float GetModifiedValue(float Damage, const FGameplayEffectCustomExecutionParameters& ExecutionParams, FAggregatorEvaluateParameters EvaluateParameters, RPGDamageStatics Statics) override;
};