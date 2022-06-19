// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_ItemTypes.h"
#include "Items/RPG_SuperItem.h"
#include "RPG_Types.h"
#include "RPG_SuperWeapon.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPG_SuperWeapon : public URPG_SuperItem
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "RPG Weapon")
	EWeaponEquipSlot EquipSlot;


	UPROPERTY(EditDefaultsOnly, Category = "RPG Weapon")
	USkeletalMesh* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "RPG Weapon")
	FEquipData EquipData;

public:

	URPG_SuperWeapon(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category = "RPG Weapon")
	virtual EWeaponEquipSlot GetEquipSlot() const;

	UFUNCTION(BlueprintPure, Category = "RPG Weapon")
	virtual USkeletalMesh* GetWeaponMesh() const;

	UFUNCTION(BlueprintPure, Category = "RPG Weapon")
	virtual FEquipData GetEquipData() const;
};
