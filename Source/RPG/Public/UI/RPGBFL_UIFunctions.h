// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGBFL_UIFunctions.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGBFL_UIFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, Category = "RPG UI | Item Data")
	static FItemUIData MakeItemUIData(const FInventorySlot& InventorySlot);

	UFUNCTION(BlueprintCallable, Category = "RPGBFL Main Functions")
	static FItemUIData GetItemUIDataFromInventorySlotData(const UInventorySystemComponent* ISC, const FInventorySlot& InventorySlot);
};
