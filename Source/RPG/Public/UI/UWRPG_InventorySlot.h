// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemTypes.h"
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
	FInventorySlot InventorySlot;

	UPROPERTY()
	UInventorySystemComponent* InventorySystemComponent;

protected:

	UFUNCTION(BlueprintCallable, Category = "RPG UI | Inventory | Item Slot")
	FInventorySlot GetInventorySlot() const;

	UFUNCTION(BlueprintCallable, Category = "RPG UI | Inventory | Item Slot")
	UInventorySystemComponent* GetInventorySystemComponent() const;

	UFUNCTION()
	void SetInventorySlot(const FInventorySlot& InInventorySlot);

	UFUNCTION()
	void SetInventorySystemComponent(UInventorySystemComponent* ISC);

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG UI | Inventory | Item Slot", DisplayName = "SetupInventorySlot")
	void K2_SetupInventorySlot(const FInventorySlot& InInventorySlot);

public:

	UFUNCTION()
	void StackCountChanged(int& NewStackCount);

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG UI | Inventory | Item Slot", DisplayName = "StackCountChanged")
	void K2_StackCountChanged(const int& NewStackCount);

	virtual void BeginDestroy() override;

public:
	UFUNCTION()
	void SetupInventorySlot(FInventorySlot& InInventorySlot, UInventorySystemComponent* ISC);
	
};
