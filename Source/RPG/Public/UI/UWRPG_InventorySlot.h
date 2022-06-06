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


protected:
	
	UPROPERTY(BlueprintReadOnly)
	UItem* Item;

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG UI | Inventory | Item Slot", DisplayName = "SetupItemSlot")
	void K2_SetupItemSlot(UItem* InItem);

	UFUNCTION()
	void StackCountChanged(int OldStackCount, int NewStackCount);

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG UI | Inventory | Item Slot", DisplayName = "StackCountChanged")
	void K2_StackCountChanged(int OldStackCount, int NewStackCount);

	virtual void BeginDestroy() override;

public:
	void SetupItemSlot(UItem* InItem);
	
};
