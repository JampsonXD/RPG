// Fill out your copyright notice in the Description page of Project Settings.


#include "FXManager.h"
#include "GameplayTagAssetInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"


template <typename T>
T* UFXManager::GetAssetLoaded(TSoftObjectPtr<T> SoftObjectPtr)
{
	if(SoftObjectPtr.IsValid())
	{
		return SoftObjectPtr.Get();
	}

	return SoftObjectPtr.LoadSynchronous();
}

FActiveEffectPackHandle UFXManager::PlayEffectAtLocation(AActor* SourceActor, AActor* TargetActor,
	FEffectPack& EffectPack, EEffectActivationType ActivationType, FTransform Transform)
{
	if(!EffectPack.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect Pack is Empty."))
		return FActiveEffectPackHandle();
	}

	if(!SourceActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Source Actor is invalid!"))
		return FActiveEffectPackHandle();
	}

	FActiveEffectPack ActivePack = FActiveEffectPack(GenerateNewActivePackId(), SourceActor, TargetActor, nullptr, ActivationType);

	const FGameplayTagContainer SourceTags = GetActorTags(SourceActor);
	const FGameplayTagContainer TargetTags = GetActorTags(TargetActor);

	for(FVFXData& VfxData : EffectPack.VFXData)
	{
		/* Go to the next effect data if this one is unable to play */
		if(!VfxData.CanPlay(SourceTags, TargetTags))
		{
			continue;
		}

		ActivePack.AddActiveVFX(SpawnVFXDataAtLocation(VfxData, SourceActor, Transform));
	}

	for(FSFXData& SfxData : EffectPack.SFXData)
	{
		/* Go to the next effect data if this one is unable to play */
		if (!SfxData.CanPlay(SourceTags, TargetTags))
		{
			continue;
		}

		ActivePack.AddActiveSound(SpawnSFXDataAtLocation(SfxData, SourceActor, Transform));
	}

	if(ActivationType == EEffectActivationType::Active && ActivePack.IsActive())
	{
		ActiveEffectPacks.Add(ActivePack);
		return ActivePack.CreateHandle();
	}

	if(ActivePack.IsActive())
	{
		AddInstantPack(SourceActor, ActivePack);
		return ActivePack.CreateHandle();
	}

	return FActiveEffectPackHandle();
}

FActiveEffectPackHandle UFXManager::PlayEffectAttached(AActor* SourceActor, AActor* TargetActor,
	USceneComponent* AttachComponent, FEffectPack& EffectPack, EEffectActivationType ActivationType)
{
	if (!EffectPack.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Effect Pack is Empty."))
			return FActiveEffectPackHandle();
	}

	if (!SourceActor || !AttachComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Source Actor or Attach Component is invalid!"))
			return FActiveEffectPackHandle();
	}

	FActiveEffectPack ActivePack = FActiveEffectPack(GenerateNewActivePackId(), SourceActor, TargetActor, AttachComponent, ActivationType);

	const FGameplayTagContainer SourceTags = GetActorTags(SourceActor);
	const FGameplayTagContainer TargetTags = GetActorTags(TargetActor);

	for (FVFXData& VfxData : EffectPack.VFXData)
	{
		/* Go to the next effect data if this one is unable to play */
		if (!VfxData.CanPlay(SourceTags, TargetTags))
		{
			continue;
		}

		ActivePack.AddActiveVFX(SpawnVFXDataAtComponent(VfxData, SourceActor, AttachComponent));
	}

	for (FSFXData& SfxData : EffectPack.SFXData)
	{
		/* Go to the next effect data if this one is unable to play */
		if (!SfxData.CanPlay(SourceTags, TargetTags))
		{
			continue;
		}

		ActivePack.AddActiveSound(SpawnSFXDataAtComponent(SfxData, SourceActor, AttachComponent));
	}

	if (ActivationType == EEffectActivationType::Active && ActivePack.IsActive())
	{
		ActiveEffectPacks.Add(ActivePack);
		return ActivePack.CreateHandle();
	}

	if (ActivePack.IsActive())
	{
		AddInstantPack(SourceActor, ActivePack);
		return ActivePack.CreateHandle();
	}

	return FActiveEffectPackHandle();
}

