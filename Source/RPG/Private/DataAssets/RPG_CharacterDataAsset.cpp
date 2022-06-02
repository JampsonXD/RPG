// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/RPG_CharacterDataAsset.h"

#include "RPGAssetManager.h"

FPrimaryAssetId URPG_CharacterDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(URPGAssetManager::CharacterType, GetFName());
}
