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
	TArray<FInventorySlot> InventorySlots;
	if(ISC->GetInventoryItems(FGameplayTag::EmptyTag, InventorySlots))
	{
		for(FInventorySlot InventorySlot : InventorySlots)
		{
			AddItemToInventoryWidget(InventorySlot);
		}
	}
}

void UUWRPG_Inventory::OnInventoryItemChanged(FInventorySlot InventorySlot, EInventorySlotChangeType ChangeType)
{
	switch (ChangeType)
	{
		case EInventorySlotChangeType::Added :
			AddItemToInventoryWidget(InventorySlot);
			break;

		case EInventorySlotChangeType::Removed :
			RemoveItemFromInventoryWidget(InventorySlot);
			break;

		case EInventorySlotChangeType::StackChange :
			UpdateInventorySlotStackCount(InventorySlot);
			break;

		default: break;
	}
}

void UUWRPG_Inventory::AddItemToInventoryWidget(FInventorySlot& InventorySlot)
{
	if(!InventorySlot.IsValid())
	{
		return;
	}
	
	check(InventorySlotClass)
	UUWRPG_InventorySlot* ItemSlot = CreateWidget<UUWRPG_InventorySlot>(this, InventorySlotClass);
	ItemSlot->SetupInventorySlot(InventorySlot, GetInventorySystemComponent());
	InventoryItemHolder->AddChild(ItemSlot);

	// Add our new item to our item slot map
	ItemInventorySlotMap.Add(InventorySlot.GetGuid(), ItemSlot);
}

void UUWRPG_Inventory::RemoveItemFromInventoryWidget(const FInventorySlot& InventorySlot)
{
	if(UUWRPG_InventorySlot* Widget = GetMapValueFromInventorySlot(InventorySlot))
	{
		Widget->RemoveFromParent();
		ItemInventorySlotMap.Remove(InventorySlot.GetGuid());
	}
}

void UUWRPG_Inventory::UpdateInventorySlotStackCount(FInventorySlot& InventorySlot)
{
	if (UUWRPG_InventorySlot* Widget = GetMapValueFromInventorySlot(InventorySlot))
	{
		Widget->StackCountChanged(InventorySlot.StackCount);
	}
}

UUWRPG_InventorySlot* UUWRPG_Inventory::GetMapValueFromInventorySlot(const FInventorySlot& InventorySlot)
{
	if (!ItemInventorySlotMap.Contains(InventorySlot.GetGuid()))
	{
		return nullptr;
	}

	// Check if a key exists for the item in our inventory map and remove the item slot from our parent
	if (UUWRPG_InventorySlot** MapValue = ItemInventorySlotMap.Find(InventorySlot.GetGuid()))
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
