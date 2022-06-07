// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "RPG_Types.h"
#include "GameFramework/Character.h"
#include "GAS/AbilitySet.h"
#include "Items/ArmorItem.h"
#include "RPG_Character.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FArmorChanged, FArmorEquipData, ArmorEquipData);

UCLASS(config=Game)
class ARPG_Character : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	USkeletalMeshComponent* ChestArmorMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	USkeletalMeshComponent* GauntletMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	USkeletalMeshComponent* HelmetMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	USkeletalMeshComponent* LegMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	TMap<EArmorEquipSlot, UArmorItem*> ArmorMap;
	
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Data")
	FPrimaryAssetId CharacterDataAssetID;

	UPROPERTY(BlueprintAssignable)
	FArmorChanged ArmorChanged;
	
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
	USkeletalMeshComponent* GetMeshComponentFromArmorSlotType(EArmorEquipSlot EquipSlot) const;
	
	UFUNCTION()
	virtual void InitAttributeSets(class ARPG_PlayerState* PS);

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void OnCharacterDataLoaded(FPrimaryAssetId LoadedId);

public:
	ARPG_Character(const FObjectInitializer& ObjectInitializer);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, BlueprintPure , Category = "Attributes | Character Level")
	float GetCharacterLevel();

	UFUNCTION(BlueprintPure, Category = "Item | Armor")
	bool IsEquipped(UItem* Item) const;
	
	UFUNCTION(BlueprintCallable, Category = "Item | Armor")
	bool TryEquipArmor(UArmorItem* Armor);

	UFUNCTION()
	void UnequipArmor(UArmorItem* ArmorItem);
};

