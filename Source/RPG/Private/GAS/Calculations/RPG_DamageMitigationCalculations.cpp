// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Calculations/RPG_DamageMitigationCalculations.h"

#include "GAS/Calculations/EC_SuperDamage.h"

float URPG_DamageMitigationCalculations::GetModifiedValue(float Damage,
                                             const FGameplayEffectCustomExecutionParameters& ExecutionParams, FAggregatorEvaluateParameters EvaluateParameters,
                                             RPGDamageStatics Statics)
{
	return Damage;
}

float URPG_PhysicalDamageCalc::GetModifiedValue(float Damage,
	const FGameplayEffectCustomExecutionParameters& ExecutionParams, FAggregatorEvaluateParameters EvaluateParameters,
	RPGDamageStatics Statics)
{
	float PhysicalArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Statics.PhysicalArmorDef, EvaluateParameters, PhysicalArmor);
	return (100 / (100 + PhysicalArmor)) * Damage;
}

float URPG_MagicDamageCalc::GetModifiedValue(float Damage,
	const FGameplayEffectCustomExecutionParameters& ExecutionParams, FAggregatorEvaluateParameters EvaluateParameters,
	RPGDamageStatics Statics)
{
	float MagicArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Statics.MagicArmorDef, EvaluateParameters, MagicArmor);
	return (100 / (100 + MagicArmor)) * Damage;
}
