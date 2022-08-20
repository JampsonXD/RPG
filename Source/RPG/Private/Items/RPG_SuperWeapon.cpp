// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/RPG_SuperWeapon.h"
#include "RPG_Types.h"

URPG_SuperWeapon::URPG_SuperWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	EquipSlot = EWeaponEquipSlot::None;
	ItemType = FPrimaryAssetType("Weapon");
}
