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

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Pool Actor")
	void OnPoolEntered();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Pool Actor")
	void OnPoolLeft(const FActorPopData& PopData);
};
