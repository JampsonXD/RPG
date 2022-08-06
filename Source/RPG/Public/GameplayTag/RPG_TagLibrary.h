// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"

/**
 * 
 */

class UGameplayTagsManager;

struct RPG_API FRPG_TagLibrary : public FNoncopyable
{
public:

	FRPG_TagLibrary(UGameplayTagsManager& TagManager);

	static FORCEINLINE const FRPG_TagLibrary& Get()
	{
		checkSlow(Singleton);
		return *Singleton;
	}

	FORCEINLINE const FGameplayTag& AimingTag() const { return Aiming; }
	FORCEINLINE const FGameplayTag& CrouchingTag() const { return Crouching; }
	FORCEINLINE const FGameplayTag& RunningTag() const { return Running; }
	FORCEINLINE const FGameplayTag& WeaponLoweredTag() const { return WeaponLowered; }
	FORCEINLINE const FGameplayTag& JumpingTag() const { return Jumping; }

private:

	// Main Module
	friend class FRPGGameModule;

	static void Startup(UGameplayTagsManager& TagManager)
	{
		checkSlow(Singleton);
		Singleton = new FRPG_TagLibrary(TagManager);
	}

	static const FRPG_TagLibrary* Singleton;

	/* Gameplay tags */

	FGameplayTag Aiming;
	FGameplayTag Crouching;
	FGameplayTag Running;
	FGameplayTag WeaponLowered;
	FGameplayTag Jumping;

};
