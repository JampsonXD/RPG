// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GAS_Types.h"
#include "GAS/Calculations/RPG_DamageMitigationCalculations.h"
#include "RPG_Types.generated.h"

/**
 * 
 */

UENUM()
enum class EArmorEquipSlot : uint8
{
	None,
	Helmet,
	Chest,
	Gauntlets,
	Legs
};

UENUM()
enum class EWeaponEquipSlot : uint8
{
	None,
	RightHand,
	LeftHand
};

USTRUCT(BlueprintType)
struct FArmorEquipData
{
	GENERATED_BODY()

	FArmorEquipData()
	{
		EquipSlot = EArmorEquipSlot::None;
		ArmorItem = nullptr;
		EquipStatus = FGameplayTag::EmptyTag;
	}

	FArmorEquipData(EArmorEquipSlot Slot, UObject* ActorPointer, FGameplayTag Status)
	{
		EquipSlot = Slot;
		ArmorItem = ActorPointer;
		EquipStatus = Status;
	}
	
	// Equipment slot we are modifying
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip Data")
	EArmorEquipSlot EquipSlot;

	// Pointer to the actor we are trying to equip/unequip
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip Data")
	UObject* ArmorItem;

	// Status of our equipment, ex. whether we are equipping or unequipping the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip Data")
	FGameplayTag EquipStatus;
	
};

USTRUCT(BlueprintType)
struct FDamagePopupData
{
	GENERATED_BODY()

	FDamagePopupData()
	{
		DamageColor = FLinearColor::White;
		DamageAmount = 0.f;
		bCriticalStrike = false;
		DamagePopupLocation = FVector::ZeroVector;
	}

	FDamagePopupData(FLinearColor InDamageColor, float InDamageAmount, bool IsCriticalStrike = false, FVector InLocation = FVector::ZeroVector)
	{
		DamageColor = InDamageColor;
		DamageAmount = InDamageAmount;
		bCriticalStrike = IsCriticalStrike;
		DamagePopupLocation = InLocation;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Popup Data")
	FLinearColor DamageColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Popup Data")
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Popup Data")
	bool bCriticalStrike;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Popup Data")
	FVector DamagePopupLocation;
};

USTRUCT(BlueprintType)
struct FDamageTypeData
{
	GENERATED_BODY()

	FDamageTypeData()
	{
		DamageType = EDamageType::PhysicalDamage;
		DamageColor = FLinearColor::White;
		DamageMitigationMod = URPG_DamageMitigationCalculations::StaticClass();
	}

	FDamageTypeData(EDamageType InDamageType, FLinearColor InDamageColor)
	{
		DamageType = InDamageType;
		DamageColor = InDamageColor;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Type Data")
	EDamageType DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Type Data")
	FLinearColor DamageColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Type Data")
	TSubclassOf<URPG_DamageMitigationCalculations> DamageMitigationMod;
	
};