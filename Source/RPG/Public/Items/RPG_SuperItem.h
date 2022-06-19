// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "GAS/AbilitySet.h"
#include "RPG_SuperItem.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPG_SuperItem : public UItem
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "RPG Item | Ability Set")
	UAbilitySet* ItemAbilitySet;

	UPROPERTY(BlueprintReadWrite, Category = "RPG Item | Ability Set")
	FAbilitySetActiveHandle ItemAbilitySetHandle;

public:

	URPG_SuperItem(const FObjectInitializer& ObjectInitializer);

	void AddAbilitySet(UAbilitySystemComponent* AbilitySystemComponent, bool bShouldReactivate = false);

	virtual bool CanUse() const override;

	virtual void Use() override;

	virtual void Remove() override;

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Can Use")
	bool K2_CanUse() const;

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Use")
	void K2_Use(UInventorySystemComponent* ISC, AActor* InOwningActor);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Remove")
	void K2_Remove(UInventorySystemComponent* ISC, AActor* InOwningActor);
	
	void RemoveAbilitySet();
};
