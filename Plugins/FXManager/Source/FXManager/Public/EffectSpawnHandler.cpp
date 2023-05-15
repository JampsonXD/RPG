#include "EffectSpawnHandler.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"

class UNiagaraSystemComponent;

const TMap<EAttachmentRule, EAttachLocation::Type> SpawnerConstants::AttachmentMap = {
	{EAttachmentRule::SnapToTarget, EAttachLocation::Type::SnapToTarget},
	{EAttachmentRule::KeepRelative, EAttachLocation::Type::KeepRelativeOffset},
	{EAttachmentRule::KeepWorld, EAttachLocation::Type::KeepWorldPosition}
};

EAttachLocation::Type SpawnerConstants::GetAttachLocationType(const EAttachmentRule& Rule)
{
	return AttachmentMap.FindRef(Rule);
}

TSharedPtr<FXSystemComponentAttachedSpawner> FXSystemSpawnAttachedFactory::Create(UFXSystemAsset* Asset)
{
	if(UParticleSystem* Cascade = Cast<UParticleSystem>(Asset))
	{
		return MakeShared<CascadeComponentAttachedSpawner>();
	}

	if(UNiagaraSystem* Niagara = Cast<UNiagaraSystem>(Asset))
	{
		return MakeShared<NiagaraComponentAttachedSpawner>();
	}

	check(nullptr)
	return {};
}

UFXSystemComponent* NiagaraComponentAttachedSpawner::Spawn(UFXSystemAsset* Asset,
                                                                 USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, FRotator Rotation, FVector Scale,
                                                                 EAttachLocation::Type LocationType, bool bAutoDestroy, bool bAutoActivate)
{
	UNiagaraSystem* Niagara = Cast<UNiagaraSystem>(Asset);
	check(Asset)
	
	return Cast<UFXSystemComponent>(UNiagaraFunctionLibrary::SpawnSystemAttached(Niagara, AttachToComponent, AttachPointName,
			Location, Rotation, Scale,
			LocationType, bAutoDestroy, ENCPoolMethod::AutoRelease, bAutoActivate, true));
}

UFXSystemComponent* CascadeComponentAttachedSpawner::Spawn(UFXSystemAsset* Asset,
	USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, FRotator Rotation, FVector Scale,
	EAttachLocation::Type LocationType, bool bAutoDestroy, bool bAutoActivate)
{
	UParticleSystem* Cascade = Cast<UParticleSystem>(Asset);
	check(Asset)
	
	return UGameplayStatics::SpawnEmitterAttached(Cascade, AttachToComponent, AttachPointName, Location,
			Rotation, Scale, LocationType, bAutoDestroy, EPSCPoolMethod::AutoRelease, bAutoActivate);
}

UFXSystemComponent* SpawnVisualEffectsAtLocation::Spawn(const FVFXData Data, const AActor* SourceActor, const AActor* TargetActor)
{
	UFXSystemAsset* Asset = Data.ParticleSystem;
	if(!Asset)
	{
		return nullptr;
	}

	const FVector Location = Transform.GetLocation() + Data.AttachmentData.RelativeTransform.GetLocation();
	const FRotator Rotation = FRotator(Transform.GetRotation() + Data.AttachmentData.RelativeTransform.GetRotation());
	const FVector Scale = Transform.GetScale3D() * Data.AttachmentData.RelativeTransform.GetScale3D();

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

UFXSystemComponent* SpawnVisualEffectsAtComponent::Spawn(const FVFXData Data, const AActor* SourceActor, const AActor* TargetActor)
{
	UFXSystemAsset* Asset = Data.ParticleSystem;
	if (!Asset)
	{
		return nullptr;
	}

	/* If our attach type is at socket location, return our effect at location instead of trying to attach */
	if(Data.AttachmentData.AttachType == EAttachType::AtSocketLocation)
	{
		SpawnVisualEffectsAtLocation SpawnAtLocation = SpawnVisualEffectsAtLocation(SceneComponent->GetSocketTransform(Data.AttachmentData.SocketName));
		return SpawnAtLocation.Spawn(Data, SourceActor, TargetActor);
	}

	const FTransform& RelativeTransform = Data.GetRelativeTransform();
	const EAttachLocation::Type AttachRule = SpawnerConstants::GetAttachLocationType(Data.AttachmentData.AttachmentRule);
	
	FXSystemSpawnAttachedFactory Factory = FXSystemSpawnAttachedFactory();
	const auto Spawner = Factory.Create(Asset);

	return Spawner.Get()->Spawn(Asset, SceneComponent, Data.AttachmentData.SocketName, RelativeTransform.GetLocation(),
			FRotator(RelativeTransform.GetRotation()), RelativeTransform.GetScale3D(), AttachRule, true, true);
}

UAudioComponent* SpawnSoundEffectsAtLocation::Spawn(const FSFXData Data, const AActor* SourceActor, const AActor* TargetActor)
{
	USoundBase* Asset = Data.Sound;
	if(!Asset)
	{
		return nullptr;
	}

	if(Data.AudioType == EAudioType::TwoDimensional)
	{
		return UGameplayStatics::SpawnSound2D(SourceActor, Asset);
	}

	if(Data.AudioType == EAudioType::ThreeDimensional)
	{
		const FVector Location = Transform.GetLocation() + Data.AttachmentData.RelativeTransform.GetLocation();
		const FRotator Rotation = FRotator(Transform.GetRotation() + Data.AttachmentData.RelativeTransform.GetRotation());
		return UGameplayStatics::SpawnSoundAtLocation(SourceActor, Asset, Location, Rotation);
	}

	return nullptr;
}

UAudioComponent* SpawnSoundEffectsAttached::Spawn(const FSFXData Data, const AActor* SourceActor, const AActor* TargetActor)
{
	USoundBase* Asset = Data.Sound;
	if (!Asset)
	{
		return nullptr;
	}

	/* If our attach type is at socket location or we are playing a generic two dimensional sound, play it at location instead of attached */
	if (Data.AttachmentData.AttachType == EAttachType::AtSocketLocation || Data.AudioType == EAudioType::TwoDimensional)
	{
		SpawnSoundEffectsAtLocation SpawnAtLocation = SpawnSoundEffectsAtLocation(SceneComponent->GetSocketTransform(Data.AttachmentData.SocketName));
		return SpawnAtLocation.Spawn(Data, SourceActor, TargetActor);
	}

	const FTransform& RelativeTransform = Data.GetRelativeTransform();
	const EAttachLocation::Type AttachRule = SpawnerConstants::GetAttachLocationType(Data.AttachmentData.AttachmentRule);

	return UGameplayStatics::SpawnSoundAttached(Asset, SceneComponent, Data.AttachmentData.SocketName,
		RelativeTransform.GetLocation(), FRotator(RelativeTransform.GetRotation()), AttachRule, false);
}
