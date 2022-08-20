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

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RPG Weapon")
	EWeaponEquipSlot EquipSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RPG Weapon")
	USkeletalMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RPG Weapon")
	FEquipData EquipData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RPG Weapon")
	TSubclassOf<UAnimInstance> AnimationClass;

	URPG_SuperWeapon(const FObjectInitializer& ObjectInitializer);

};
