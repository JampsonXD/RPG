// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystemComponent.h"

AActor* UInventorySystemComponent::GetOwningActor() const
{
	return OwningActor;
}

AActor* UInventorySystemComponent::GetAvatarActor() const
{
	return AvatarActor;
}

bool UInventorySystemComponent::AddInventoryItem(UItem* Item, int StackCount)
{
	if(!Item || StackCount <= 0)
	{
		return false;
	}

	const bool bIsStackable = Item->IsStackable();
	bool bAddedItems = false;

	// Check if we have any open slots from current items to add our stacks to
	TArray<FInventorySlot*> OpenSlots;
	if(bIsStackable && GetOpenInventorySlotsFromItem(Item, OpenSlots))
	{
		bAddedItems = true;
		for(FInventorySlot*& InventorySlot : OpenSlots)
		{
			const int StacksToAdd = FMath::Min(InventorySlot->GetOpenStackCount(), StackCount);
			InventorySlot->StackCount += StacksToAdd;
			OnItemChanged.Broadcast(*InventorySlot, EInventorySlotChangeType::StackChange);
			StackCount -= StacksToAdd;

			if(StackCount <= 0)
			{
				return bAddedItems;
			}
		}
	}

	// Don't try to add items if our inventory is already full
	if(IsInventoryFull())
	{
		return bAddedItems;
	}

	// We are going to create atleast one instance of an inventory slot so we can change to true
	bAddedItems = true;

	while(StackCount > 0 && !IsInventoryFull())
	{
		const int Stacks = FMath::Min(Item->GetMaxStackCount(), StackCount);
		FInventorySlot NewSlot = FInventorySlot(Item, Stacks);
		AddInventoryItemSlot(NewSlot);
		StackCount -= Stacks;
	}
	

	return bAddedItems;
}

void UInventorySystemComponent::AddInventoryItemSlot(FInventorySlot& InventorySlot)
{
	InventorySlot.GenerateGuidId();
	InventoryItems.Add(InventorySlot);
	OnItemChanged.Broadcast(InventorySlot, EInventorySlotChangeType::Added);
}

bool UInventorySystemComponent::TryRemoveItem(const FGuid FGuid, int32 StacksToRemove)
{
	FInventorySlot InventorySlot;
	if(!GetItemById(FGuid, InventorySlot))
	{
		return false;
	}

	InventorySlot.StackCount -= StacksToRemove;
	if(InventorySlot.StackCount <= 0)
	{
		OnItemChanged.Broadcast(InventorySlot, EInventorySlotChangeType::Removed);
		InventoryItems.Remove(InventorySlot);
		return true;
	}

	OnItemChanged.Broadcast(InventorySlot, EInventorySlotChangeType::StackChange);
	return true;
}

void UInventorySystemComponent::InitInventorySystemComponent()
{
	/* If our inventory is not unlimited or 0, reserve space based on our max inventory size */
	if(InventoryLimit > 0)
	{
		InventoryItems.Reserve(InventoryLimit);
	}

	// Add Default Inventory Items to our Inventory
	for(FInventorySlot InventorySlot : DefaultInventoryItemData)
	{
		InventorySlot.StackCount = FMath::Min(InventorySlot.Item->GetMaxStackCount(), InventorySlot.StackCount);
		AddInventoryItemSlot(InventorySlot);
	}

	// Set our Inventory Size Limit after, letting us drop items if we are above the max
	SetInventoryLimit(InventoryLimit, true);
}

bool UInventorySystemComponent::GetInventoryItems(FGameplayTag Type, TArray<FInventorySlot>& OutInventorySlots) const
{
	// Return our whole Inventory Items Array if we aren't filtering
	if(Type == FGameplayTag::EmptyTag)
	{
		OutInventorySlots.Append(InventoryItems);
		return true;
	}
	
	for(const FInventorySlot& InventorySlot : InventoryItems)
	{
		if(InventorySlot.Item->GetItemType() == Type)
		{
			OutInventorySlots.Add(InventorySlot);
		}
	}

	return OutInventorySlots.Num() > 0 ? true : false;
}

bool UInventorySystemComponent::GetItemById(const FGuid Guid, FInventorySlot& OutItem) const
{
	for (const FInventorySlot& InventorySlot : InventoryItems)
	{
		if (Guid == InventorySlot.GetGuid())
		{
			OutItem = InventorySlot;
			return true;
		}
	}

	OutItem = FInventorySlot();
	return false;
}

bool UInventorySystemComponent::GetOpenInventorySlotsFromItem(const UItem* Item, TArray<FInventorySlot*>& InventorySlots)
{
	for(FInventorySlot& InventorySlot : InventoryItems)
	{
		if(Item == InventorySlot.Item && !InventorySlot.IsFilledSlot())
		{
			InventorySlots.Add(&InventorySlot);
		}
	}

	return InventorySlots.Num() > 0;
}

FOnItemChanged& UInventorySystemComponent::GetOnItemChangedDelegate()
{
	return OnItemChanged;
}

void UInventorySystemComponent::InitActorInfo(AActor* InOwningActor, AActor* InAvatarActor)
{
	OwningActor = InOwningActor;
	AvatarActor = InAvatarActor;
}

void UInventorySystemComponent::SetInventoryLimit(int NewLimitSize, bool bDropItemsOverLimit)
{
	const int OldLimitSize = InventoryLimit;
	InventoryLimit = NewLimitSize;

	for(int i = InventoryItems.Num(); i > InventoryLimit; i--)
	{
		OnItemChanged.Broadcast(InventoryItems[i], EInventorySlotChangeType::Removed);
		InventoryItems.RemoveAtSwap(i);
	}
}

bool UInventorySystemComponent::IsInventoryFull() const
{
	return InventoryItems.Num() >= InventoryLimit;
}

int UInventorySystemComponent::GetOpenInventorySlotAmount() const
{
	return InventoryLimit - InventoryItems.Num();
}