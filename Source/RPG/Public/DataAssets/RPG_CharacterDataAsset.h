// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RPG_CharacterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPG_CharacterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

	public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CharacterData, AssetRegistrySearchable, meta = (AssetBundles = "InWorld"))
	USkeletalMesh* CharacterMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CharacterData, AssetRegistrySearchable, meta = (AssetBundles = "InWorld"))
	TSubclassOf<UAnimInstance> AnimClass;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
