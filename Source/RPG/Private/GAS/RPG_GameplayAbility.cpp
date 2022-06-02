// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/RPG_GameplayAbility.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "RPG_Character.h"
#include "Camera/CameraComponent.h"
#include "GAS/RPG_AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

EAbilityInput URPG_GameplayAbility::GetInputID() const
{
	return InputID;
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

		// Check to see if we are not overriding the ability level, if so just grab the GAB's ability levl
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

void URPG_GameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

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
