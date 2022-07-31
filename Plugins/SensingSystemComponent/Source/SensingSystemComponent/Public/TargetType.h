// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TargetType.generated.h"

class USensingSystemComponent;
/**
 * 
 */
UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class SENSINGSYSTEM_API UCollectionType : public UObject
{
	GENERATED_BODY()


public:

	/* Determines how to gather actors that are available for sensing */
	UFUNCTION(BlueprintNativeEvent)
	TArray<AActor*> GatherActorsForSensing(USensingSystemComponent* SensingSystemComponent, AActor* SensingSystemOwner);
};


UCLASS(Blueprintable)
class SENSINGSYSTEM_API UFilterSensingData : public UObject
{
	GENERATED_BODY()

public:

	/* Returns all actors that make it past a filtering process and are available for sensing */
	UFUNCTION()
	virtual TArray<AActor*> FilterActorsForSensing(USensingSystemComponent* SensingSystemComponent, AActor* SensingSystemOwner, const TArray<AActor*>& TargetActors) const;

	UFUNCTION(BlueprintImplementableEvent)
	TArray<AActor*> K2_FilterActorsForSensing(USensingSystemComponent* SensingSystemComponent, AActor* SensingSystemOwner, UPARAM(ref) const TArray<AActor*>& TargetActors) const;
};