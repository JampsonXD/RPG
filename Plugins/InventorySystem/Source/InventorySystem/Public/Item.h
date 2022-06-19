// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ItemInterface.h"
#include "ItemTypes.h"
#include "UObject/Object.h"
#include "Item.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentStackCountChanged, int32, OldStackCount, int32, NewStackCount);

class UInventorySystemComponent;

/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class INVENTORYSYSTEM_API UItem : public UObject, public IItemInterface
{
	GENERATED_BODY()

friend UInventorySystemComponent;

	UPROPERTY()
	UInventorySystemComponent* OwningInventorySystemComponent;

	UPROPERTY()
	AActor* OwningActor;

	void SetOwningInventorySystemComponent(UInventorySystemComponent* OwningISC) { OwningInventorySystemComponent = OwningISC; }

	void SetOwningActor(AActor* InOwningActor) { OwningActor = InOwningActor; }

protected:

	// Label for our Item, what the Item Type is
	UPROPERTY(EditDefaultsOnly, Category = "Item | Info")
	FGameplayTag ItemType;

	UPROPERTY(EditDefaultsOnly, Category = "Item | Info")
	FName ItemName;

	UPROPERTY(EditDefaultsOnly, Category = "Item | Info")
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, Category = "Item | Info")
	TSoftObjectPtr<UTexture2D> ItemImageSoftPointer;

	// If we can be stacked, our max stack count, defaults to -1 if we have unlimited stacks
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	int32 MaxStackCount;

	// Current stack count of our item
	UPROPERTY(BlueprintReadOnly, Category = "Item")
	int32 CurrentStackCount;

	// Delegate for when our Current Stack Count Changes
	UPROPERTY(BlueprintAssignable)
	FOnCurrentStackCountChanged OnCurrentStackCountChanged;

	bool HasUnlimitedStacks() const { return MaxStackCount == -1; }
	
public:

	UItem(const FObjectInitializer& ObjectInitializer);

	virtual UInventorySystemComponent* GetOwningInventorySystemComponent() const override;

	virtual AActor* GetOwningActor() const override;

	virtual FGameplayTag GetItemType() const override;

	virtual FName GetItemName() const;

	virtual FText GetItemDescription() const;

	virtual UTexture2D* GetItemImage() const;

	virtual bool CanUse() const override;

	virtual void Use() override;

	virtual void Add(UInventorySystemComponent* InventorySystemComponent) override;

	virtual void Remove() override;

	virtual int GetCurrentStackCount() const override;

	virtual bool IsStackable() const override;

	virtual int GetMaxStackCount() const override;

	UFUNCTION()
	FOnCurrentStackCountChanged& GetOnCurrentStackCountChangedDelegate();

	/** Adds Stacks to our current item
	 * @param StacksToAdd : Amount of stacks we will be adding to our Item
	*/
	UFUNCTION()
	virtual void AddStacks(int32 StacksToAdd) override;

	/** Removes Stacks from our current item
	 * @param StacksToRemove : Amount of stacks we will try removing from our Item.
	 */
	UFUNCTION()
	virtual void RemoveStacks(int32 StacksToRemove) override;
};
