// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FXManagerSubsystem.h"
#include "FXTypes.h"
#include "GameplayTag/RPG_TagLibrary.h"
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
	 * Applies a Gameplay Effect using TargetData using the passed in AbilitySystemComponent as the source.
	 * @param AbilitySystemComponent : Source Ability System Component
	 * @param GameplayEffect : Gameplay Effect Class we are applying to our target
	 * @param TargetData : Target Data Handle that determines what targets to apply the effect to
	 * @param AbilityLevel : The level of the gameplay effect we are applying
	 * @param Stacks : The amount of stacks of the gameplay effect we are applying to targets
	 * @return FGameplayEffectHandle : Array of Handles to applied active gameplay effects
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability System Component")
	static TArray<FActiveGameplayEffectHandle> ApplyGameplayEffectToTargetWithTargetData(UAbilitySystemComponent* AbilitySystemComponent,
		TSubclassOf<UGameplayEffect> GameplayEffect,
		FGameplayAbilityTargetDataHandle TargetData,
		int AbilityLevel = 1, int Stacks = 1);

	UFUNCTION(BlueprintCallable, Category = "Ability System Component")
	static UObject* GetCurrentSourceObjectFromSpec(const FGameplayEffectSpec& Spec);

	UFUNCTION(BlueprintCallable, Category = "Target Data")
	static FGameplayEffectContainerSpec AddTargetDataToContainerSpec(UPARAM(Ref) FGameplayEffectContainerSpec& ContainerSpec, FGameplayAbilityTargetDataHandle TargetDataHandle);

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

	UFUNCTION(BlueprintCallable, Category = "RPGBFL Main Functions")
	static bool AddActorToPool(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "RPG Effects")
	static FActiveEffectPackHandle PlayEffectPackAtLocation(FEffectPack EffectPack, AActor* SourceActor,
	AActor* TargetActor = nullptr, EEffectActivationType ActivationType = EEffectActivationType::Instant, FTransform Transform = FTransform());

	UFUNCTION(BlueprintCallable, Category = "RPG Effects")
	static FActiveEffectPackHandle PlayEffectPackAttached(FEffectPack EffectPack, AActor* SourceActor, AActor* TargetActor = nullptr,
		EEffectActivationType ActivationType = EEffectActivationType::Instant, USceneComponent* AttachComponent = nullptr);

	UFUNCTION(BlueprintCallable, Category = "RPG Effects")
	static void StopActiveEffectPackWithHandle(UPARAM(ref) FActiveEffectPackHandle& Handle);

	UFUNCTION(BlueprintCallable, Category = "RPG Effects")
	static void AddForceFeedbackOnController(AController* Controller, UForceFeedbackEffect* ForceFeedbackEffect, bool bIsLooping = false);

	UFUNCTION(BlueprintCallable, Category = "RPG Loading Screen")
	static void PlayRPGLoadingScreen(bool bPlayUntilStopped, float Playtime);

	UFUNCTION(BlueprintCallable, Category = "RPG Loading Screen")
	static void StopRPGLoadingScreen();

	UFUNCTION(BlueprintCallable, Category = "RPG GAS")
	static void AddLooseGameplayTagsToActor(AActor* TargetActor, const FGameplayTagContainer Tags);

	UFUNCTION(BlueprintCallable, Category = "RPG GAS")
	static void RemoveLooseGameplayTagsToActor(AActor* TargetActor, const FGameplayTagContainer Tags);

	UFUNCTION(BlueprintPure, Category = "RPG Gameplay Tags")
	static FGameplayTag GetAimingTag()
	{
		return FRPG_TagLibrary::Get().AimingTag();
	}

	UFUNCTION(BlueprintPure, Category = "RPG Gameplay Tags")
		static FGameplayTag GetCrouchingTag()
	{
		return FRPG_TagLibrary::Get().CrouchingTag();
	}

	UFUNCTION(BlueprintPure, Category = "RPG Gameplay Tags")
		static FGameplayTag GetRunningTag()
	{
		return FRPG_TagLibrary::Get().RunningTag();
	}

	UFUNCTION(BlueprintPure, Category = "RPG Gameplay Tags")
		static FGameplayTag GetWeaponLoweredTag()
	{
		return FRPG_TagLibrary::Get().WeaponLoweredTag();
	}

	UFUNCTION(BlueprintPure, Category = "RPG Gameplay Tags")
		static FGameplayTag GetJumpingTag()
	{
		return FRPG_TagLibrary::Get().JumpingTag();
	}

	UFUNCTION(BlueprintCallable, Category = "RPG Math")
	static void SortFloatsByValue(UPARAM(ref) TArray<float>& FloatArray)
	{
		const auto lambda = [](float ParamOne, float ParamTwo)
		{
			return ParamOne >= ParamTwo;
		};
		
		QuickSort<float>(FloatArray, 0, FloatArray.Num() - 1, lambda);
	}

	template<class A, class B>
	static TArray<TPair<A,B>> SortMap(const TMap<A,B>& UnsortedMap, TFunction<bool(TPair<A,B> ParamOne, TPair<A,B> ParamTwo)> Predicate)
	{
		TArray<TPair<A,B>> Pairs;
		for(const TPair<A,B>& Pair : UnsortedMap)
		{
			Pairs.Add(Pair);
		}

		QuickSort(Pairs, 0, Pairs.Num() - 1);
		return Pairs;
	}

	template<class T>
	static void QuickSort(TArray<T>& Array, int LowIndex, int HighIndex, const TFunctionRef<bool(T ParamOne, T ParamTwo)>& Predicate)
	{
		if(LowIndex >= HighIndex)
		{
			return;
		}

		T Pivot = Array[HighIndex];
		
		int leftPointer = LowIndex;
		int rightPointer = HighIndex;
		

		while(leftPointer < rightPointer)
		{
			while(Predicate(Pivot, Array[leftPointer]) && leftPointer < rightPointer)
			{
				++leftPointer;
			}

			while(Predicate(Array[rightPointer], Pivot) && leftPointer < rightPointer)
			{
				--rightPointer;
			}
			
			Array.Swap(leftPointer, rightPointer);
		}

		Array.Swap(leftPointer, HighIndex);
		
		QuickSort<T>(Array, LowIndex, leftPointer - 1, Predicate);
		QuickSort<T>(Array, leftPointer + 1, HighIndex, Predicate);
	}
};
