// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_TargetTypes.h"
#include "GAS_Types.generated.h"

/**
 * 
 */

class URPG_AbilitySystemComponent;
struct FAbilitySet;
class URPG_GameplayAbility;
static FGameplayTag PhysicalDamageTag = FGameplayTag::RequestGameplayTag("DamageType.Physical");
static FGameplayTag MagicDamageTag = FGameplayTag::RequestGameplayTag("DamageType.Magic");
static FGameplayTag TrueDamageTag = FGameplayTag::RequestGameplayTag("DamageType.True");

USTRUCT(BlueprintType)
struct FGameplayEffectContainer
{
	GENERATED_BODY()

public:
	FGameplayEffectContainer() { }

	// Targeting Type our Gameplay Effects will be applied to
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay Effect Container")
	TSubclassOf<URPG_TargetTypes> TargetingType;

	// List of Gameplay Effects that will be applied to our targets
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay Effect Container")
	TArray<TSubclassOf<UGameplayEffect>> TargetedEffects;
};

USTRUCT(BlueprintType)
struct FGameplayEffectContainerSpec
{
	GENERATED_BODY()

public:
	FGameplayEffectContainerSpec() { }

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay Effect Container Spec")
	FGameplayAbilityTargetDataHandle TargetData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay Effect Container Spec")
	TArray<FGameplayEffectSpecHandle> TargetGameplayEffectSpecs;

	// Whether or not our container has valid target data
	bool HasValidTargets() const;

	// Whether or not our container has any valid effects
	bool HasValidEffects() const;

	// Add HitResults, TargetActors to our Target Data
	void AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);
};

UENUM(BlueprintType)
enum class EAbilityInput : uint8
{
	// 0 None
	None				UMETA(DisplayName = "None"),
	// 1 Confirm
	Confirm				UMETA(DisplayName = "Confirm"),
	// 2 Cancel
	Cancel				UMETA(DisplayName = "Cancel"),
	// 3 Interact
	Interact			UMETA(DisplayName = "Interact"),
	// 4 PrimaryAttack
	PrimaryAttack		UMETA(DisplayName = "Primary Attack"),
	// 5 Ability 1
	Ability1			UMETA(DisplayName = "Ability1"),
	// 6 Ability 2
	Ability2			UMETA(DisplayName = "Ability2"),
	// 7 Ability 3
	Ability3			UMETA(DisplayName = "Ability3"),
	// 8 Ability 4
	Ability4			UMETA(DisplayName = "Ability4"),
	// 9 Sprint
	Sprint				UMETA(DisplayName = "Sprint"),
	// 10 Jump
	Jump				UMETA(DisplayName = "Jump")
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	PhysicalDamage,
	MagicDamage,
	TrueDamage
};