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
	LeftHand,
	BothHands
};

UENUM()
enum class EGunFireMode : uint8
{
	SemiAutomatic,
	Automatic,
	BurstFire
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


class URPG_SuperWeapon;

USTRUCT()
struct FItemActorData
{
	GENERATED_BODY()

	FItemActorData() : WeaponObject(nullptr), InventorySystemComponent(nullptr), OwningActor(nullptr) {}
	FItemActorData(URPG_SuperWeapon* InWeaponObject, UInventorySystemComponent* InInventorySystemComponent)
	{
		WeaponObject = InWeaponObject;
		InventorySystemComponent = InInventorySystemComponent;
		OwningActor = InInventorySystemComponent ? InInventorySystemComponent->OwningActor : nullptr;
	}

	bool operator==(const FItemActorData& Other) const { return WeaponObject == Other.WeaponObject && InventorySystemComponent == Other.InventorySystemComponent && OwningActor == Other.OwningActor;  }
	bool operator!=(const FItemActorData& Other) const { return !(*this == Other); }

	URPG_SuperWeapon* WeaponObject;

	UInventorySystemComponent* InventorySystemComponent;

	AActor* OwningActor;

	bool IsValid() const { return WeaponObject && InventorySystemComponent && OwningActor; }
};