void UFXManager::StopActivePack(const FActiveEffectPackHandle& Handle)
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

UFXSystemComponent* UFXManager::SpawnVFXDataAtLocation(const FVFXData VFXData, const AActor* SourceActor, const FTransform& Transform) const
{

	UFXSystemAsset* Asset = GetAssetLoaded<UFXSystemAsset>(VFXData.ParticleSystem);
	if(!Asset)
	{
		return nullptr;
	}

	const FVector Location = Transform.GetLocation() + VFXData.AttachmentData.RelativeTransform.GetLocation();
	const FRotator Rotation = FRotator(Transform.GetRotation() + VFXData.AttachmentData.RelativeTransform.GetRotation());
	const FVector Scale = Transform.GetScale3D() * VFXData.AttachmentData.RelativeTransform.GetScale3D();

	if(UParticleSystem* Cascade = Cast<UParticleSystem>(Asset))
	{
		return UGameplayStatics::SpawnEmitterAtLocation
		(SourceActor, Cascade, Location, Rotation, Scale, true);
	}

	if(UNiagaraSystem* Niagara = Cast<UNiagaraSystem>(Asset))
	{
		return Cast<UFXSystemComponent>(UNiagaraFunctionLibrary::SpawnSystemAtLocation(SourceActor, Niagara, 
			Location, Rotation, Scale, true, true));
	}

	return nullptr;
}

UAudioComponent* UFXManager::SpawnSFXDataAtLocation(const FSFXData SFXData, const AActor* SourceActor, const FTransform& Transform)
{
	USoundBase* Asset = GetAssetLoaded<USoundBase>(SFXData.Sound);

	if(!Asset)
	{
		return nullptr;
	}

	if(SFXData.AudioType == EAudioType::TwoDimensional)
	{
		return UGameplayStatics::SpawnSound2D(SourceActor, Asset);
	}

	if(SFXData.AudioType == EAudioType::ThreeDimensional)
	{
		const FVector Location = Transform.GetLocation() + SFXData.AttachmentData.RelativeTransform.GetLocation();
		const FRotator Rotation = FRotator(Transform.GetRotation() + SFXData.AttachmentData.RelativeTransform.GetRotation());
		return UGameplayStatics::SpawnSoundAtLocation(SourceActor, Asset, Location, Rotation);
	}

	return nullptr;
}

UFXSystemComponent* UFXManager::SpawnVFXDataAtComponent(const FVFXData VFXData, const AActor* SourceActor,
	USceneComponent* AttachComponent)
{
	UFXSystemAsset* Asset = GetAssetLoaded<UFXSystemAsset>(VFXData.ParticleSystem);
	if (!Asset)
	{
		return nullptr;
	}

	/* If our attach type is at socket location, return our effect at location instead of trying to attach */
	if(VFXData.AttachmentData.AttachType == EAttachType::AtSocketLocation)
	{
		return SpawnVFXDataAtLocation(VFXData, SourceActor, AttachComponent->GetSocketTransform(VFXData.AttachmentData.SocketName));
	}

	const FTransform RelativeTransform = VFXData.GetRelativeTransform();
	const EAttachLocation::Type AttachRule = GetAttachLocationType(VFXData.AttachmentData.AttachmentRule);
	

	if (UParticleSystem* Cascade = Cast<UParticleSystem>(Asset))
	{
		return UGameplayStatics::SpawnEmitterAttached(Cascade, AttachComponent, VFXData.AttachmentData.SocketName, RelativeTransform.GetLocation(),
			FRotator(RelativeTransform.GetRotation()), RelativeTransform.GetScale3D(), AttachRule, true, EPSCPoolMethod::AutoRelease, true);
	}

	if (UNiagaraSystem* Niagara = Cast<UNiagaraSystem>(Asset))
	{
		return Cast<UFXSystemComponent>(UNiagaraFunctionLibrary::SpawnSystemAttached(Niagara, AttachComponent, VFXData.AttachmentData.SocketName,
			RelativeTransform.GetLocation(), FRotator(RelativeTransform.GetRotation()),
			AttachRule, true, true, ENCPoolMethod::AutoRelease));
	}

	return nullptr;
}

