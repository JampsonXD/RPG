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

	bool bAddedItems = false;

	// Check if we have any open slots from current items to add our stacks to
	TArray<FInventorySlot*> OpenSlots;
	if(Item->IsStackable() && GetOpenInventorySlotsFromItem(Item, OpenSlots))
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
	if (IsInventoryFull())
	{
		return bAddedItems;
	}

	// We are going to create atleast one instance of an inventory slot so we can change to true
	bAddedItems = true;

	while (StackCount > 0 && !IsInventoryFull())
	{
		const int Stacks = FMath::Min(Item->GetMaxStackCount(), StackCount);
		FInventorySlot NewSlot = FInventorySlot(Item, Stacks);
		AddInventoryItemSlot(NewSlot);
		StackCount -= Stacks;
	}


	return bAddedItems;
}

bool UInventorySystemComponent::AddInventoryItemSlotFromSlotData(FInventorySlot& InventorySlot)
{
	// Don't try to add items if our inventory is already full
	if (IsInventoryFull())
	{
		return false;
	}


	AddInventoryItemSlot(InventorySlot);
	return true;
}

void UInventorySystemComponent::AddInventoryItemSlot(FInventorySlot& InventorySlot)
{
	InventorySlot.GenerateGuidId();
	InventoryItems.Add(InventorySlot);
	OnItemChanged.Broadcast(InventorySlot, EInventorySlotChangeType::Added);
}

void UInventorySystemComponent::RemoveInventoryItemSlot(FInventorySlot& InventorySlot)
{
	/* Remove the item from our equipped slot before removing the item from our inventory */
	FEquippedSlot EquippedSlot;
	if(IsItemEquipped(InventorySlot, EquippedSlot))
	{
		RemoveItemFromEquipmentSlot(EquippedSlot);
	}

	OnItemChanged.Broadcast(InventorySlot, EInventorySlotChangeType::Removed);
	InventoryItems.RemoveSingle(InventorySlot);
}

bool UInventorySystemComponent::RemoveItemStacks(FInventorySlot* InventorySlot, int StacksToRemove)
{
	if(!InventorySlot || !InventorySlot->IsValid())
	{
		return false;
	}

	InventorySlot->StackCount -= StacksToRemove;
	if (InventorySlot->StackCount <= 0)
	{
		RemoveInventoryItemSlot(*InventorySlot);
		return true;
	}

	OnItemChanged.Broadcast(*InventorySlot, EInventorySlotChangeType::StackChange);
	return true;
}

bool UInventorySystemComponent::TryRemoveItem(const FGuid FGuid, int32 StacksToRemove)
{
	if(FInventorySlot* Slot = Internal_GetItemById(FGuid))
	{
		return RemoveItemStacks(Slot, StacksToRemove);
	}

	return false;
}

void UInventorySystemComponent::InitInventorySystemComponent()
{
	/* If our inventory is not unlimited or 0, reserve space based on our max inventory size */
	if(InventoryLimit > 0)
	{
		InventoryItems.Reserve(InventoryLimit);
	}

	if(!DefaultEquipmentSlots.IsEmpty())
	{
		/* Loop through our map of slot types to slot amounts
		 * Add a new equipment slot for each slot up the total amount
		 */
		for(const TPair<FGameplayTag, int32>& Pair : DefaultEquipmentSlots)
		{
			for(int i = 0; i < Pair.Value; i++)
			{
				EquipmentMap.Add(FEquippedSlot(Pair.Key, i));
			}
		}
	}

	// Add Default Inventory Items to our Inventory
	for(const FDefaultInventoryData& InventorySlot : DefaultInventoryItemData)
	{
		FInventorySlot Slot = FInventorySlot(InventorySlot.Item, InventorySlot.StackCount);
		Slot.StackCount = FMath::Min(InventorySlot.Item->GetMaxStackCount(), InventorySlot.StackCount);
		AddInventoryItemSlot(Slot);

		/* Try equipping our item in the first open slot */
		if(InventorySlot.bEquipOnAdded)
		{
			FEquippedSlot EquippedSlot;
			AddInventoryItemToFirstOpenEquipmentSlot(Slot, EquippedSlot);
		}
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
		if(InventorySlot.Item->GetItemType().MatchesTag(Type))
		{
			OutInventorySlots.Add(InventorySlot);
		}
	}

	return OutInventorySlots.Num() > 0 ? true : false;
}

