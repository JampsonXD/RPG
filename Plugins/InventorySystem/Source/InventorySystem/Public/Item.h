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

public:

	// Label for our Item, what the Item Type is
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item | Info")
	FGameplayTag ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item | Info")
	FName ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item | Info")
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item | Info")
	TSoftObjectPtr<UTexture2D> ItemImageSoftPointer;

	// If we can be stacked, our max stack count, defaults to -1 if we have unlimited stacks
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	int32 MaxStackCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	bool bConsumeOnUse;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	bool bIsStackable;

	UFUNCTION(BlueprintCallable, Category = "Item | Info")
	UTexture2D* K2_GetItemImage();

	virtual FGameplayTag GetItemType() const override;

	virtual FName GetItemName() const;

	virtual FText GetItemDescription() const;

	virtual UTexture2D* GetItemImage() const;

	virtual bool IsStackable() const override;

	virtual int GetMaxStackCount() const override;

	virtual bool ConsumeOnUse() const override;
};
