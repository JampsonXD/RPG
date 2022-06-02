// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractTypes.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INTERACTIONSYSTEM_API IInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// Returns true if the interaction starts successfully
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool PreInteract(FInteractInfo InteractInfo);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(FInteractInfo InteractInfo);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PostInteract(FInteractInfo InteractInfo, EInteractionEndState EndState);
	
};
