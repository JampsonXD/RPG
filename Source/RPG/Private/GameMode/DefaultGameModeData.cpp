// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/DefaultGameModeData.h"

#include "RPGAssetManager.h"

FPrimaryAssetId UDefaultGameModeData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(URPGAssetManager::GameModeType, GetFName());
}
