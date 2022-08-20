// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ArmorItem.h"
#include "RPG_Character.h"

UArmorItem::UArmorItem(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Armor items should not be stackable
	MaxStackCount = 1;
	ArmorMesh = nullptr;
	EquipSlot = EArmorEquipSlot::None;
	ItemType = FPrimaryAssetType("Armor");
}

USkeletalMesh* UArmorItem::GetArmorMesh() const
{
	return ArmorMesh;
}

EArmorEquipSlot UArmorItem::GetEquipSlot() const
{
	return EquipSlot;
}
