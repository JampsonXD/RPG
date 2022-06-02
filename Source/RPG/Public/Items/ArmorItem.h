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
	virtual bool Use_Implementation(UInventorySystemComponent* TargetInventorySystemComponent, AActor* TargetActor) override;
	
	// Gets our Armor Mesh pointer
	USkeletalMesh* GetArmorMesh() const;

	// Gets our equip slot type
	EArmorEquipSlot GetEquipSlot() const;
};
