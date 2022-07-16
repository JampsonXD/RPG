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

/* Additional information an item data contains that can modify state and is set in C++.
 * This is useful for things such as reserve ammo found in a weapon or the durability
 * of a weapon. Empty by default but should be sub-classed and passed down for
 * different items based on the needs.
 */

USTRUCT()
struct FInventoryItemData
{
	GENERATED_BODY()

	FInventoryItemData() {}
	virtual ~FInventoryItemData() {}
	virtual bool IsValid() { return false; }
};

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
		ItemData = nullptr;
	}

	FInventorySlot(UItem* InItem, int InStackCount = 1, FInventoryItemData* InItemData = nullptr)
	{
		Item = InItem;
		StackCount = InStackCount;
		Id = FGuid();
		ItemData = InItemData;
	}

	FORCEINLINE bool operator ==(const FInventorySlot& OtherSlot) const { return this->Id == OtherSlot.Id; }
	FORCEINLINE bool operator !=(const FInventorySlot& OtherSlot) const { return !(*this == OtherSlot); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItem* Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int StackCount;

	FInventoryItemData* ItemData;

	bool IsValid() const { return Item && StackCount > 0 && Id.IsValid();  }
	// Checks if we have a valid Item class and stacks, but does not check for our Id
	bool IsValidNonGenerated() const { return Item && StackCount > 0; }
	int GetMaxStackCount() const { return Item ? Item->GetMaxStackCount() : 0;  }
	int GetOpenStackCount() const { return GetMaxStackCount() - StackCount;  }
	bool IsFilledSlot() const { return StackCount >= GetMaxStackCount();  }
	void GenerateGuidId() { Id = FGuid::NewGuid(); }
	void InvalidateGuidId() { Id.Invalidate(); }
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