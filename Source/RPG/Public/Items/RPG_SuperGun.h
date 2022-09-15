// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/RPG_SuperWeapon.h"
#include "DataAssets/RPG_WeaponEffectDataAsset.h"
#include "RPG_SuperGun.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPG_SuperGun : public URPG_SuperWeapon
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG Gun")
	int ClipSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG Gun")
	int FireRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG Gun")
	EGunFireMode FireMode;

	/* Pointer to the type of object we will consume to use the weapon */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG Gun")
	UItem* AmmoItem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG Gun | Effects")
	URPG_WeaponEffectDataAsset* WeaponEffects;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG Gun | UI")
	TSubclassOf<UUserWidget> CrosshairWidget;

};
