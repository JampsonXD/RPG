// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SensingSystemComponent.h"
#include "UObject/Interface.h"
#include "SensingSystemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USensingSystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SENSINGSYSTEM_API ISensingSystemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual USensingSystemComponent* GetSensingSystemComponent() const = 0;
};
