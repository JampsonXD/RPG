// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "UWRPG_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UUWRPG_Inventory : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "RPG UI | Inventory")
	TSubclassOf<class UUWRPG_InventorySlot> InventorySlotClass;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* InventoryItemHolder;

	/* Map for getting the inventory slot an item references */
	UPROPERTY()
	TMap<UItem*, UUWRPG_InventorySlot*> ItemInventorySlotMap;

	UPROPERTY(BlueprintReadOnly)
	UInventorySystemComponent* InventorySystemComponent;

	void SetInventorySystemComponent(UInventorySystemComponent* ISC) { InventorySystemComponent = ISC; }

	UInventorySystemComponent* GetInventorySystemComponent() const { return InventorySystemComponent; }

	UFUNCTION(BlueprintCallable, Category = "RPG UI | Inventory")
	void AddItemToInventoryWidget(UItem* NewItem);

	UFUNCTION()
	void RemoveItemFromInventoryWidget(UItem* OldItem);

	virtual void BeginDestroy() override;
	
public:

	UFUNCTION(BlueprintCallable, Category = "RPG UI | Inventory")
	void SetupInventoryWidget(UInventorySystemComponent* ISC);
};
