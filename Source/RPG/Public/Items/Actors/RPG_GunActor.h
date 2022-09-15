// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Actors/RPG_WeaponActor.h"
#include "RPG_GunActor.generated.h"

class URPG_SuperGun;
class URPG_WeaponEffectDataAsset;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentClipSizeChanged, int, OldValue, int, NewValue);

/**
 * 
 */
UCLASS()
class RPG_API ARPG_GunActor : public ARPG_WeaponActor
{
	GENERATED_BODY()

public:

	ARPG_GunActor();

	/* Multicast delegate invoked when our current ammo changes */
	UPROPERTY(BlueprintAssignable)
	FOnCurrentClipSizeChanged OnCurrentClipSizeChanged;

protected:

	/* Maximum amount of ammo the weapons magazine can hold */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int MaximumClipSize;

	/* Current amount of ammo in the weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentClipSize;

	/* Pointer to our Ammo Type */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UItem* AmmoData;

	/* Pointer to the data asset that represents our gun */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	URPG_SuperGun* GunData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	URPG_WeaponEffectDataAsset* EffectsData;

	UPROPERTY()
	UUserWidget* CrosshairWidget;

protected:

	virtual void SetupItem_Implementation(FItemSetupData ItemSetupData) override;

	virtual void EquipItem_Implementation(ARPG_Character* EquippingCharacter, URPG_AbilitySystemComponent* RPGAbilitySystemComponent) override;

	virtual void UnEquipItem_Implementation(ARPG_Character* EquippingCharacter, URPG_AbilitySystemComponent* RPGAbilitySystemComponent) override;

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "RPG Gun")
	bool CanFire();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "RPG Gun")
	bool CanReload();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "RPG Gun")
	void Reload();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "RPG Gun")
	void FireWeaponEffects();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetCurrentAmmo() const;

	UFUNCTION(BlueprintCallable)
	void SetCurrentAmmo(int NewAmmoAmount);
};
