// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Actors/RPG_GunActor.h"

ARPG_GunActor::ARPG_GunActor() : Super()
{
	CurrentClipSize = 0;
	MaximumClipSize = 0;
}

bool ARPG_GunActor::CanFire_Implementation()
{
	return CurrentClipSize > 0;
}

bool ARPG_GunActor::CanReload_Implementation()
{
	UInventorySystemComponent* ISC = GetOwningInventorySystemComponent();

	if(!ISC)
	{
		return false;
	}

	return ISC->HasItem(AmmoType);
}

void ARPG_GunActor::Reload_Implementation()
{

}

void ARPG_GunActor::FireWeapon_Implementation()
{

}
