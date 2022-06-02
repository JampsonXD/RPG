// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GAS_Types.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGBFL_MainFunctions.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGBFL_MainFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	public:

	/**
	 * Returns the first actor of the passed in actor filter, or the first actor within the array
	 * @param ActorArray : Array we are filtering
	 * @param ActorFilter : The actor subclass we are checking against
	 * @param FoundActor : The actor we are returning as an output value, will be null if the array was empty
	 * @return bool : Whether we were able to successfully return an actor from the array (false if empty array or invalid)
	 */
	UFUNCTION(BlueprintCallable, Category = "RPGBFL Main Functions")
	static bool GetFirstActorOfClassOrFirstActor(TArray<AActor*> ActorArray, TSubclassOf<AActor> ActorFilter, AActor*& FoundActor);
	
};
