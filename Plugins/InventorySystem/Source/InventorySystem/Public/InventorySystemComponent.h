// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Components/ActorComponent.h"
#include "InventorySystemComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAdded, UItem*, NewItem);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FItemQuery, UItem*, Item);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UInventorySystemComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()


	// Owning actor of our component
	UPROPERTY()
	AActor* OwningActor;

	// Actor that our owning actor maybe controlling. Can be the same as our owning actor
	UPROPERTY()
	AActor* AvatarActor;
	
protected:
	
	// Our Array of Items the component currently has
	UPROPERTY(BlueprintReadOnly, Category = "Inventory System Component | Items")
	TArray<UItem*> InventoryItems;

	/** Maximum size of our components inventory **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory System Component")
	int32 MaxInventorySize;
	
	/* Array of Item Classes to grant our Component when initialized
	adding one or more of the same class will add a new item or update the existing
	stack count of an item based on its stackable setting
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory System Component | Defaults")
	TArray<TSubclassOf<UItem>> DefaultInventoryItems;

	UPROPERTY(BlueprintAssignable)
	FOnItemAdded OnItemAdded;

public:
	
	/** Tries adding an item of class to our inventory
	 * @param Item : The item class to add
	 *  @param StacksToGrant : The amount of the item we want to try to add
	 *  @return : Returns true if we successfully added an item
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Items")
	bool AddItem(TSubclassOf<UItem> Item, int32 StacksToGrant = 1);

	/** Initializes our Default Inventory Items
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Defaults")
	void InitInventorySystemComponent();

	/** Getter for our Inventory Items Array
	 * @return TArray<UItem*> : Returns our Inventory Items
	 */
	UFUNCTION(BlueprintCallable)
	bool GetInventoryItems(FGameplayTag Type, TArray<UItem*>& OutItems) const;

	UFUNCTION()
	FOnItemAdded& GetOnItemAddedDelegate();

	UFUNCTION(BlueprintCallable, Category = "Item Query")
	bool TryGetItemWithStackCount(int32 StackCount, UItem*& OutItem);
	
	UFUNCTION(BlueprintCallable, Category = "Item Query")
	bool QueryInventoryForItem(FItemQuery Query, UItem*& OutItem);

	UFUNCTION(BlueprintCallable, Category = "Item Query")
	bool QueryInventoryForItems(FItemQuery Query, TArray<UItem*>& OutItems);
	
	bool QueryInventoryForItem(TFunction<bool(UItem* Item)> Query, UItem*& OutItem);

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Items")
	bool TryUsingInventoryItem(UItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Initialize")
	void InitActorInfo(AActor* InOwningActor, AActor* InAvatarActor);

protected:

	UFUNCTION()
	bool ContainsValidItemOfClass(const TSubclassOf<UItem>& Item, TArray<UItem*>& OutItemsFound);

	/** Creates an Instance of the passed in Item Class, adding the passed in stacks up to its maximum allowed
	 * @param  Item : Passed in Item Class to spawn
	 * @param StacksToGrant : Passed in Stack Amount by reference
	 */
	UFUNCTION()
	void CreateItemInstance(TSubclassOf<UItem> Item, int32& StacksToGrant);

	/** Adds available stacks to items as long as we still have stacks to grant
	 * @param Items : Array of items we are trying to add stacks to
	 * @param StacksToGrant : The amount of stacks to try to add
	 * @return : Returns false if we are unable to add stacks to the item
	 */
	UFUNCTION()
	static void AddStacksToItems(TArray<UItem*>& Items, int32& StacksToGrant);

	/** Adds available stacks to an item, if no stacks are left, returns true
	 * @param StacksToGrant : The stacks we want to add to an item instance
	 * @param Item : The Item Instance to add stacks to
	 * @return bool : Returns true if we used all the stacks
	 */
	UFUNCTION()
	static bool AddAvailableStacksToItem(int32& StacksToGrant, UItem* Item);

	/** Returns the available stack count - Max Stacks - Current Stacks
	 * @param Item : Item to get available stacks from 
	 */
	UFUNCTION()
	static int32 GetAvailableStackCount(UItem* Item);
	
	UFUNCTION()
	bool HasUnlimitedInventorySize() const;
	
	bool IsInventoryFull() const;
};