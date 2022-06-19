// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/RPG_SuperWeapon.h"
#include "RPG_SuperGun.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPG_SuperGun : public URPG_SuperWeapon
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "RPG Gun")
	int ClipSize;

	UPROPERTY(EditDefaultsOnly, Category = "RPG Gun")
	int FireRate;

	UPROPERTY(EditDefaultsOnly, Category = "RPG Gun")
	EGunFireMode FireMode;

public:

	UFUNCTION(BlueprintPure, Category = "RPG Gun")
	virtual int GetMaxClipSize() const;

	UFUNCTION(BlueprintPure, Category = "RPG Gun")
	virtual float GetFireRate() const;

	UFUNCTION(BlueprintPure, Category = "RPG Gun")
	virtual EGunFireMode GetFireMode() const;
	
};
