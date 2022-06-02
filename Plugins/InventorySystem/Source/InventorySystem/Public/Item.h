// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ItemTypes.h"
#include "UObject/Object.h"
#include "Item.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentStackCountChanged, int32, OldStackCount, int32, NewStackCount);

class UInventorySystemComponent;

/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class INVENTORYSYSTEM_API UItem : public UObject
{
	GENERATED_UCLASS_BODY()

friend UInventorySystemComponent;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Item | Info")
	FName ItemName;

	// Label for our Item, what the Item Type is
	UPROPERTY(EditDefaultsOnly, Category = "Item | Info")
	FGameplayTag ItemType;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item | Info")
	FText ItemDescription;

	// Text that will be shown for "using" the item
	UPROPERTY(EditDefaultsOnly, Category = "Item | Info")
	FText ItemUseText;

	UPROPERTY(EditDefaultsOnly, Category = "Item | Info")
	class UTexture2D* ItemImage;

	// Skeletal Mesh used for showing off the item in the UI
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSoftObjectPtr<USkeletalMesh> ItemSkeletalMesh;

	// If we can be stacked, our max stack count, defaults to -1 if we have unlimited stacks
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	int32 MaxStackCount;

	// Current stack count of our item
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	int32 CurrentStackCount;

	// Delegate for when our Current Stack Count Changes
	UPROPERTY(BlueprintAssignable)
	FOnCurrentStackCountChanged OnCurrentStackCountChanged;

	UFUNCTION(BlueprintNativeEvent, Category = "Item | Interaction")
	bool CanUseItem(UInventorySystemComponent* TargetInventorySystemComponent, AActor* TargetActor);
	
	/* Functionality called when user tries to interact with the item */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item | Interaction")
	bool Use(UInventorySystemComponent* TargetInventorySystemComponent, AActor* TargetActor);
	
public:

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Item")
	bool IsStackable();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Item")
	bool HasUnlimitedStacks();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Item")
	int32 GetMaxItemStacks();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Item")
	int32 GetCurrentItemStacks();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Item")
	FGameplayTag GetItemType();

	// Makes a struct of data about the item we can use in things like our UI
	UFUNCTION(BlueprintPure , BlueprintCallable, Category = "Item Data")
	FItemUIData MakeItemData();

	UFUNCTION()
	FOnCurrentStackCountChanged& GetOnCurrentStackCountChangedDelegate();

	/** Adds Stacks to our current item
	 * @param StacksToAdd : Amount of stacks we will be adding to our UItem
	*/
	UFUNCTION()
	void AddStacks(int32 StacksToAdd);
	
};
