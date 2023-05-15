// Fill out your copyright notice in the Description page of Project Settings.


#include "FXManagerSubsystem.h"
#include "GameplayTagAssetInterface.h"
#include "NiagaraFunctionLibrary.h"


bool UFXManagerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UFXManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UFXManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

UFXManagerSubsystem* UFXManagerSubsystem::GetFXManager()
{
	if(GEngine)
	{
		return GEngine->GetEngineSubsystem<UFXManagerSubsystem>();
	}

	return nullptr;
}

FActiveEffectPackHandle UFXManagerSubsystem::PlayEffectAtLocation(AActor* SourceActor, AActor* TargetActor,
                                                         const FEffectPack& EffectPack, EEffectActivationType ActivationType, FTransform Transform)
{
	SpawnVisualEffectsAtLocation VisualEffectsSpawner = SpawnVisualEffectsAtLocation(Transform);
	SpawnSoundEffectsAtLocation SoundEffectsSpawner = SpawnSoundEffectsAtLocation(Transform);
	return Internal_PlayEffect(SourceActor, TargetActor, EffectPack, ActivationType, &VisualEffectsSpawner, &SoundEffectsSpawner);
}

FActiveEffectPackHandle UFXManagerSubsystem::PlayEffectAttached(AActor* SourceActor, AActor* TargetActor,
	USceneComponent* AttachComponent, const FEffectPack& EffectPack, EEffectActivationType ActivationType)
{
	if(!AttachComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attach Component is null!"))
		return FActiveEffectPackHandle();
	}

	SpawnVisualEffectsAtComponent VisualEffectsSpawner = SpawnVisualEffectsAtComponent(AttachComponent);
	SpawnSoundEffectsAttached SoundEffectsSpawner = SpawnSoundEffectsAttached(AttachComponent);
	return Internal_PlayEffect(SourceActor, TargetActor, EffectPack, ActivationType, &VisualEffectsSpawner, &SoundEffectsSpawner);
}

FActiveEffectPackHandle UFXManagerSubsystem::Internal_PlayEffect(AActor* SourceActor, AActor* TargetActor,
	const FEffectPack& EffectPack, EEffectActivationType ActivationType,
	SpawnEffects<UFXSystemComponent, FVFXData>* SpawnVisualEffects,
	SpawnEffects<UAudioComponent, FSFXData>* SpawnSoundEffects)
{
	if (!EffectPack.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect Pack is Empty."))
		return FActiveEffectPackHandle();
	}

	if (!SourceActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Source Actor is invalid!"))
		return FActiveEffectPackHandle();
	}

	FActiveEffectPack ActivePack = FActiveEffectPack(GenerateNewActivePackId(), SourceActor, TargetActor, ActivationType);

	const FGameplayTagContainer SourceTags = GetActorTags(SourceActor);
	const FGameplayTagContainer TargetTags = GetActorTags(TargetActor);

	for (const FVFXData& VfxData : EffectPack.VFXData)
	{
		/* Go to the next effect data if this one is unable to play */
		if (!VfxData.CanPlay(SourceTags, TargetTags))
		{
			continue;
		}

		ActivePack.AddActiveVFX(SpawnVisualEffects->Spawn(VfxData, SourceActor, TargetActor), VfxData.AccessTag);
	}

	for (const FSFXData& SfxData : EffectPack.SFXData)
	{
		/* Go to the next effect data if this one is unable to play */
		if (!SfxData.CanPlay(SourceTags, TargetTags))
		{
			continue;
		}

		ActivePack.AddActiveSound(SpawnSoundEffects->Spawn(SfxData, SourceActor, TargetActor), SfxData.AccessTag);
	}

	if (!ActivePack.IsActive())
	{
		return FActiveEffectPackHandle();
	}

	ActivationType == EEffectActivationType::Active ? ActiveEffectPacks.Add(ActivePack) : AddInstantPack(SourceActor, ActivePack);
	return ActivePack.CreateHandle();
}

