// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FXTypes.h"
#include "GAS/GAS_Types.h"
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
	 * Takes in two arrays, adding any unique items from the target array to the secondary array
	 * @param TargetArray : Array we are adding unique items from
	 * @param OutArray : Array we are adding items to
	 * @return int : How many unique items we have added
	 */
	UFUNCTION(BlueprintCallable, meta = (ArrayParm = "TargetArray", ArrayTypeDependentParams = "OutArray"))
	static int AppendUnique(const TArray<UProperty*>& TargetArray, UPARAM(ref)TArray<UProperty*>& OutArray);

	/**
	 * Returns the first actor of the passed in actor filter, or the first actor within the array
	 * @param ActorArray : Array we are filtering
	 * @param ActorFilter : The actor subclass we are checking against
	 * @param FoundActor : The actor we are returning as an output value, will be null if the array was empty
	 * @return bool : Whether we were able to successfully return an actor from the array (false if empty array or invalid)
	 */
	UFUNCTION(BlueprintCallable, Category = "RPGBFL Main Functions")
	static bool GetFirstActorOfClassOrFirstActor(TArray<AActor*> ActorArray, TSubclassOf<AActor> ActorFilter, AActor*& FoundActor);

	UFUNCTION(BlueprintCallable, Category = "RPG Effects")
	static FActiveEffectPackHandle PlayEffectPackAtLocation(FEffectPack EffectPack, AActor* SourceActor,
	AActor* TargetActor = nullptr, EEffectActivationType ActivationType = EEffectActivationType::Instant, FTransform Transform = FTransform());

	UFUNCTION(BlueprintCallable, Category = "RPG Effects")
	static FActiveEffectPackHandle PlayEffectPackAttached(FEffectPack EffectPack, AActor* SourceActor, AActor* TargetActor = nullptr,
		EEffectActivationType ActivationType = EEffectActivationType::Instant, USceneComponent* AttachComponent = nullptr);

	UFUNCTION(BlueprintCallable, Category = "RPG Effects")
	static void StopActiveEffectPackWithHandle(UPARAM(ref) FActiveEffectPackHandle& Handle);
};
