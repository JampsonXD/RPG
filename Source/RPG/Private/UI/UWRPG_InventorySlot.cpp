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

void UUWRPG_InventorySlot::SetupItemSlot(UItem* InItem)
{
	Item = InItem;
	Item->GetOnCurrentStackCountChangedDelegate().AddUniqueDynamic(this, &UUWRPG_InventorySlot::StackCountChanged);
	K2_SetupItemSlot(InItem);
}

void UUWRPG_InventorySlot::StackCountChanged(int OldStackCount, int NewStackCount)
{
	K2_StackCountChanged(OldStackCount, NewStackCount);
}
