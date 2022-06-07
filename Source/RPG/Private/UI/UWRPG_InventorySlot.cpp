// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UWRPG_InventorySlot.h"


void UUWRPG_InventorySlot::BeginDestroy()
{
	if(Item)
	{
		Item->GetOnCurrentStackCountChangedDelegate().RemoveDynamic(this, &UUWRPG_InventorySlot::StackCountChanged);
	}
	
	Super::BeginDestroy();
}

void UUWRPG_InventorySlot::SetupItemSlot(UItem* InItem, UInventorySystemComponent* ISC)
{
	SetItem(InItem);
	SetInventorySystemComponent(ISC);
	InItem->GetOnCurrentStackCountChangedDelegate().AddUniqueDynamic(this, &UUWRPG_InventorySlot::StackCountChanged);
	K2_SetupItemSlot(InItem);
}

UItem* UUWRPG_InventorySlot::GetItem() const
{
	return Item;
}

UInventorySystemComponent* UUWRPG_InventorySlot::GetInventorySystemComponent() const
{
	return InventorySystemComponent;
}

void UUWRPG_InventorySlot::SetItem(UItem* const InItem)
{
	Item = InItem;
}

void UUWRPG_InventorySlot::SetInventorySystemComponent(UInventorySystemComponent* ISC)
{
	InventorySystemComponent = ISC;
}

void UUWRPG_InventorySlot::StackCountChanged(int OldStackCount, int NewStackCount)
{
	K2_StackCountChanged(OldStackCount, NewStackCount);
}
