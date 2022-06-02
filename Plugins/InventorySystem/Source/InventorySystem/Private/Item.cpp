// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "ItemTypes.h"

UItem::UItem(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxStackCount = -1;
	CurrentStackCount = 0;
	ItemImage = nullptr;
}

bool UItem::Use_Implementation(UInventorySystemComponent* TargetInventorySystemComponent, AActor* TargetActor)
{
	return true;
}

bool UItem::CanUseItem_Implementation(UInventorySystemComponent* TargetInventorySystemComponent, AActor* TargetActor)
{
	return true;
}

bool UItem::IsStackable()
{
	return MaxStackCount != 1;
}

bool UItem::HasUnlimitedStacks()
{
	return MaxStackCount == -1;
}

int32 UItem::GetMaxItemStacks()
{
	return MaxStackCount;
}

int32 UItem::GetCurrentItemStacks()
{
	return CurrentStackCount;
}

FGameplayTag UItem::GetItemType()
{
	return ItemType;
}

FItemUIData UItem::MakeItemData()
{
	FItemUIData ItemData = FItemUIData(ItemName, ItemDescription, ItemImage, IsStackable(), CurrentStackCount, MaxStackCount);
	return ItemData;
}

FOnCurrentStackCountChanged& UItem::GetOnCurrentStackCountChangedDelegate()
{
	return OnCurrentStackCountChanged;
}

void UItem::AddStacks(int32 StacksToAdd)
{
	const int32 OldStackCount = CurrentStackCount;
	
	// Verify we dont go over our max stack count
	CurrentStackCount = HasUnlimitedStacks() ? CurrentStackCount + StacksToAdd : FMath::Min(CurrentStackCount + StacksToAdd, MaxStackCount);

	// Broadcast that our stack count has changed
	if(CurrentStackCount != OldStackCount && OnCurrentStackCountChanged.IsBound())
	{
		OnCurrentStackCountChanged.Broadcast(OldStackCount, CurrentStackCount);
	}
}
