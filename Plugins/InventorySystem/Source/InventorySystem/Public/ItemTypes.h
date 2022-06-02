// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FItemUIData
{
	GENERATED_BODY()
	
	FItemUIData(FName Name, FText Description, UTexture2D* Image, bool bStackable, int32 CurrentStacks, int32 MaxStacks)
	{
		ItemName = Name;
		ItemDescription = Description;
		ItemImage = Image;
		bIsStackable = bStackable;
		CurrentStackCount = CurrentStacks;
		MaxStackCount = MaxStacks;
	}

	FItemUIData()
	{
		ItemName = TEXT("Default");
		ItemDescription = FText::GetEmpty();
		ItemImage = nullptr;
		bIsStackable = false;
		CurrentStackCount = -1;
		MaxStackCount = -1;
	}

	UPROPERTY(BlueprintReadOnly, Category = "Item | Info")
	FName ItemName;

	UPROPERTY(BlueprintReadOnly, Category = "Item | Info")
	FText ItemDescription;

	UPROPERTY(BlueprintReadOnly, Category = "Item | Info")
	class UTexture2D* ItemImage;

	// Whether this item can be stacked or not, defaults to false
	UPROPERTY(BlueprintReadOnly, Category = "Item")
	bool bIsStackable;

	// Current stack count of our item
	UPROPERTY(BlueprintReadOnly, Category = "Item")
	int32 CurrentStackCount;

	// If we can be stacked, our max stack count, defaults to -1 for if it is not stackable
	UPROPERTY(BlueprintReadOnly, Category = "Item")
	int32 MaxStackCount;
	
};