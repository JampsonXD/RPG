// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "RPG_SuperItem.h"
#include "RPG_Types.h"
#include "ArmorItem.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UArmorItem : public URPG_SuperItem
{
	
	GENERATED_UCLASS_BODY()
	
protected:
	// Armor mesh we want to show on the player
	UPROPERTY(EditDefaultsOnly, Category = "Item | Armor")
	USkeletalMesh* ArmorMesh;

	// The equipment slot we will take up
	UPROPERTY(EditDefaultsOnly, Category = "Item | Armor")
	EArmorEquipSlot EquipSlot;

public:

	virtual bool CanUse() const override;

	virtual void Use() override;

	virtual void Remove() override;
	
	// Gets our Armor Mesh pointer
	virtual USkeletalMesh* GetArmorMesh() const;

	// Gets our equip slot type
	virtual EArmorEquipSlot GetEquipSlot() const;
};
