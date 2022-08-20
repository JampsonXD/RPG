// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "InventorySystemInterface.h"
#include "RPG_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPG_PlayerState : public APlayerState, public IAbilitySystemInterface, public IInventorySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System Component", meta = (AllowPrivateAccess = "true"))
	class URPG_AbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System Component", meta = (AllowPrivateAccess = "true"))
	class URPG_AttributeSet* AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory System Component", meta = (AllowPrivateAccess = "true"))
	class UInventorySystemComponent* InventorySystemComponent;

public:

	// Constructor
	ARPG_PlayerState(const FObjectInitializer& ObjectInitializer);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual UInventorySystemComponent* GetInventorySystemComponent() const override;
	
	/** Returns Core Attribute Set subobject **/
	FORCEINLINE class URPG_AttributeSet* GetAttributeSet() const { return AttributeSet; }
};