void UFXManagerSubsystem::StopActivePack(const FActiveEffectPackHandle& Handle)
{
	for(auto Iterator = ActiveEffectPacks.CreateIterator(); Iterator; ++Iterator)
	{
		FActiveEffectPack& Pack = ActiveEffectPacks[Iterator.GetIndex()];
		if(Pack.Id == Handle.GetId())
		{
			Pack.Invalidate();
			Iterator.RemoveCurrent();
			return;
		}
	}
}

void UFXManagerSubsystem::StopActivePacks(const TArray<FActiveEffectPackHandle>& Handles)
{
	/* Cache all Handle Id's using a Set, this keeps us from having to iterate over our active effect pack
	 * array for each handle, O(n) instead of O(n^2) */
	TSet<int> PackIdsToRemove;

	// Iterate through all our active effect packs and cache their Id's
	for(int i = 0; i < Handles.Num(); ++i)
	{
		const FActiveEffectPackHandle& Handle = Handles[i];
		PackIdsToRemove.Add(Handle.GetId());
	}

	// Check if the handles Id was in our cached set, invalidate the pack, and remove the current index
	for(auto Iterator = ActiveEffectPacks.CreateIterator(); Iterator; ++Iterator)
	{
		FActiveEffectPack& Pack = ActiveEffectPacks[Iterator.GetIndex()];
		if(PackIdsToRemove.Contains(Pack.Id))
		{
			Pack.Invalidate();
			Iterator.RemoveCurrent();
		}
	}
}

UFXSystemComponent* UFXManagerSubsystem::GetVfxSystemComponentByTag(const FActiveEffectPackHandle& Handle,
	FGameplayTag Tag)
{
	return Internal_GetVfxSystemComponent(Handle, [Tag](const FActiveEffect<UFXSystemComponent*>& ActiveEffect)
	{
		return ActiveEffect.AccessTag == Tag;
	});
}

UAudioComponent* UFXManagerSubsystem::GetSfxSystemComponentByTag(const FActiveEffectPackHandle& Handle,
	FGameplayTag Tag)
{
	return Internal_FindSfxSystemComponent(Handle, [Tag](const FActiveEffect<UAudioComponent*>& ActiveEffect)
	{
		return ActiveEffect.AccessTag == Tag;
	});
}


FActiveEffectPack& UFXManagerSubsystem::GetActivePack(const FActiveEffectPackHandle& Handle)
{
	FActiveEffectPack TempPack;
	if(!Handle.IsValid())
	{
		return TempPack;
	}
	
	/* Get our active or instant pack array based on the activation data from our handle */
	TArray<FActiveEffectPack>& Array = Handle.GetPackType() == EEffectActivationType::Active ? ActiveEffectPacks : InstantEffectPacks;
	for (auto Iterator = Array.CreateConstIterator(); Iterator; ++Iterator)
	{
		FActiveEffectPack& Pack = Array[Iterator.GetIndex()];
		if (Pack.Id == Handle.GetId())
		{
			return Pack;
		}
	}
	
	return TempPack;
}

void UFXManagerSubsystem::AddInstantPack(const UObject* WorldContextObject, const FActiveEffectPack& ActivePack)
{
	InstantEffectPacks.Add(ActivePack);
	if(!WorldContextObject->GetWorld()->GetTimerManager().IsTimerActive(InstantPackTimerHandle))
	{
		InstantPackTimerHandle = WorldContextObject->GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UFXManagerSubsystem::ClearInstantPacks);
	}
}

void UFXManagerSubsystem::ClearInstantPacks()
{
	InstantEffectPacks.Empty();
}

int UFXManagerSubsystem::GenerateNewActivePackId()
{
	return Internal_NextId++;
}

FGameplayTagContainer UFXManagerSubsystem::GetActorTags(const AActor* Actor) const
{
	FGameplayTagContainer Container = FGameplayTagContainer::EmptyContainer;

	if(const IGameplayTagAssetInterface* Interface = Cast<IGameplayTagAssetInterface>(Actor))
	{
		Interface->GetOwnedGameplayTags(Container);
	}

	return Container;
}
