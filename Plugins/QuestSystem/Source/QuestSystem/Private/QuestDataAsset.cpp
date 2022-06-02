// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestDataAsset.h"

FPrimaryAssetId UQuestDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(FPrimaryAssetType(TEXT("Quest")), GetFName());
}
