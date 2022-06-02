// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QuestSystemInterface.generated.h"

class UQuestSystemComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UQuestSystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IQuestSystemInterface
{
	GENERATED_BODY()

	// Get Quest System Component from anyone who implements this interface
public:
	virtual UQuestSystemComponent* GetQuestSystemComponent()const = 0;
};
