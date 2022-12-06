// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Items/Actors/RPG_GunActor.h"
#include "AT_FireGun.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInputReleased);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCommitAbilityFailed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponFired);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponFinishedFiring);

UCLASS()
class RPG_API UAT_FireGun : public UAbilityTask
{
	GENERATED_BODY()

private:

	UPROPERTY(BlueprintAssignable)
	FOnInputReleased InputReleased;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponFired WeaponFired;

	UPROPERTY(BlueprintAssignable)
	FOnCommitAbilityFailed CommitAbilityFailed;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponFinishedFiring FinishedFiring;

	UPROPERTY()
	bool bShouldCommitAbility;
	
	UPROPERTY()
	EGunFireMode FireMode;

	UPROPERTY()
	float RateOfFire;

	UPROPERTY()
	float TotalShotsFired;

	FTimerHandle WeaponFireTimerHandle;
	
	FDelegateHandle DelegateHandle;

public:

	UAT_FireGun();

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UAT_FireGun* FireWeapon(UGameplayAbility* OwningAbility, URPG_SuperGun* GunDef, FGameplayTag CooldownTag, bool bCommitAbility = true);

	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

protected:
	
	UFUNCTION()
	void HandleWeaponFire();

	UFUNCTION()
	void SetupAutomaticWeaponInput();

	UFUNCTION()
	bool CanFireWeapon() const;

	UFUNCTION()
	void SetWeaponFireTimer();

	UFUNCTION()
	void OnReleaseCallback();

	UFUNCTION()
	void CommitAbilityCooldown();
	
};
