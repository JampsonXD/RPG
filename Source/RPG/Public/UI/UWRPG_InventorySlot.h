// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Blueprint/UserWidget.h"
#include "UWRPG_InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UUWRPG_InventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY()
	UItem* InventoryItem;

	UPROPERTY()
	UInventorySystemComponent* InventorySystemComponent;

protected:

	UFUNCTION(BlueprintCallable, Category = "RPG UI | Inventory | Item Slot")
	UItem* GetItem() const;

	UFUNCTION(BlueprintCallable, Category = "RPG UI | Inventory | Item Slot")
	UInventorySystemComponent* GetInventorySystemComponent() const;

	UFUNCTION()
	void SetItem(UItem* Item);

	UFUNCTION()
	void SetInventorySystemComponent(UInventorySystemComponent* ISC);

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG UI | Inventory | Item Slot", DisplayName = "SetupItemSlot")
	void K2_SetupItemSlot(const UItem* Item);

public:

	UFUNCTION()
	void StackCountChanged(int OldStackCount, int NewStackCount);

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG UI | Inventory | Item Slot", DisplayName = "StackCountChanged")
	void K2_StackCountChanged(int OldStackCount, int NewStackCount);

	virtual void BeginDestroy() override;

public:
	UFUNCTION()
	void SetupItemSlot(UItem* InItemSlot, UInventorySystemComponent* ISC);

	UFUNCTION()
	void CleanupItemSlot(UItem* Item, UInventorySystemComponent* ISC);
	
};
