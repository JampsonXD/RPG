// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UWRPG_InventorySlot.h"
#include "InventorySystemComponent.h"


void UUWRPG_InventorySlot::BeginDestroy()
{
	if(InventorySystemComponent && InventoryItem)
	{
		InventorySystemComponent->RegisterItemStackCountChangedEvent(InventoryItem).RemoveAll(this);
	}

	Super::BeginDestroy();
}

void UUWRPG_InventorySlot::SetupItemSlot(UItem* InItemSlot, UInventorySystemComponent* ISC)
{
	SetItem(InItemSlot);
	SetInventorySystemComponent(ISC);

	if(ISC)
	{
		ISC->RegisterItemStackCountChangedEvent(InItemSlot).AddDynamic(this, &UUWRPG_InventorySlot::StackCountChanged);
	}


	K2_SetupItemSlot(InItemSlot);
}

void UUWRPG_InventorySlot::CleanupItemSlot(UItem* Item, UInventorySystemComponent* ISC)
{
	if (InventorySystemComponent && InventoryItem)
	{
		InventorySystemComponent->RegisterItemStackCountChangedEvent(InventoryItem).RemoveAll(this);
	}
}

UItem* UUWRPG_InventorySlot::GetItem() const
{
	return InventoryItem;
}

UInventorySystemComponent* UUWRPG_InventorySlot::GetInventorySystemComponent() const
{
	return InventorySystemComponent;
}

void UUWRPG_InventorySlot::SetItem(UItem* Item)
{
	InventoryItem = Item;
}

void UUWRPG_InventorySlot::SetInventorySystemComponent(UInventorySystemComponent* ISC)
{
	InventorySystemComponent = ISC;
}

void UUWRPG_InventorySlot::StackCountChanged(int OldStackCount, int NewStackCount)
{
	K2_StackCountChanged(OldStackCount, NewStackCount);
}
