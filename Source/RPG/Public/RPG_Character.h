// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "InventorySystemInterface.h"
#include "RPG_Types.h"
#include "Characters/RPGCharacterInterface.h"
#include "GameFramework/Character.h"
#include "DataAssets/RPG_CharacterDataAsset.h"
#include "Interfaces/RPG_PrimaryAttackInterface.h"
#include "Items/RPG_ItemTypes.h"
#include "RPG_Character.generated.h"

class ARPG_WeaponActor;
UCLASS(config=Game)
class ARPG_Character : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface, public IRPGCharacterInterface, public IInventorySystemInterface, public IRPG_PrimaryAttackInterface
{
	GENERATED_BODY()

protected:
	
	virtual void PossessedBy(AController* NewController) override;

	virtual void BeginDestroy() override;
	
	// Weak Object Reference to our ASC
	TWeakObjectPtr<class URPG_AbilitySystemComponent> AbilitySystemComponent;

	// Weak Object Reference to our Attribute Set
	TWeakObjectPtr<class URPG_AttributeSet> AttributeSet;

	// Weak Object Reference to our Inventory System Component
	TWeakObjectPtr<class UInventorySystemComponent> InventorySystemComponent;

	// Handle for our default ability set
	UPROPERTY()
	FAbilitySetActiveHandle DefaultAbilitySetHandle;
	
	UFUNCTION()
	virtual void InitAttributeSets(class ARPG_PlayerState* PS);

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;

	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

	bool IsAiming_Implementation() const override;

	bool IsRunning_Implementation() const override;

	bool IsCrouching_Implementation() const override;

	bool IsWeaponLowered_Implementation() const override;

	bool IsJumping_Implementation() const override;

	virtual TMap<FGameplayTag, FAttackMontageContainer> GetPrimaryAttackMap() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnEquipmentSlotChanged(FEquippedSlot EquippedSlotData, UItem* Item, EEquipmentSlotChangeType ChangeType);

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URPG_CharacterDataAsset* CharacterData;
	
	ARPG_Character(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	virtual URPG_AbilitySystemComponent* GetRPGAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable)
	virtual UInventorySystemComponent* GetInventorySystemComponent() const override;

	UFUNCTION(BlueprintCallable, BlueprintPure , Category = "Attributes | Character Level")
	float GetCharacterLevel();

	UFUNCTION(BlueprintCallable, Category = "Ability Set")
	void AddDefaultAbilitySet();

	UFUNCTION(BlueprintCallable, Category = "Ability Set")
	void RemoveDefaultAbilitySet();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item Attach")
	void AttachItem(AActor* Actor, const FEquipData& EquipData);
};

