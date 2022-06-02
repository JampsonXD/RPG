// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GAS/GAS_Types.h"
#include "GAS/RPG_AttributeSet.h"
#include "EC_SuperDamage.generated.h"

/**
 * 
 */

struct RPGDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalArmor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicArmor)

	RPGDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPG_AttributeSet, Damage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPG_AttributeSet, PhysicalArmor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPG_AttributeSet, MagicArmor, Target, false);
	}
};

static const RPGDamageStatics& DamageStatics()
{
	static RPGDamageStatics DamageStatics;
	return DamageStatics;
}

UCLASS()
class RPG_API UEC_SuperDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UEC_SuperDamage();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	virtual bool DoesRequirePassedInTags() const override;
};
