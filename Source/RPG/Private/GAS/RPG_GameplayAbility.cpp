// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/RPG_GameplayAbility.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "ActorPoolWorldSubsystem.h"
#include "RPG_Character.h"
#include "Camera/CameraComponent.h"
#include "GAS/RPG_AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

EAbilityInput URPG_GameplayAbility::GetInputID() const
{
	return InputID;
}

bool URPG_GameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags) && RPGCheckCost(Handle, *ActorInfo);
}

void URPG_GameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	RPGApplyCost(Handle, *ActorInfo, ActivationInfo);
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);
}

bool URPG_GameplayAbility::RPGCheckCost_Implementation(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo ActorInfo) const
{
	return true;
}

void URPG_GameplayAbility::RPGApplyCost_Implementation(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{

}


FGameplayEffectContainerSpec URPG_GameplayAbility::MakeEffectContainerSpecFromContainer(
	const FGameplayEffectContainer& Container, const FGameplayEventData& EventData, int AbilityLevel)
{
	FGameplayEffectContainerSpec ContainerSpec;
	const AActor* OwningActor = GetOwningActorFromActorInfo();

	if(UAbilitySystemComponent* OwningASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
	{
		// Add our targets if our targeting type is valid
		if(Container.TargetingType.Get())
		{
			TArray<FHitResult> TargetHitResults;
			TArray<AActor*> TargetActors;
			const URPG_TargetTypes* TargetingCDO =  Container.TargetingType.GetDefaultObject();
			AActor* TargetingActor = GetAvatarActorFromActorInfo();
			ARPG_Character* TargetingCharacter = Cast<ARPG_Character>(TargetingActor);
			TargetingCDO->GetTargets(TargetingCharacter,TargetingActor, EventData, TargetHitResults, TargetActors);
			ContainerSpec.AddTargets(TargetHitResults, TargetActors);
		}

		// Check to see if we are not overriding the ability level, if so just grab the GAB's ability level
		if(AbilityLevel == INDEX_NONE)
		{
			AbilityLevel = GetAbilityLevel();
		}

		// Create our Gameplay Effect Spec Handles
		for (const TSubclassOf<UGameplayEffect>& GameplayEffectClass : Container.TargetedEffects)
		{
			ContainerSpec.TargetGameplayEffectSpecs.Add(MakeOutgoingGameplayEffectSpec(GameplayEffectClass, AbilityLevel));
		}
	}

	return ContainerSpec;
}

FGameplayEffectContainerSpec URPG_GameplayAbility::MakeEffectContainerSpecFromEffectMap(
	const FGameplayTag ContainerTag, const FGameplayEventData& EventData, int AbilityLevel)
{
	if(const FGameplayEffectContainer* Container = EffectMap.Find(ContainerTag))
	{
		return MakeEffectContainerSpecFromContainer(*Container, EventData, AbilityLevel);
	}
	return FGameplayEffectContainerSpec();
}

TArray<FActiveGameplayEffectHandle> URPG_GameplayAbility::ApplyGameplayEffectContainerSpec(const FGameplayEffectContainerSpec& ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> ActiveGameplayEffectHandles;
	for(const FGameplayEffectSpecHandle& EffectSpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	{
		ActiveGameplayEffectHandles.Append(K2_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, ContainerSpec.TargetData));
	}
	
	return ActiveGameplayEffectHandles;
}

bool URPG_GameplayAbility::MakeAndApplyGameplayEffectContainerSpecFromEffectMap(
	TArray<FActiveGameplayEffectHandle>& ActiveEffectHandles, const FGameplayTag ContainerTag,
	const FGameplayEventData& EventData, int AbilityLevel)
{
	const FGameplayEffectContainerSpec Container =  MakeEffectContainerSpecFromEffectMap(ContainerTag, EventData, AbilityLevel);
	if(Container.HasValidEffects() && Container.HasValidTargets())
	{
		ActiveEffectHandles = ApplyGameplayEffectContainerSpec(Container);
		return true;
	}

	return false;
}

UParticleSystemComponent* URPG_GameplayAbility::AddCameraEffectToOwner(UParticleSystem* EmitterTemplate,
	FTransform Transform, bool bRemoveOnAbilityEnd)
{
	// Only spawn and attach our camera effect to the owner if we have a emitter template
	if(!EmitterTemplate)
	{
		return nullptr;
	}

	// Try getting our Camera Component, returns the first camera component in our component list
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	
	if(UCameraComponent* CameraComponent = AvatarActor ? Cast<UCameraComponent>(AvatarActor->GetComponentByClass(UCameraComponent::StaticClass())) : nullptr)
	{
		UParticleSystemComponent* Particle = UGameplayStatics::SpawnEmitterAttached(EmitterTemplate, CameraComponent, FName(TEXT("None")),
			Transform.GetLocation(), Transform.Rotator(), Transform.GetScale3D(), EAttachLocation::KeepRelativeOffset);

		// Add to our array of camera effects if we need to remove on end
		if(bRemoveOnAbilityEnd)
		{
			CameraEffects.Add(Particle);
		}

		return Particle;
	}

	return nullptr;
}

void URPG_GameplayAbility::RemoveCameraEffectFromOwner(UParticleSystemComponent* ParticleSystemComponent)
{
	if(!ParticleSystemComponent)
	{
		return;
	}

	ParticleSystemComponent->DestroyComponent();
}

ARPG_Projectile* URPG_GameplayAbility::RequestProjectile(TSubclassOf<ARPG_Projectile> ActorClass, FVector Velocity,
	FVector Location, FRotator Rotation)
{
	return RequestProjectile_Internal(ActorClass, Velocity, Location, Rotation);
}

ARPG_Projectile* URPG_GameplayAbility::RequestProjectileFromSocketLocation(TSubclassOf<ARPG_Projectile> ActorClass,
	FVector Velocity, FName SocketName, FRotator Rotation)
{
	const FGameplayAbilityActorInfo ActorInfo = GetActorInfo();
	if(ActorInfo.SkeletalMeshComponent.IsValid())
	{
		const USkeletalMeshComponent* SkeletalMeshComponent = ActorInfo.SkeletalMeshComponent.Get();
		return RequestProjectile_Internal(ActorClass, Velocity, SkeletalMeshComponent->GetSocketLocation(SocketName), Rotation);
	}

	return nullptr;
}

ARPG_Projectile* URPG_GameplayAbility::RequestProjectile_Internal(const TSubclassOf<ARPG_Projectile>& ActorClass,
	const FVector& Velocity, const FVector& Location, const FRotator& Rotation) const
{
	if (!ActorClass)
	{
		return nullptr;
	}

	if (UActorPoolWorldSubsystem* PoolingSystem = UActorPoolWorldSubsystem::GetActorPoolWorldSubsystem(this))
	{
		FActorPopData PopData;
		PopData.Owner = GetAvatarActorFromActorInfo();
		PopData.Velocity = Velocity;
		PopData.Rotation = Rotation;
		PopData.Location = Location;
		PopData.Magnitude = -1.f;
		PopData.OptionalObject = GetAbilitySystemComponentFromActorInfo();
		PopData.OptionalObject2 = this;

		return PoolingSystem->RequestActorFromPool<ARPG_Projectile>(ActorClass, PopData);
	}

	return nullptr;
}

ARPG_Character* URPG_GameplayAbility::GetRPGCharacter() const
{
	return RPGCharacter;
}

URPG_AbilitySystemComponent* URPG_GameplayAbility::GetRPGAbilitySystemComponent() const
{
	return RPGAbilitySystemComponent;
}

void URPG_GameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if(ActorInfo->AvatarActor.IsValid())
	{
		RPGCharacter = Cast<ARPG_Character>(ActorInfo->AvatarActor.Get());
	}

	if(ActorInfo->AbilitySystemComponent.IsValid())
	{
		RPGAbilitySystemComponent = Cast<URPG_AbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
	}

	if(bActivateOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

void URPG_GameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	// Remove any camera effects waiting for ability end
	if(!CameraEffects.IsEmpty())
	{
		for(UParticleSystemComponent*& ParticleSystemComponent : CameraEffects)
		{
			ParticleSystemComponent->DestroyComponent();
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
