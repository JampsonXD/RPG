// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ItemTypes.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORYSYSTEM_API IItemInterface
{
	GENERATED_BODY()

public:

	/* The owning inventory system */
	virtual UInventorySystemComponent* GetOwningInventorySystemComponent() const = 0;

	/* Owning Actor of the item, generally will be the owning actor of the inventory system */
	virtual AActor* GetOwningActor() const = 0;

	/* The type of item this is */
	virtual FGameplayTag GetItemType() const = 0;

	/* Whether the item can currently be used or not */
	virtual bool CanUse() const = 0;

	/* Called when an item is added to an inventory system */
	virtual void Add(UInventorySystemComponent* InventorySystemComponent) = 0;

	/* Called when a user tries to use the item */
	virtual void Use() = 0;

	/* Called when an item is removed from an inventory system */
	virtual void Remove() = 0;

	/* Whether the item is stackable or not */
	virtual bool IsStackable() const = 0;

	/* The max stack count of the item */
	virtual int GetMaxStackCount() const = 0;

	/* The current stack count of the item */
	virtual int GetCurrentStackCount() const = 0;

	/* Call to add stacks to the items stack count */
	virtual void AddStacks(int StacksToAdd) = 0;

	/* Call to remove stacks from the items stack count */
	virtual void RemoveStacks(int StacksToRemove) = 0;
};
