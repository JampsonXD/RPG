// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "AbilitySet.generated.h"

class URPG_AbilitySystemComponent;
class URPG_GameplayAbility;
struct FActiveGameplayEffectHandle;
struct FGameplayAbilitySpecHandle;
struct FAbilitySet;

USTRUCT(BlueprintType)
struct FRPG_GameplayAbilityInfo
{
	GENERATED_BODY()

	FRPG_GameplayAbilityInfo()
	{
		GameplayAbility = nullptr;
		AbilityLevel = 1.f;
	}

	FRPG_GameplayAbilityInfo(TSubclassOf<URPG_GameplayAbility> InGameplayAbility, float InAbilityLevel)
	{
		GameplayAbility = InGameplayAbility;
		AbilityLevel = InAbilityLevel;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS | RPG Ability Info")
	TSubclassOf<URPG_GameplayAbility> GameplayAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS | RPG Ability Info")
	float AbilityLevel;
	
};

USTRUCT(BlueprintType)
struct FRPG_GameplayEffectInfo
{
	GENERATED_BODY()

	FRPG_GameplayEffectInfo()
	{
		GameplayEffect = nullptr;
		EffectLevel = 1.f;
	}

	FRPG_GameplayEffectInfo(TSubclassOf<UGameplayEffect> InGameplayEffect, float InEffectLevel)
	{
		GameplayEffect = InGameplayEffect;
		EffectLevel = InEffectLevel;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS | RPG Effect Info")
	TSubclassOf<UGameplayEffect> GameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS | RPG Effect Info")
	float EffectLevel;
};

USTRUCT(BlueprintType)
struct FRPG_AttributeSetInfo
{
	GENERATED_BODY()

	FRPG_AttributeSetInfo()
	{
		AttributeSet = nullptr;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS | RPG Attribute Info")
	TSubclassOf<UAttributeSet> AttributeSet;
};


/** Active Handle for an Ability Set that has been activated, contains references to the gameplay abilities and gameplay effects handles created **/
USTRUCT(BlueprintType)
struct FAbilitySetActiveHandle
{
	GENERATED_BODY()
	
	FAbilitySetActiveHandle(){};
	
protected:

	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	TArray<FGameplayAbilitySpecHandle> AbilityHandles;
	
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;
	
	TArray<TWeakObjectPtr<UAttributeSet>> AttributeSets;

	void RemoveAbilities();

	void RemoveActiveGameplayEffects();

	void RemoveAttributeSets();
	
public:

	bool IsActive() const { return AbilityHandles.Num() > 0 || GameplayEffectHandles.Num() > 0 || AttributeSets.Num() > 0; }

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle) { AbilityHandles.Add(Handle); }

	void AddActiveGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle) { GameplayEffectHandles.Add(Handle); }

	void AddAttributeSet(const TWeakObjectPtr<UAttributeSet>& AttributeSet) { AttributeSets.Add(AttributeSet); }

	void SetAbilitySystemComponent(const TWeakObjectPtr<UAbilitySystemComponent>& ASCWeakPointer) { AbilitySystemComponent = ASCWeakPointer; }

	void Remove();
};

/** Ability Set's are data assets of gameplay effects and gameplay abilities to add to an ability system component.
*	Ability Set's will return an active handle containing references to the active gameplay effect handles and ability handles,
*	and a weak pointer to the ability system component they were added onto. Useful for things such as effects and abilities added
*	onto a character on spawn, or for weapons or other objects that grant abilities and effects.
*/
UCLASS(BlueprintType, Const)
class UAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS | Ability Set")
	TArray<FRPG_GameplayAbilityInfo> GameplayAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS | Ability Set")
	TArray<FRPG_GameplayEffectInfo> GameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS | Ability Set")
	TArray<FRPG_AttributeSetInfo> AttributeSets;

public:
	FAbilitySetActiveHandle AddAbilitySet(UAbilitySystemComponent* AbilitySystemComponent, UObject* SourceObject = nullptr) const;

protected:
	void AddGameplayAbilities(UAbilitySystemComponent* AbilitySystemComponent, UObject* SourceObject, FAbilitySetActiveHandle& Handle) const;
	void AddGameplayEffects(UAbilitySystemComponent* AbilitySystemComponent, const UObject* SourceObject, FAbilitySetActiveHandle& Handle) const;
	void AddAttributeSets(UAbilitySystemComponent* AbilitySystemComponent, FAbilitySetActiveHandle& Handle) const;
};
