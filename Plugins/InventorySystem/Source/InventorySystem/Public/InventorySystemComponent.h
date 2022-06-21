// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemTypes.h"
#include "Components/ActorComponent.h"
#include "InventorySystemComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemChanged, FInventorySlot, ItemSlotInfo, EInventorySlotChangeType, ChangeType);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FItemQuery, UItem*, Item);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UInventorySystemComponent : public UActorComponent
{
	GENERATED_BODY()


	// Owning actor of our component
	UPROPERTY()
	AActor* OwningActor;

	// Actor that our owning actor maybe controlling. Can be the same as our owning actor
	UPROPERTY()
	AActor* AvatarActor;
	
protected:
	
	// Our Array of Items the component currently has
	UPROPERTY(BlueprintReadOnly, Category = "Inventory System Component | Items")
	TArray<FInventorySlot> InventoryItems;

	/** Maximum size of our components inventory **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory System Component")
	int32 InventoryLimit;
	
	/* Array of Item Classes to grant our Component when initialized
	adding one or more of the same class will add a new item or update the existing
	stack count of an item based on its stackable setting
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory System Component | Defaults")
	TArray<FInventorySlot> DefaultInventoryItemData;

	UPROPERTY(BlueprintAssignable)
	FOnItemChanged OnItemChanged;

public:

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component")
	AActor* GetOwningActor() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component")
	AActor* GetAvatarActor() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Items")
	bool AddInventoryItem(UItem* Item, int StackCount = 1);

private:

	UFUNCTION()
	void AddInventoryItemSlot(FInventorySlot& InventorySlot);

public:

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Items")
	bool TryRemoveItem(FGuid FGuid, int32 StacksToRemove = 1);

	/** Initializes our Default Inventory Items
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Defaults")
	void InitInventorySystemComponent();

	/** Getter for our Inventory Items Array
	 * @return TArray<UItem*> : Returns our Inventory Items
	 */
	UFUNCTION(BlueprintCallable)
	bool GetInventoryItems(FGameplayTag Type, TArray<FInventorySlot>& OutInventorySlots) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component")
	bool GetItemById(const FGuid Guid, FInventorySlot& OutItem) const;

private:

	bool GetOpenInventorySlotsFromItem(const UItem* Item, TArray<FInventorySlot*>& InventorySlots);

public:

	UFUNCTION()
	FOnItemChanged& GetOnItemChangedDelegate();

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Initialize")
	void InitActorInfo(AActor* InOwningActor, AActor* InAvatarActor);

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component")
	void SetInventoryLimit(int NewLimitSize, bool bDropItemsOverLimit = false);

protected:

	int GetOpenInventorySlotAmount() const;
	
	bool IsInventoryFull() const;
};
