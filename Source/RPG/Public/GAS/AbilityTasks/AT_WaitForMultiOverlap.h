// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_WaitForMultiOverlap.generated.h"

/**
 * 
 */

class USphereComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FATWaitForMultiOverlapEventDelegate, const FGameplayAbilityTargetDataHandle&,
                                            Handle);

UCLASS()
class RPG_API UAT_WaitForMultiOverlap : public UAbilityTask
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<AActor*> OverlappingActors;

	UPROPERTY()
	float SphereRadius;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> OverlapCollisionChannels;

	UPROPERTY()
	TSubclassOf<AActor> OptionalActorFilter;

	UPROPERTY()
	TArray<AActor*> IgnoreActors;

	UPROPERTY()
	FTimerHandle TimerHandle;

protected:

	// Constructor
	UAT_WaitForMultiOverlap(const FObjectInitializer& ObjectInitializer);
	
	// Ability Task Functions to override
	virtual void Activate() override;
	virtual void ExternalCancel() override;
	virtual FString GetDebugString() const override;
	virtual void OnDestroy(bool AbilityEnded) override;

	/* Uses a Sphere Trace to check for any actor overlaps, compares an internal array of actors overlapped last tick to the new array
	* to find if any actors left the overlap
	*/
	void Update();

	bool GetOverlappingActorsFromSphereTrace(TArray<AActor*>& InOverlappingActors) const;

	public:

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE", AutoCreateRefTerm = "ActorsToIgnore"))
	static UAT_WaitForMultiOverlap* WaitForMultiSphereOverlap(UGameplayAbility* OwningAbility, FName TaskInstanceName,
		float Radius, TArray<TEnumAsByte<EObjectTypeQuery>> CollisionChannels, TSubclassOf<AActor> OptionalActorClassFilter, TArray<AActor*> ActorsToIgnore);

	UPROPERTY(BlueprintAssignable)
	FATWaitForMultiOverlapEventDelegate OnActorAdded;

	UPROPERTY(BlueprintAssignable)
	FATWaitForMultiOverlapEventDelegate OnActorRemoved;
};
