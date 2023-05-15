#pragma once
#include "FXTypes.h"
#include "Particles/ParticleSystemComponent.h"

struct SpawnerConstants
{
	static EAttachLocation::Type GetAttachLocationType(const EAttachmentRule& Rule);

private:
	static const TMap<EAttachmentRule, EAttachLocation::Type> AttachmentMap;
};


#pragma region INTERFACES

template<typename EffectType, class DataType>
class SpawnEffects
{
public:
	virtual ~SpawnEffects() = default;
	virtual EffectType* Spawn(const DataType Data, const AActor* SourceActor, const AActor* TargetActor) = 0;
};

class FXSystemComponentAttachedSpawner
{
public:
	virtual ~FXSystemComponentAttachedSpawner() = default;
	virtual UFXSystemComponent* Spawn(UFXSystemAsset* Asset, USceneComponent* AttachToComponent,
		FName AttachPointName, FVector Location, FRotator Rotation, FVector Scale, EAttachLocation::Type LocationType, bool bAutoDestroy, bool bAutoActivate) = 0;
};

#pragma endregion INTERFACES

#pragma region VFX

class FXSystemSpawnAttachedFactory
{
public:
	virtual ~FXSystemSpawnAttachedFactory() = default;
	virtual TSharedPtr<FXSystemComponentAttachedSpawner> Create(UFXSystemAsset* Asset);
};

class NiagaraComponentAttachedSpawner: public FXSystemComponentAttachedSpawner
{
public:
	virtual UFXSystemComponent* Spawn(UFXSystemAsset* Asset, USceneComponent* AttachToComponent,
		FName AttachPointName, FVector Location, FRotator Rotation, FVector Scale, EAttachLocation::Type LocationType, bool bAutoDestroy, bool bAutoActivate) override;
};

class CascadeComponentAttachedSpawner: public FXSystemComponentAttachedSpawner
{
public:
	virtual UFXSystemComponent* Spawn(UFXSystemAsset* Asset, USceneComponent* AttachToComponent,
		FName AttachPointName, FVector Location, FRotator Rotation, FVector Scale, EAttachLocation::Type LocationType, bool bAutoDestroy, bool bAutoActivate) override;
};

class SpawnVisualEffectsAtLocation: public SpawnEffects<UFXSystemComponent, FVFXData>
{
public:

	SpawnVisualEffectsAtLocation(FTransform location): Transform(location){}
	
	virtual UFXSystemComponent* Spawn(const FVFXData Data, const AActor* SourceActor, const AActor* TargetActor) override;

private:
	FTransform Transform;
};

class SpawnVisualEffectsAtComponent: public SpawnEffects<UFXSystemComponent, FVFXData>
{
public:
	SpawnVisualEffectsAtComponent(USceneComponent* AttachComponent): SceneComponent(AttachComponent){}

	virtual UFXSystemComponent* Spawn(const FVFXData Data, const AActor* SourceActor, const AActor* TargetActor) override;

private:

	USceneComponent* SceneComponent;
};
#pragma endregion VFX

#pragma region SFX
class SpawnSoundEffectsAtLocation: public SpawnEffects<UAudioComponent, FSFXData>
{
public:
	SpawnSoundEffectsAtLocation(FTransform Location): Transform(Location){}
	
	virtual UAudioComponent* Spawn(const FSFXData Data, const AActor* SourceActor, const AActor* TargetActor) override;

private:

	FTransform Transform;
};

class SpawnSoundEffectsAttached: public SpawnEffects<UAudioComponent, FSFXData>
{
public:
	SpawnSoundEffectsAttached(USceneComponent* AttachComponent): SceneComponent(AttachComponent){}
	
	virtual UAudioComponent* Spawn(const FSFXData Data, const AActor* SourceActor, const AActor* TargetActor) override;

private:
	USceneComponent* SceneComponent;
};
#pragma endregion SFX

