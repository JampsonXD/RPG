// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "InventorySystemComponent.h"

UItem::UItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxStackCount = -1;
	CurrentStackCount = 0;
}

UInventorySystemComponent* UItem::GetOwningInventorySystemComponent() const
{
	return OwningInventorySystemComponent;
}

AActor* UItem::GetOwningActor() const
{
	return OwningActor;
}

FGameplayTag UItem::GetItemType() const
{
	return ItemType;
}

FName UItem::GetItemName() const
{
	return ItemName;
}

FText UItem::GetItemDescription() const
{
	return ItemDescription;
}

UTexture2D* UItem::GetItemImage() const
{
	return ItemImageSoftPointer.LoadSynchronous();
}

bool UItem::CanUse() const
{
	return false;
}

void UItem::Use()
{
	
}

void UItem::Add(UInventorySystemComponent* InventorySystemComponent)
{
	SetOwningInventorySystemComponent(InventorySystemComponent);
	SetOwningActor(InventorySystemComponent->OwningActor);
}

void UItem::Remove()
{
	SetOwningInventorySystemComponent(nullptr);
	SetOwningActor(nullptr);
}

int UItem::GetCurrentStackCount() const
{
	return CurrentStackCount;
}

bool UItem::IsStackable() const
{
	return false;
}

int UItem::GetMaxStackCount() const
{
	return MaxStackCount;
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

void UItem::RemoveStacks(int32 StacksToRemove)
{
	// Verify we are a stackable item
	if(!IsStackable())
	{
		return;
	}

	const int32 OldStackCount = CurrentStackCount;

	// Only remove stacks down to a 0 count
	CurrentStackCount = FMath::Max(CurrentStackCount - StacksToRemove, 0);

	// Broadcast our stack count has changed
	if(CurrentStackCount != OldStackCount && OnCurrentStackCountChanged.IsBound())
	{
		OnCurrentStackCountChanged.Broadcast(OldStackCount, CurrentStackCount);
	}
}
