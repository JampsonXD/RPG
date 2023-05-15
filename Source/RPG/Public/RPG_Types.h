// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystemComponent.h"
#include "GAS/GAS_Types.h"
#include "GAS/Calculations/RPG_DamageMitigationCalculations.h"
#include "RPG_Types.generated.h"

/**
 * 
 */

UENUM()
enum class EWeaponEquipSlot : uint8
{
	None,
	RightHand,
	LeftHand,
	BothHands
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
