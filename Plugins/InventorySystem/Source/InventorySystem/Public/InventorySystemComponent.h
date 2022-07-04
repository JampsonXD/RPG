// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemTypes.h"
#include "Components/ActorComponent.h"
#include "InventorySystemComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemChanged, FInventorySlot, ItemSlotInfo, EInventorySlotChangeType, ChangeType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEquipmentSlotChanged, FEquippedSlot, EquippedSlotData, FInventorySlot, InventorySlot, EEquipmentSlotChangeType, ChangeType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipmentSlotUsed, FInventorySlot, InventorySlot);
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
	TArray<FDefaultInventoryData> DefaultInventoryItemData;

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
	bool AddInventoryItemSlotFromSlotData(FInventorySlot& InventorySlot);

	UFUNCTION()
	void AddInventoryItemSlot(FInventorySlot& InventorySlot);

	UFUNCTION()
	void RemoveInventoryItemSlot(FInventorySlot& InventorySlot);

	bool RemoveItemStacks(FInventorySlot* InventorySlot, int StacksToRemove = 1);

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
	bool GetItemById(const FGuid Guid, FInventorySlot& OutItem);

	FInventorySlot* Internal_GetItemById(const FGuid Guid);

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


	/**********************************************************
	 ***                  Equipment Slots                  ****
	 *********************************************************/

protected:

	// Maps equipped slot data to an inventory slot
	UPROPERTY(BlueprintReadOnly, Category = "Inventory System Component | Equipment")
	TMap<FEquippedSlot, FInventorySlot> EquipmentMap;

	UPROPERTY(BlueprintAssignable)
	FOnEquipmentSlotChanged OnEquipmentSlotChanged;

	UPROPERTY(BlueprintAssignable)
	FOnEquipmentSlotUsed OnEquipmentSlotUsed;

	/* Default map of item slot types and how many slots to apply for each type when our component is initialized */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory System Component | Equipment")
	TMap<FGameplayTag, int32> DefaultEquipmentSlots;

public:

	/**
	 * Tries adding an Inventory Slot to the first available equipment slot
	 * @param  Slot : The inventory slot we are trying to add
	 * @param OutEquipSlot : The equipment slot we were added to, returns a default equipment slot structure if not available
	 * @return bool : Whether we succeeded at adding the item to an equipment slot
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Equipment")
	bool AddInventoryItemToFirstOpenEquipmentSlot(UPARAM(ref) FInventorySlot& Slot, FEquippedSlot& OutEquipSlot);

	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Equipment")
	bool UseItemAtEquipmentSlot(const FEquippedSlot EquippedSlot);

protected:

	FInventorySlot* GetItemAtEquipmentSlot(const FEquippedSlot& EquippedSlot);

	UFUNCTION()
	bool IsItemEquipped(const FInventorySlot& InventorySlot, FEquippedSlot& EquippedSlot);

	UFUNCTION()
	bool GetFirstAvailableEquipmentSlot(FGameplayTag Type, FEquippedSlot& OutOpenSlot);

	UFUNCTION()
	void AddItemToEquipmentSlot(const FEquippedSlot& EquippedSlot, const FInventorySlot& InventorySlot);

	UFUNCTION()
	void RemoveItemFromEquipmentSlot(const FEquippedSlot& EquippedSlot);
};
