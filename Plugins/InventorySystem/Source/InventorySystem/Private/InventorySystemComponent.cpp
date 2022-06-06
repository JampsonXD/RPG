// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystemComponent.h"


// Sets default values for this component's properties
UInventorySystemComponent::UInventorySystemComponent(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Initialize as Unlimited Inventory Size
	MaxInventorySize = -1;
	PrimaryComponentTick.bCanEverTick = true;
	OwningActor = GetOwner();
	AvatarActor = nullptr;
}


bool UInventorySystemComponent::AddItem(TSubclassOf<UItem> Item, int32 StacksToGrant)
{
	// Make sure we are actually adding an amount or have a valid Item Class
	if(StacksToGrant <= 0 || !IsValid(Item))
	{
		return false;
	}
	
	TArray<UItem*> ValidItems;
	bool bAddedItems = false;

	// Check if we are a stackable item and already have items of the same type
	if(Cast<UItem>(Item->GetDefaultObject())->IsStackable() && ContainsValidItemOfClass(Item,ValidItems))
	{
		bAddedItems = true;
		AddStacksToItems(ValidItems, StacksToGrant);
	}
	
	// We have no stacks to update left or we have no inventory space
	if(StacksToGrant <= 0 || IsInventoryFull())
	{
		return bAddedItems;
	}

	// We have space and are going to at least create one instance of UItem so change
	// our boolean to true
	bAddedItems = true;
	
	// Continue creating item instances from class until we have no stacks to give left or our inventory is full
	while(!IsInventoryFull() && StacksToGrant > 0)
	{
		// Create an Item Instance and Re-Evaluate if we have stacks leftover
		CreateItemInstance(Item, StacksToGrant);
	}
	
	return bAddedItems;
}

void UInventorySystemComponent::CreateItemInstance(TSubclassOf<UItem> Item, int32& StacksToGrant)
{
	// Create a new Item Instance
	UItem* NewItem = NewObject<UItem>(this, Item);
	InventoryItems.Add(NewItem);
	
	// Only add one stack if our item is not stackable
	if(!NewItem->IsStackable())
	{
		NewItem->AddStacks(1);
		StacksToGrant -= 1;
	}

	// Add all available stacks if our item is stackable
	if(NewItem->IsStackable())
	{
		AddAvailableStacksToItem(StacksToGrant, NewItem);
	}

	// Broadcast that we have a new item added to our Inventory
	if(OnItemAdded.IsBound())
	{
		OnItemAdded.Broadcast(NewItem);
	}
}

void UInventorySystemComponent::InitInventorySystemComponent()
{
	// Add Default Inventory Items to our Inventory
	for(const TSubclassOf<UItem> Item : DefaultInventoryItems)
	{
		AddItem(Item, 1);
	}
}

bool UInventorySystemComponent::GetInventoryItems(FGameplayTag Type, TArray<UItem*>& OutItems) const
{
	// Return our whole Inventory Items Array if we aren't filtering
	if(Type == FGameplayTag::EmptyTag)
	{
		OutItems.Append(InventoryItems);
		return true;
	}
	
	for(UItem* Item : InventoryItems)
	{
		if(Item->GetItemType().MatchesTag(Type))
		{
			OutItems.Add(Item);
		}
	}

	return OutItems.Num() > 0 ? true : false;
}

FOnItemAdded& UInventorySystemComponent::GetOnItemAddedDelegate()
{
	return OnItemAdded;
}

FOnItemRemoved& UInventorySystemComponent::GetOnItemRemovedDelegate()
{
	return OnItemRemoved;
}

bool UInventorySystemComponent::TryGetItemWithStackCount(int32 StackCount, UItem*& OutItem)
{
	return QueryInventoryForItem([StackCount](UItem* Item)
	{
		return Item->GetCurrentItemStacks() == StackCount;
	}, OutItem);
}

bool UInventorySystemComponent::QueryInventoryForItem(FItemQuery Query, UItem*& OutItem)
{
	for(UItem*& CurrentItem : InventoryItems)
	{
		if(Query.Execute(CurrentItem))
		{
			OutItem = CurrentItem;
			return true;
		}
	}

	return false;
}

bool UInventorySystemComponent::QueryInventoryForItems(FItemQuery Query, TArray<UItem*>& OutItems)
{
	TArray<UItem*> MatchingItems;
	for(UItem*& CurrentItem : InventoryItems)
	{
		if(Query.Execute(CurrentItem))
		{
			MatchingItems.Add(CurrentItem);
		}
	}

	OutItems.Append(MatchingItems);
	return !MatchingItems.IsEmpty();
}

bool UInventorySystemComponent::QueryInventoryForItem(TFunction<bool(UItem* Item)> Query, UItem*& OutItem)
{
	for(UItem*& CurrentItem : InventoryItems)
	{
		if(Query(CurrentItem))
		{
			OutItem = CurrentItem;
			return true;
		}
	}

	OutItem = nullptr;
	return false;
}

bool UInventorySystemComponent::TryUsingInventoryItem(UItem* Item)
{
	if(!Item)
	{
		return false;
	}
	
	return Item->Use(this, AvatarActor);
}

void UInventorySystemComponent::InitActorInfo(AActor* InOwningActor, AActor* InAvatarActor)
{
	OwningActor = InOwningActor;
	AvatarActor = InAvatarActor;
}

bool UInventorySystemComponent::ContainsValidItemOfClass(const TSubclassOf<UItem>& Item, TArray<UItem*>& OutItemsFound)
{
	for(UItem* InventoryItem : InventoryItems)
	{
		// Check if are InventoryItem is of the same class / is a stackable item / its current stacks are less than th max stacks allowed or it has unlimited stacks
		if(InventoryItem->IsA(Item) && InventoryItem->IsStackable() && (GetAvailableStackCount(InventoryItem) > 0 || InventoryItem->HasUnlimitedStacks()))
		{
			OutItemsFound.Add(InventoryItem);
		}
	}

	// If our items found is greater than 0 return true
	return OutItemsFound.Num() > 0;
}

void UInventorySystemComponent::AddStacksToItems(TArray<UItem*>& Items, int32& StacksToGrant)
{
	// Iterate through our Items of the same type until we have no stacks to give or run out of items to update 
	for(UItem* Item : Items)
	{
		// Returns true if we have no stacks leftover
		if (AddAvailableStacksToItem(StacksToGrant, Item)) return;
	}
}

bool UInventorySystemComponent::AddAvailableStacksToItem(int32& StacksToGrant, UItem* Item)
{
	const int32 AvailableStacks = GetAvailableStackCount(Item);

	if(StacksToGrant - AvailableStacks <= 0 || Item->HasUnlimitedStacks())
	{
		Item->AddStacks(StacksToGrant);
		StacksToGrant = 0;
		return true;
	}

	Item->AddStacks(AvailableStacks);
	StacksToGrant -= AvailableStacks;
	return false;
}

int32 UInventorySystemComponent::GetAvailableStackCount(UItem* Item)
{
	return Item->GetMaxItemStacks() - Item->GetCurrentItemStacks();
}

bool UInventorySystemComponent::HasUnlimitedInventorySize() const
{
	return MaxInventorySize == -1;
}

bool UInventorySystemComponent::IsInventoryFull() const
{
	return InventoryItems.Num() >= MaxInventorySize && !HasUnlimitedInventorySize();
}
