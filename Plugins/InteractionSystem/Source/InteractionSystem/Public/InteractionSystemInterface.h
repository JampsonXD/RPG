// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionSystemInterface.generated.h"

USTRUCT()
struct FInteractionTracePoints
{
	GENERATED_BODY()

	FInteractionTracePoints()
	{
		StartLocation = FVector();
		ForwardVector = FVector();
	}

	FInteractionTracePoints(FVector InStartLocation, FVector InForwardVector)
	{
		StartLocation = InStartLocation;
		ForwardVector = InForwardVector;
	}
	
	// Starting Location for our Line Trace
	UPROPERTY()
	FVector StartLocation;

	// Forward Vector we will do our trace out from
	UPROPERTY()
	FVector ForwardVector;
	
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UInteractionSystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INTERACTIONSYSTEM_API IInteractionSystemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// Get our Interaction System Component
	virtual class UInteractionSystemComponent* GetInteractionSystemComponent() const = 0;

	// Trace Points for where our Interaction System should get its starting location and forward vector to do the trace out from
	virtual FInteractionTracePoints GetTracePoints() const = 0;
};
