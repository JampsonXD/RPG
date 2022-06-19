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

bool UArmorItem::CanUse() const
{
	return K2_CanUse();
}

void UArmorItem::Use()
{
	ARPG_Character* TargetCharacter = GetOwningInventorySystemComponent() ? Cast<ARPG_Character>(GetOwningInventorySystemComponent()->GetAvatarActor()) : nullptr;

	if (!TargetCharacter)
	{
		return;
	}

	TargetCharacter->IsEquipped(this) ? TargetCharacter->UnequipArmor(this) : TargetCharacter->TryEquipArmor(this);
	K2_Use(GetOwningInventorySystemComponent(), GetOwningActor());
}

void UArmorItem::Remove()
{
	ARPG_Character* TargetCharacter = GetOwningInventorySystemComponent() ? Cast<ARPG_Character>(GetOwningInventorySystemComponent()->GetAvatarActor()) : nullptr;

	if (!TargetCharacter)
	{
		return;
	}

	TargetCharacter->UnequipArmor(this);
	K2_Use(GetOwningInventorySystemComponent(), GetOwningActor());
}

USkeletalMesh* UArmorItem::GetArmorMesh() const
{
	return ArmorMesh;
}

EArmorEquipSlot UArmorItem::GetEquipSlot() const
{
	return EquipSlot;
}
