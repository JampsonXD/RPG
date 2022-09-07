// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FXManager/Public/FXTypes.h"
#include "RPG_WeaponEffectDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPG_WeaponEffectDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FEffectPack OnFireEffects;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UForceFeedbackEffect* OnFireForce;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FEffectPack MuzzleEffects;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FEffectPack BulletTrailEffects;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FEffectPack WeaponEmptyEffects;
};
