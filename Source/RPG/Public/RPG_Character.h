// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "RPG_Types.h"
#include "Characters/RPGCharacterInterface.h"
#include "DataAssets/RPG_CharacterDataAsset.h"
#include "GameFramework/Character.h"
#include "GAS/AbilitySet.h"
#include "Items/ArmorItem.h"
#include "RPG_Character.generated.h"

UCLASS(config=Game)
class ARPG_Character : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface, public IRPGCharacterInterface
{
	GENERATED_BODY()

protected:
	
	virtual void PossessedBy(AController* NewController) override;

	/* Primary Asset Id used to load data related to our character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Data")
	FPrimaryAssetId CharacterDataAssetID;
	
	// Weak Object Reference to our ASC
	TWeakObjectPtr<class URPG_AbilitySystemComponent> AbilitySystemComponent;

	// Weak Object Reference to our Attribute Set
	TWeakObjectPtr<class URPG_AttributeSet> AttributeSet;

	// Default Ability Set to grant our player
	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	UAbilitySet* DefaultAbilitySet;

	// Handle for our default ability set
	UPROPERTY()
	FAbilitySetActiveHandle DefaultAbilitySetHandle;
	
	UFUNCTION()
	virtual void InitAttributeSets(class ARPG_PlayerState* PS);

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void OnCharacterDataLoaded(FPrimaryAssetId LoadedId);

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;

	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

	bool IsAiming_Implementation() const override;

	bool IsRunning_Implementation() const override;

	bool IsCrouching_Implementation() const override;

	bool IsWeaponLowered_Implementation() const override;

	bool IsJumping_Implementation() const override;

public:
	ARPG_Character(const FObjectInitializer& ObjectInitializer);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, BlueprintPure , Category = "Attributes | Character Level")
	float GetCharacterLevel();
};

