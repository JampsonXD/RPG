// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemTypes.generated.h"

class UInventorySystemComponent;
class UItem;

/**
 * 
 */

USTRUCT(BlueprintType)
struct FEquippedSlot
{
	GENERATED_BODY()

	bool operator==(const FEquippedSlot& Other) const
	{
		return SlotType.MatchesTagExact(Other.SlotType) && SlotNumber == Other.SlotNumber;
	}

	FEquippedSlot()
	{
		SlotType = FGameplayTag::EmptyTag;
		SlotNumber = -1;
	}

	FEquippedSlot(FGameplayTag Tag, int InSlotNumber)
	{
		SlotType = Tag;
		SlotNumber = InSlotNumber;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag SlotType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SlotNumber;

	friend inline uint32 GetTypeHash(const FEquippedSlot& Key)
	{
		uint32 Hash = 0;
		Hash = HashCombine(Hash, GetTypeHash(Key.SlotType));
		Hash = HashCombine(Hash, static_cast<uint32>(Key.SlotNumber));
		return Hash;
	}

	bool IsValid() const
	{
		return SlotType.IsValid() && SlotNumber >= 0;
	}
};

USTRUCT(BlueprintType)
struct FDefaultInventoryData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItem* Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int StackCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEquipOnAdded;
};

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()
	
	FInventorySlot()
	{
		Item = nullptr;
		StackCount = -1;
		Id = FGuid();
	}

	FInventorySlot(UItem* InItem, int InStackCount = 1)
	{
		Item = InItem;
		StackCount = InStackCount;
		Id = FGuid();
	}

	FORCEINLINE bool operator ==(const FInventorySlot& OtherSlot) const { return this->Id == OtherSlot.Id; }
	FORCEINLINE bool operator !=(const FInventorySlot& OtherSlot) const { return !(*this == OtherSlot); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItem* Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int StackCount;

	bool IsValid() const { return Item && StackCount > 0 && Id.IsValid();  }
	// Checks if we have a valid Item class and stacks, but does not check for our Id
	bool IsValidNonGenerated() const { return Item && StackCount > 0; }
	int GetMaxStackCount() const { return Item ? Item->GetMaxStackCount() : 0;  }
	int GetOpenStackCount() const { return GetMaxStackCount() - StackCount;  }
	bool IsFilledSlot() const { return StackCount >= GetMaxStackCount();  }
	void GenerateGuidId() { Id = FGuid::NewGuid(); }
	FGuid GetGuid() const { return Id; }

	private:
	UPROPERTY()
	FGuid Id;
};

UENUM(BlueprintType)
enum class EInventorySlotChangeType : uint8
{
	None,
	Removed,
	Added,
	StackChange
};

UENUM(BlueprintType)
enum class EEquipmentSlotChangeType : uint8
{
	None,
	Added,
	Removed
};

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
	UTexture2D* ItemImage;

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