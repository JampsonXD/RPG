// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Actors/RPG_WeaponActor.h"
#include "RPG_GunActor.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPG_GunActor : public ARPG_WeaponActor
{
	GENERATED_BODY()

public:

	ARPG_GunActor();

protected:

	/* Maximum amount of ammo the weapons magazine can hold */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int MaximumClipSize;

	/* Current amount of ammo in the weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentClipSize;

	/* Pointer to the data asset that represents the type of ammo this weapon will use */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UItem* AmmoType;

	/* Ability Set Active Handle, Handle for the Active Ability Set while this weapon is in use and granting effects */
	UPROPERTY(BlueprintReadOnly)
	FAbilitySetActiveHandle AbilitySetActiveHandle;

public:

	UFUNCTION(BlueprintNativeEvent, Category = "RPG Gun")
	bool CanFire();

	UFUNCTION(BlueprintNativeEvent, Category = "RPG Gun")
	bool CanReload();

	UFUNCTION(BlueprintNativeEvent, Category = "RPG Gun")
	void Reload();

	UFUNCTION(BlueprintNativeEvent, Category = "RPG Gun")
	void FireWeapon();
};
