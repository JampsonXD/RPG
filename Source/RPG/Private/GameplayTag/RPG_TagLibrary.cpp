// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTag/RPG_TagLibrary.h"
#include "GameplayTagsManager.h"

const FRPG_TagLibrary* FRPG_TagLibrary::Singleton = nullptr;

FRPG_TagLibrary::FRPG_TagLibrary(UGameplayTagsManager& TagManager)
{
	Aiming = TagManager.AddNativeGameplayTag("Status.Aiming");
	Crouching = TagManager.AddNativeGameplayTag("Status.Crouching");
	Running = TagManager.AddNativeGameplayTag("Status.Running");
	WeaponLowered = TagManager.AddNativeGameplayTag("Status.WeaponLowered");
	Jumping = TagManager.AddNativeGameplayTag("Status.Jumping");
}
