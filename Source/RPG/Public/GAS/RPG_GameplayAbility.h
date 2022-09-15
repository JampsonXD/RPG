// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS_Types.h"
#include "Abilities/GameplayAbility.h"
#include "RPG_GameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPG_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()


	UPROPERTY()
	ARPG_Character* RPGCharacter;

	UPROPERTY()
	URPG_AbilitySystemComponent* RPGAbilitySystemComponent;


public:

	// Getter for our InputID
	enum EAbilityInput GetInputID() const;

protected:

	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Ability)
	bool RPGCheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo ActorInfo) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Ability)
	void RPGApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const;
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay Effect Container")
	FGameplayEffectContainerSpec MakeEffectContainerSpecFromContainer(const FGameplayEffectContainer& Container, const FGameplayEventData& EventData, int AbilityLevel = -1);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Effect Container")
	FGameplayEffectContainerSpec MakeEffectContainerSpecFromEffectMap(const FGameplayTag ContainerTag, const FGameplayEventData& EventData, int AbilityLevel = -1);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Effect Container")
	TArray<FActiveGameplayEffectHandle> ApplyGameplayEffectContainerSpec(const FGameplayEffectContainerSpec& ContainerSpec);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Effect Container", meta = (AutoCreateRefTerm = "ActiveEffectHandles"))
	bool MakeAndApplyGameplayEffectContainerSpecFromEffectMap(TArray<FActiveGameplayEffectHandle>& ActiveEffectHandles,
		const FGameplayTag ContainerTag,
		const FGameplayEventData& EventData,
		int AbilityLevel = -1);

	/** Spawns and attaches a Camera Effect to our players Camera Component
	 * @param EmitterTemplate : Particle System we want to add to our camera
	 * @param Location : Relative Location to our camera
	 * @param Rotation : Relative Rotation to our camera
	 * @param Scale : Scale/Size of our emitter
	 * @return Pointer to our spawned particle system
	 **/
	UFUNCTION(BlueprintCallable, Category = "RPG Gameplay Ability | Camera")
	UParticleSystemComponent* AddCameraEffectToOwner(UParticleSystem* EmitterTemplate,
	FTransform Transform, bool bRemoveOnAbilityEnd = false);

	UFUNCTION(BlueprintCallable, Category = "RPG Gameplay Ability | Camera")
	void RemoveCameraEffectFromOwner(UParticleSystemComponent* ParticleSystemComponent);

	UFUNCTION(BlueprintPure, Category = "RPG Gameplay Ability")
	ARPG_Character* GetRPGCharacter() const;

	UFUNCTION(BlueprintPure, Category = "RPG Gameplay Ability")
	URPG_AbilitySystemComponent* GetRPGAbilitySystemComponent() const;

protected:
	
	// Map for our various Effect Containers
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect Map")
	TMap<FGameplayTag, FGameplayEffectContainer> EffectMap;

	// Use this to set which input to assign this ability to
	UPROPERTY(EditDefaultsOnly, Category = "Ability Defaults")
	EAbilityInput InputID;

	// Whether to grant this ability as soon as its granted
	UPROPERTY(EditDefaultsOnly, Category = "Ability Defaults")
	bool bActivateOnGranted;

	// Array of currently activated camera effects
	TArray<UParticleSystemComponent*> CameraEffects;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