UAudioComponent* UFXManager::SpawnSFXDataAtComponent(const FSFXData SFXData, const AActor* SourceActor,
	USceneComponent* AttachComponent)
{
	USoundBase* Asset = GetAssetLoaded<USoundBase>(SFXData.Sound);

	if (!Asset)
	{
		return nullptr;
	}

	/* If our attach type is at socket location or we are playing a generic two dimensional sound, play it at location instead of attached */
	if (SFXData.AttachmentData.AttachType == EAttachType::AtSocketLocation || SFXData.AudioType == EAudioType::TwoDimensional)
	{
		return SpawnSFXDataAtLocation(SFXData, SourceActor, AttachComponent->GetSocketTransform(SFXData.AttachmentData.SocketName));
	}

	const FTransform RelativeTransform = SFXData.GetRelativeTransform();
	const EAttachLocation::Type AttachRule = GetAttachLocationType(SFXData.AttachmentData.AttachmentRule);

	return UGameplayStatics::SpawnSoundAttached(Asset, AttachComponent, SFXData.AttachmentData.SocketName,
		RelativeTransform.GetLocation(), FRotator(RelativeTransform.GetRotation()), AttachRule, false);

}


FActiveEffectPack& UFXManager::GetActivePack(const FActiveEffectPackHandle& Handle)
{
	/* Get our active or instant pack array based on the activation data from our handle */
	TArray<FActiveEffectPack>& Array = Handle.GetPackType() == EEffectActivationType::Active ? ActiveEffectPacks : InstantEffectPacks;
	for (auto Iterator = ActiveEffectPacks.CreateConstIterator(); Iterator; ++Iterator)
	{
		FActiveEffectPack& Pack = Array[Iterator.GetIndex()];
		if (Pack.Id == Handle.GetId())
		{
			return Pack;
		}
	}

	FActiveEffectPack* Pack =  new FActiveEffectPack();
	return *Pack;
}

void UFXManager::AddInstantPack(const UObject* WorldContextObject, const FActiveEffectPack& ActivePack)
{
	InstantEffectPacks.Add(ActivePack);
	if(!WorldContextObject->GetWorld()->GetTimerManager().IsTimerActive(InstantPackTimerHandle))
	{
		InstantPackTimerHandle = WorldContextObject->GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UFXManager::ClearInstantPacks);
	}
}

void UFXManager::ClearInstantPacks()
{
	InstantEffectPacks.Empty();
}

int UFXManager::GenerateNewActivePackId()
{
	return Internal_NextId++;
}

FGameplayTagContainer UFXManager::GetActorTags(const AActor* Actor) const
{
	FGameplayTagContainer Container = FGameplayTagContainer::EmptyContainer;

	if(const IGameplayTagAssetInterface* Interface = Cast<IGameplayTagAssetInterface>(Actor))
	{
		Interface->GetOwnedGameplayTags(Container);
		return Container;
	}

	return Container;
}

EAttachLocation::Type UFXManager::GetAttachLocationType(const EAttachmentRule& Rule)
{
	switch (Rule)
	{
		case EAttachmentRule::KeepRelative:
			return EAttachLocation::KeepRelativeOffset;
		case EAttachmentRule::KeepWorld:
			return EAttachLocation::KeepWorldPosition;
		case EAttachmentRule::SnapToTarget:
			return EAttachLocation::SnapToTarget;
		default: return EAttachLocation::KeepRelativeOffset;
	}
}
