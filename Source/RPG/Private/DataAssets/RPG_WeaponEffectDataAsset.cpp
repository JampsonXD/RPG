// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/RPG_WeaponEffectDataAsset.h"

FPrimaryAssetId URPG_WeaponEffectDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(FPrimaryAssetType("Weapon Effects"), GetFName());
}
