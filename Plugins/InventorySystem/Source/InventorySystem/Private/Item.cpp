// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

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

bool UItem::IsStackable() const
{
	return bIsStackable;
}

int UItem::GetMaxStackCount() const
{
	return MaxStackCount;
}

bool UItem::ConsumeOnUse() const
{
	return bConsumeOnUse;
}
