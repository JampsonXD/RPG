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

/* Additional information an item data contains that can modify state
 * This is useful for things such as reserve ammo found in a weapon or the durability
 * of a weapon. Empty by default but should be sub-classed and passed down for
 * different items based on the needs.
 */

UCLASS()
class UItemBonusData : public UObject
{
	GENERATED_BODY()
	
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
struct FInventorySlotData
{
	GENERATED_BODY()
	
	FInventorySlotData()
	{
		StackCount = -1;
		ItemData = nullptr;
	}

	FInventorySlotData(int InStackCount, UItemBonusData* InItemData = nullptr)
	{
		StackCount = InStackCount;
		ItemData = InItemData;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int StackCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItemBonusData* ItemData;

	bool IsValid() const { return StackCount > 0;  }
	bool operator==(FInventorySlotData& Other) const { return this->StackCount == Other.StackCount && this->ItemData == Other.ItemData; }
	bool operator!=(FInventorySlotData& Other) const { return !(*this == Other); }

	void UpdateSlotData(const FInventorySlotData& Other, int MaxCount)
	{
		if(MaxCount < 0)
		{
			MaxCount = INT16_MAX;
		}

		StackCount = FMath::Clamp(StackCount + Other.StackCount, 0, MaxCount);
	}
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