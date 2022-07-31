// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPFL_SensingSystem.generated.h"

class USensingSystemComponent;
/**
 * 
 */
UCLASS()
class SENSINGSYSTEM_API UBPFL_SensingSystem : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


	UFUNCTION(BlueprintPure, Category = "BPFL Sensing System")
	static USensingSystemComponent* GetSensingSystemComponentFromActor(const AActor* TargetActor);
};
