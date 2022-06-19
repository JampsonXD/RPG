// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RPGBFL_UIFunctions.h"

FItemUIData URPGBFL_UIFunctions::MakeItemUIData(const UItem* Item)
{
	FItemUIData ItemData = FItemUIData();

	ItemData.CurrentStackCount = Item->GetCurrentStackCount();
	ItemData.MaxStackCount = Item->GetMaxStackCount();
	ItemData.ItemName = Item->GetItemName();
	ItemData.ItemImage = Item->GetItemImage();
	ItemData.ItemDescription = Item->GetItemDescription();
	ItemData.bIsStackable = Item->IsStackable();

	return ItemData;
}
