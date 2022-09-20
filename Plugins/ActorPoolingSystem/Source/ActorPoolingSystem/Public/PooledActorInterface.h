// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolTypes.h"
#include "UObject/Interface.h"
#include "PooledActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UPooledActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACTORPOOLINGSYSTEM_API IPooledActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual void OnPoolEntered() = 0;

	UFUNCTION()
	virtual void OnPoolLeft(const FActorPopData& PopupData) = 0;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Pool Actor", meta = (DisplayName = "On Pool Entered"))
	void K2_OnPoolEntered();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Pool Actor", meta = (DisplayName = "On Pool Left"))
	void K2_OnPoolLeft(const FActorPopData& PopData);
};
