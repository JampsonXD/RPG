// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TargetType.generated.h"

class USensingSystemComponent;
/**
 * 
 */
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, meta = (ShowWorldContextPin))
class SENSINGSYSTEM_API UCollectionType : public UObject
{
	GENERATED_BODY()


public:

	/* Determines how to gather actors that are available for sensing */
	UFUNCTION(BlueprintNativeEvent)
	TArray<AActor*> GatherActorsForSensing(USensingSystemComponent* SensingSystemComponent, AActor* SensingSystemOwner);
};


UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew)
class SENSINGSYSTEM_API UFilterSensingData : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * @brief Returns whether an actor meets the filter criteria.
	 * @param SensingSystemComponent The owning SensingSystemComponent checking if an actor meets the filter criteria
	 * @param SensingSystemOwner The owning Actor of the Sensing System Component
	 * @param TargetActor The actor we are checking against
	 * @return True if the actor meets the criteria and can be sensed, false if the actor does not meet the criteria
	 */
	UFUNCTION()
	virtual bool FilterActorForSensing(USensingSystemComponent* SensingSystemComponent, AActor* SensingSystemOwner, const AActor* TargetActor) const;

	UFUNCTION(BlueprintImplementableEvent)
	bool K2_FilterActorForSensing(USensingSystemComponent* SensingSystemComponent, AActor* SensingSystemOwner, const AActor* TargetActor) const;
};