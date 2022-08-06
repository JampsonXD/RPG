// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RPGCharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class URPGCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_API IRPGCharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsAiming() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsRunning() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsCrouching() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsWeaponLowered() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsJumping() const;
};
