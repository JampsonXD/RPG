// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EquippableItemInterface.generated.h"

class UItem;
class URPG_AbilitySystemComponent;
class ARPG_Character;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UEquippableItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_API IEquippableItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetupItem(UItem* ItemDataAsset);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EquipItem(ARPG_Character* EquippingCharacter, URPG_AbilitySystemComponent* RPGAbilitySystemComponent);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UnEquipItem(ARPG_Character* EquippingCharacter, URPG_AbilitySystemComponent* RPGAbilitySystemComponent);
};
