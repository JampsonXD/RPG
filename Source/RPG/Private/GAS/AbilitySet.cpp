// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AbilitySet.h"

#include "GAS/RPG_AbilitySystemComponent.h"
#include "GAS/RPG_GameplayAbility.h"


void FAbilitySetActiveHandle::Remove()
{
	RemoveAbilities();
	RemoveActiveGameplayEffects();
	RemoveAttributeSets();

	AbilityHandles.Reset();
	GameplayEffectHandles.Reset();
	AttributeSets.Reset();
}

UAbilitySet::UAbilitySet(const FObjectInitializer& ObjectInitializer)
{
	
}

void FAbilitySetActiveHandle::RemoveAbilities()
{
	for(int index = 0; index < AbilityHandles.Num(); index++)
	{
		FGameplayAbilitySpecHandle& Handle = AbilityHandles[index];
		AbilitySystemComponent.Get()->CancelAbilityHandle(Handle);
	}
}

void FAbilitySetActiveHandle::RemoveActiveGameplayEffects()
{
	for(int index = 0; index < GameplayEffectHandles.Num(); index++)
	{
		const FActiveGameplayEffectHandle& Handle = GameplayEffectHandles[index];
		AbilitySystemComponent.Get()->RemoveActiveGameplayEffect(Handle);
	}
}

void FAbilitySetActiveHandle::RemoveAttributeSets()
{
	for(int index = 0; index < AttributeSets.Num(); index++)
	{
		if(AttributeSets[index].IsValid())
		{
			UAttributeSet* AttributeSet = AttributeSets[index].Get();
			AbilitySystemComponent->GetSpawnedAttributes_Mutable().Remove(AttributeSet);
		}
	}
}

FAbilitySetActiveHandle UAbilitySet::AddAbilitySet(UAbilitySystemComponent* AbilitySystemComponent,
                                                   UObject* SourceObject) const
{
	if(!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability System Component passed into object %s is invalid!"), *GetNameSafe(this));
		return FAbilitySetActiveHandle();
	}

	FAbilitySetActiveHandle Handle;

	AddAttributeSets(AbilitySystemComponent, Handle);
	
	AddGameplayAbilities(AbilitySystemComponent, SourceObject, Handle);

	AddGameplayEffects(AbilitySystemComponent, SourceObject, Handle);

	TWeakObjectPtr<UAbilitySystemComponent> ObjectPtr(AbilitySystemComponent);
	Handle.SetAbilitySystemComponent(ObjectPtr);

	return Handle;
}

void UAbilitySet::AddGameplayAbilities(UAbilitySystemComponent* AbilitySystemComponent, UObject* SourceObject, FAbilitySetActiveHandle& Handle) const
{
	for(int index = 0; index < GameplayAbilities.Num(); index++)
	{
		const FRPG_GameplayAbilityInfo& CurrentAbility = GameplayAbilities[index];
		
		if(!IsValid(CurrentAbility.GameplayAbility))
		{
			UE_LOG(LogTemp, Warning, TEXT("Ability at index %d of %s is invalid!"), index, *GetNameSafe(this));
			continue;
		}
		
		FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(CurrentAbility.GameplayAbility, CurrentAbility.AbilityLevel,
				static_cast<int32>(CurrentAbility.GameplayAbility.GetDefaultObject()->GetInputID()), SourceObject));

		Handle.AddAbilitySpecHandle(SpecHandle);
	}
}

void UAbilitySet::AddGameplayEffects(UAbilitySystemComponent* AbilitySystemComponent,
                                     const UObject* SourceObject, FAbilitySetActiveHandle& Handle) const
{
	// Loop through our Gameplay Effects we want to grant and add each to our Ability System Component
	for(int index = 0; index < GameplayEffects.Num(); index++)
	{
		const FRPG_GameplayEffectInfo& CurrentGameplayEffect = GameplayEffects[index];
		
		if(!IsValid(CurrentGameplayEffect.GameplayEffect))
		{
			UE_LOG(LogTemp, Warning, TEXT("Gameplay Effect Class at index %d of %s is invalid!"), index, *GetNameSafe(this));
			continue;
		}
		
		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		EffectContextHandle.AddSourceObject(SourceObject);
		
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CurrentGameplayEffect.GameplayEffect, CurrentGameplayEffect.EffectLevel, EffectContextHandle);
		if(SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), AbilitySystemComponent);
			Handle.AddActiveGameplayEffectHandle(ActiveGameplayEffectHandle);
		}
	}
}

void UAbilitySet::AddAttributeSets(UAbilitySystemComponent* AbilitySystemComponent,
	FAbilitySetActiveHandle& Handle) const
{
	for(int index = 0; index < AttributeSets.Num(); index++)
	{
		const FRPG_AttributeSetInfo& CurrentAttributeSet = AttributeSets[index];

		if(!IsValid(CurrentAttributeSet.AttributeSet))
		{
			UE_LOG(LogTemp, Warning, TEXT("Attribute Set Class at index %d of %s is invalid!"), index, *GetNameSafe(this));
			continue;
		}

		UAttributeSet* AttributeSet = NewObject<UAttributeSet>(AbilitySystemComponent->GetOwner(),CurrentAttributeSet.AttributeSet);
		AbilitySystemComponent->AddAttributeSetSubobject(AttributeSet);

		// Add a weak object pointer to our handle
		TWeakObjectPtr<UAttributeSet> WeakObjectPtr(AttributeSet);
		Handle.AddAttributeSet(WeakObjectPtr);
	}
}
