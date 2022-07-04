// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RPGBFL_UIFunctions.h"

#include "InventorySystemComponent.h"

FItemUIData URPGBFL_UIFunctions::MakeItemUIData(const FInventorySlot& InventorySlot)
{
	if(!InventorySlot.IsValid())
	{
		return FItemUIData();
	}

	FItemUIData ItemData = FItemUIData();

	ItemData.CurrentStackCount = InventorySlot.StackCount;
	ItemData.MaxStackCount = InventorySlot.Item->GetMaxStackCount();
	ItemData.ItemName = InventorySlot.Item->GetItemName();
	ItemData.ItemImage = InventorySlot.Item->GetItemImage();
	ItemData.ItemDescription = InventorySlot.Item->GetItemDescription();
	ItemData.bIsStackable = InventorySlot.Item->IsStackable();

	return ItemData;
}

FItemUIData URPGBFL_UIFunctions::GetItemUIDataFromInventorySlotData(UInventorySystemComponent* ISC,
	FInventorySlot& InventorySlot)
{
	FInventorySlot TempSlot;
	ISC->GetItemById(InventorySlot.GetGuid(), TempSlot);

	return MakeItemUIData(TempSlot);
}
