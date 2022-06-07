// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UWRPG_Inventory.h"
#include "InventorySystemComponent.h"
#include "UI/UWRPG_InventorySlot.h"
#include "Components/VerticalBox.h"


void UUWRPG_Inventory::SetupInventoryWidget(UInventorySystemComponent* ISC)
{
	SetInventorySystemComponent(ISC);

	ISC->GetOnItemAddedDelegate().AddDynamic(this, &UUWRPG_Inventory::AddItemToInventoryWidget);
	ISC->GetOnItemRemovedDelegate().AddDynamic(this, &UUWRPG_Inventory::RemoveItemFromInventoryWidget);

	// Any items that could be added prior to our inventory widget being setup will need to have item slots created for them
	TArray<UItem*> Items;
	if(ISC->GetInventoryItems(FGameplayTag::EmptyTag, Items))
	{
		for(UItem* Item : Items)
		{
			AddItemToInventoryWidget(Item);
		}
	}
}

void UUWRPG_Inventory::AddItemToInventoryWidget(UItem* NewItem)
{
	if(!NewItem)
	{
		return;
	}
	
	check(InventorySlotClass)
	UUWRPG_InventorySlot* ItemSlot = CreateWidget<UUWRPG_InventorySlot>(this, InventorySlotClass);
	ItemSlot->SetupItemSlot(NewItem, GetInventorySystemComponent());
	InventoryItemHolder->AddChild(ItemSlot);

	// Add our new item to our item slot map
	ItemInventorySlotMap.Add(NewItem, ItemSlot);
}

void UUWRPG_Inventory::RemoveItemFromInventoryWidget(UItem* OldItem)
{
	if(!ItemInventorySlotMap.Contains(OldItem))
	{
		return;
	}

	// Check if a key exists for the item in our inventory map and remove the item slot from our parent
	if(UUWRPG_InventorySlot** MapValue = ItemInventorySlotMap.Find(OldItem))
	{
		(*MapValue)->RemoveFromParent();
	}

	ItemInventorySlotMap.Remove(OldItem);
}

void UUWRPG_Inventory::BeginDestroy()
{
	if(UInventorySystemComponent* ISC = GetInventorySystemComponent())
	{
		ISC->GetOnItemAddedDelegate().RemoveDynamic(this, &UUWRPG_Inventory::AddItemToInventoryWidget);
		ISC->GetOnItemRemovedDelegate().RemoveDynamic(this, &UUWRPG_Inventory::RemoveItemFromInventoryWidget);
	}
	
	Super::BeginDestroy();
}