bool UInventorySystemComponent::GetItemById(const FGuid Guid, FInventorySlot& OutItem)
{
	if(const FInventorySlot* Slot = Internal_GetItemById(Guid))
	{
		OutItem = *Slot;
		return true;
	}

	OutItem = FInventorySlot();
	return false;
}

FInventorySlot* UInventorySystemComponent::Internal_GetItemById(const FGuid Guid)
{
	for (FInventorySlot& InventorySlot : InventoryItems)
	{
		if (Guid == InventorySlot.GetGuid())
		{
			return &InventorySlot;
		}
	}

	return nullptr;
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

bool UInventorySystemComponent::AddInventoryItemToFirstOpenEquipmentSlot(FInventorySlot& Slot, FEquippedSlot& OutEquipSlot)
{
	if(GetFirstAvailableEquipmentSlot(Slot.Item->GetItemType(), OutEquipSlot))
	{
		AddItemToEquipmentSlot(OutEquipSlot, Slot);
		return true;
	}

	return false;
}

bool UInventorySystemComponent::UseItemAtEquipmentSlot(const FEquippedSlot EquippedSlot)
{
	FInventorySlot* InventorySlot = GetItemAtEquipmentSlot(EquippedSlot);
	if(!InventorySlot)
	{
		return false;
	}

	OnEquipmentSlotUsed.Broadcast(*InventorySlot);
	if(InventorySlot->Item->ConsumeOnUse())
	{
		RemoveItemStacks(InventorySlot);
	}

	return true;
}

FInventorySlot* UInventorySystemComponent::GetItemAtEquipmentSlot(const FEquippedSlot& EquippedSlot)
{
	const FInventorySlot TempSlot = EquipmentMap.FindRef(EquippedSlot);
	return Internal_GetItemById(TempSlot.GetGuid());
}

bool UInventorySystemComponent::IsItemEquipped(const FInventorySlot& InventorySlot, FEquippedSlot& EquippedSlot)
{
	for(TPair<FEquippedSlot, FInventorySlot> Pair : EquipmentMap)
	{
		if(Pair.Value == InventorySlot)
		{
			EquippedSlot = Pair.Key;
			return true;
		}
	}

	EquippedSlot = FEquippedSlot();
	return false;
}

bool UInventorySystemComponent::GetFirstAvailableEquipmentSlot(FGameplayTag Type, FEquippedSlot& OutOpenSlot)
{
	if(!Type.IsValid())
	{
		return false;
	}

	for(TPair<FEquippedSlot, FInventorySlot> Slot : EquipmentMap)
	{
		FEquippedSlot& Key = Slot.Key;
		if(Type.MatchesTag(Key.SlotType) && !Slot.Value.IsValid())
		{
			OutOpenSlot = Slot.Key;
			return true;
		}
	}

	OutOpenSlot = FEquippedSlot();
	return false;
}

void UInventorySystemComponent::AddItemToEquipmentSlot(const FEquippedSlot& EquippedSlot, const FInventorySlot& InventorySlot)
{
	EquipmentMap.Add(EquippedSlot, InventorySlot);
	OnEquipmentSlotChanged.Broadcast(EquippedSlot, InventorySlot, EEquipmentSlotChangeType::Added);
}

void UInventorySystemComponent::RemoveItemFromEquipmentSlot(const FEquippedSlot& EquippedSlot)
{
	EquipmentMap.Add(EquippedSlot, FInventorySlot());
	OnEquipmentSlotChanged.Broadcast(EquippedSlot, FInventorySlot(), EEquipmentSlotChangeType::Removed);
}

int UInventorySystemComponent::GetOpenInventorySlotAmount() const
{
	return InventoryLimit - InventoryItems.Num();
}
