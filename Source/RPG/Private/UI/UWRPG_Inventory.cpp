// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UWRPG_Inventory.h"
#include "InventorySystemComponent.h"
#include "UI/UWRPG_InventorySlot.h"
#include "Components/VerticalBox.h"


void UUWRPG_Inventory::SetupInventoryWidget(UInventorySystemComponent* ISC)
{
	SetInventorySystemComponent(ISC);

	ISC->GetOnItemChangedDelegate().AddDynamic(this, &UUWRPG_Inventory::OnInventoryItemChanged);

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

void UUWRPG_Inventory::OnInventoryItemChanged(UItem* Item, EInventorySlotChangeType ChangeType)
{
	switch (ChangeType)
	{
		case EInventorySlotChangeType::Added :
			AddItemToInventoryWidget(Item);
			break;

		case EInventorySlotChangeType::Removed :
			RemoveItemFromInventoryWidget(Item);
			break;

		default: break;
	}
}

void UUWRPG_Inventory::AddItemToInventoryWidget(UItem* Item)
{
	if(!Item)
	{
		return;
	}
	
	check(InventorySlotClass)
	UUWRPG_InventorySlot* ItemSlot = CreateWidget<UUWRPG_InventorySlot>(this, InventorySlotClass);
	ItemSlot->SetupItemSlot(Item, GetInventorySystemComponent());
	InventoryItemHolder->AddChild(ItemSlot);

	// Add our new item to our item slot map
	ItemInventorySlotMap.Add(Item, ItemSlot);
}

void UUWRPG_Inventory::RemoveItemFromInventoryWidget(UItem* Item)
{
	if(UUWRPG_InventorySlot* Widget = GetMapValueFromInventorySlot(Item))
	{
		Widget->CleanupItemSlot(Item, GetInventorySystemComponent());
		Widget->RemoveFromParent();
		ItemInventorySlotMap.Remove(Item);
	}
}

UUWRPG_InventorySlot* UUWRPG_Inventory::GetMapValueFromInventorySlot(const UItem* Item)
{
	if (!ItemInventorySlotMap.Contains(Item))
	{
		return nullptr;
	}

	// Check if a key exists for the item in our inventory map and remove the item slot from our parent
	if (UUWRPG_InventorySlot** MapValue = ItemInventorySlotMap.Find(Item))
	{
		return *(MapValue);
	}

	return nullptr;
}

void UUWRPG_Inventory::BeginDestroy()
{
	if(UInventorySystemComponent* ISC = GetInventorySystemComponent())
	{
		ISC->GetOnItemChangedDelegate().RemoveDynamic(this, &UUWRPG_Inventory::OnInventoryItemChanged);
	}
	
	Super::BeginDestroy();
}
