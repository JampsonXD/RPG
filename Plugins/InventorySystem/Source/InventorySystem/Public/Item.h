// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ItemInterface.h"
#include "Item.generated.h"

class UInventorySystemComponent;

/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class INVENTORYSYSTEM_API UItem : public UPrimaryDataAsset, public IItemInterface
{
	GENERATED_BODY()

friend UInventorySystemComponent;

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

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	bool bConsumeOnUse;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	bool bIsStackable;
	
public:

	virtual FGameplayTag GetItemType() const override;

	virtual FName GetItemName() const;

	virtual FText GetItemDescription() const;

	virtual UTexture2D* GetItemImage() const;

	virtual bool IsStackable() const override;

	virtual int GetMaxStackCount() const override;

	virtual bool ConsumeOnUse() const override;
};
