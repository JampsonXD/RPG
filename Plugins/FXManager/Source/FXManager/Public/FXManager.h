// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FXTypes.h"
#include "UObject/NoExportTypes.h"
#include "FXManager.generated.h"

/**
 * 
 */
UCLASS()
class FXMANAGER_API UFXManager : public UObject
{
	GENERATED_BODY()
		
	template <typename T>
	static T* GetAssetLoaded(TSoftObjectPtr<T> SoftObjectPtr);

	/* What our next Id should be for any active pack */
	int Internal_NextId = 0;

	/* Array of active effects we are managing */
	TArray<FActiveEffectPack> ActiveEffectPacks;

	/* Array of internal effect packs that will be removed on the next tick */
	TArray<FActiveEffectPack> InstantEffectPacks;

	FTimerHandle InstantPackTimerHandle;

	/* Maps attachment rules to attach location types */
	static const TMap<EAttachmentRule, EAttachLocation::Type> AttachmentMap;

public:

	FActiveEffectPackHandle PlayEffectAtLocation(AActor* SourceActor, AActor* TargetActor, FEffectPack& EffectPack, EEffectActivationType ActivationType = EEffectActivationType::Instant, FTransform Transform = FTransform::Identity);
	FActiveEffectPackHandle PlayEffectAttached(AActor* SourceActor, AActor* TargetActor, USceneComponent* AttachComponent, FEffectPack& EffectPack, EEffectActivationType ActivationType = EEffectActivationType::Instant);

	void StopActivePack(const FActiveEffectPackHandle& Handle);

private:

	UFXSystemComponent* SpawnVFXDataAtLocation(const FVFXData VFXData, const AActor* SourceActor, const FTransform& Transform) const;

	UAudioComponent* SpawnSFXDataAtLocation(const FSFXData SFXData, const AActor* SourceActor, const FTransform& Transform) const;

	UFXSystemComponent* SpawnVFXDataAtComponent(const FVFXData VFXData, const AActor* SourceActor, USceneComponent* AttachComponent) const;

	UAudioComponent* SpawnSFXDataAtComponent(const FSFXData SFXData, const AActor* SourceActor, USceneComponent* AttachComponent) const;

	FActiveEffectPack& GetActivePack(const FActiveEffectPackHandle& Handle);

	/* Adds packs with an instant activation for the next tick so that they can be modified if necessary */
	void AddInstantPack(const UObject* WorldContextObject, const FActiveEffectPack& ActivePack);

	void ClearInstantPacks();

	/* Increments our internal id counter and returns the next id */
	int GenerateNewActivePackId();

	/* Returns actor tags from the IGameplayTagInterface, if implemented by the passed in actor */
	FGameplayTagContainer GetActorTags(const AActor* Actor) const;

	/* Converts an attachment rule to the needed attach location type enumerator */
	static EAttachLocation::Type GetAttachLocationType(const EAttachmentRule& Rule);
};
