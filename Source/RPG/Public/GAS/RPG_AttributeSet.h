// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "RPG_AttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class URPG_AbilitySystemComponent;
/**
 * 
 */
UCLASS()
class RPG_API URPG_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	
	URPG_AttributeSet(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(URPG_AttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(URPG_AttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Mana")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(URPG_AttributeSet, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "Mana")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(URPG_AttributeSet, MaxMana)

	UPROPERTY(BlueprintReadOnly, Category = "PhysicalArmor")
	FGameplayAttributeData PhysicalArmor;
	ATTRIBUTE_ACCESSORS(URPG_AttributeSet, PhysicalArmor)

	UPROPERTY(BlueprintReadOnly, Category = "MagicArmor")
	FGameplayAttributeData MagicArmor;
	ATTRIBUTE_ACCESSORS(URPG_AttributeSet, MagicArmor)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(URPG_AttributeSet, Damage)

	UPROPERTY(BlueprintReadOnly, Category = "Character Level")
	FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS(URPG_AttributeSet, CharacterLevel)

	UPROPERTY(BlueprintReadOnly, Category = "Experience")
	FGameplayAttributeData Experience;
	ATTRIBUTE_ACCESSORS(URPG_AttributeSet, Experience)

	UPROPERTY(BlueprintReadOnly, Category = "Gold")
	FGameplayAttributeData Gold;
	ATTRIBUTE_ACCESSORS(URPG_AttributeSet, Gold)

protected:
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void HandleDamageAttribute(const FGameplayEffectModCallbackData& Data);

	virtual void HandleExperienceAttribute(UAbilitySystemComponent* ASC);

	virtual float EvaluateExperienceGained(URPG_AbilitySystemComponent* ASC, const float InExperience, float& LevelsGained);
};
