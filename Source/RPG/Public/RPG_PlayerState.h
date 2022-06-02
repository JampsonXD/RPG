// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "RPG_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPG_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System Component", meta = (AllowPrivateAccess = "true"))
	class URPG_AbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System Component", meta = (AllowPrivateAccess = "true"))
	class URPG_AttributeSet* AttributeSet;

public:

	// Constructor
	ARPG_PlayerState(const FObjectInitializer& ObjectInitializer);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	/** Returns Core Attribute Set subobject **/
	FORCEINLINE class URPG_AttributeSet* GetAttributeSet() const { return AttributeSet; }
};
