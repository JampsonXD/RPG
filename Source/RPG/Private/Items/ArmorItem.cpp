// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ArmorItem.h"
#include "RPG_Character.h"

UArmorItem::UArmorItem(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Armor items should not be stackable
	MaxStackCount = 1;
	ArmorMesh = nullptr;
	EquipSlot = EArmorEquipSlot::None;
}

bool UArmorItem::Use_Implementation(UInventorySystemComponent* TargetInventorySystemComponent, AActor* TargetActor)
{
	ARPG_Character* TargetCharacter =  Cast<ARPG_Character>(TargetActor);
	if(!TargetCharacter)
	{
		return false;
	}

	return TargetCharacter->TryEquipArmor(this);
}

USkeletalMesh* UArmorItem::GetArmorMesh() const
{
	return ArmorMesh;
}

EArmorEquipSlot UArmorItem::GetEquipSlot() const
{
	return EquipSlot;
}
