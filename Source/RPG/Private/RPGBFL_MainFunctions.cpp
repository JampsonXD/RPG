// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGBFL_MainFunctions.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "ActorPoolWorldSubsystem.h"
#include "RPG_GameSingleton.h"

TArray<FActiveGameplayEffectHandle> URPGBFL_MainFunctions::ApplyGameplayEffectToTargetWithTargetData(
	UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayEffect> GameplayEffect,
	FGameplayAbilityTargetDataHandle TargetData, int AbilityLevel, int Stacks)
{
	TArray<FActiveGameplayEffectHandle> Handles;
	if(!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Ability System Component passed into URPGBFL_MainFunctions::ApplyGameplayEffectToTargetWithTargetData"));
		return Handles;
	}

	if(!GameplayEffect)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Gameplay Effect Class passed into URPGBFL_MainFunctions::ApplyGameplayEffectToTargetWithTargetData"));
		return Handles;
	}

	if(!AbilitySystemComponent->IsOwnerActorAuthoritative())
	{
		return Handles;
	}
	
	for (TSharedPtr<FGameplayAbilityTargetData> Data : TargetData.Data)
	{
		if (Data.IsValid())
		{
			const FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, AbilityLevel, Context);
			SpecHandle.Data->StackCount = Stacks;
			Handles = Data->ApplyGameplayEffectSpec(*SpecHandle.Data.Get());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("URPGBFL_MainFunctions::ApplyGameplayEffectToTargetWithTargetData invalid target data passed in."));
		}
	}

	return Handles;
}

UObject* URPGBFL_MainFunctions::GetCurrentSourceObjectFromSpec(const FGameplayEffectSpec& Spec)
{
	return Spec.GetContext().GetSourceObject();
}

FGameplayEffectContainerSpec URPGBFL_MainFunctions::AddTargetDataToContainerSpec(
	FGameplayEffectContainerSpec& ContainerSpec, FGameplayAbilityTargetDataHandle TargetDataHandle)
{
	FGameplayEffectContainerSpec Container = ContainerSpec;
	Container.TargetData.Append(TargetDataHandle);
	return Container;
}

int URPGBFL_MainFunctions::AppendUnique(const TArray<UProperty*>& TargetArray, UPARAM(ref)TArray<UProperty*>& OutArray)
{
	int UniqueCount = 0;
	for(UProperty* Property : TargetArray)
	{
		if(OutArray.AddUnique(Property) > 0)
		{
			UniqueCount++;
		}
	}

	return UniqueCount;
}

bool URPGBFL_MainFunctions::GetFirstActorOfClassOrFirstActor(TArray<AActor*> ActorArray,
                                                             TSubclassOf<AActor> ActorFilter, AActor*& FoundActor)
{
	FoundActor = nullptr;

	// Validate we dont have an empty array
	if(ActorArray.IsEmpty())
	{
		return false;
	}
	
	for(AActor*& Actor : ActorArray)
	{
		if(!ActorFilter || Actor->IsA(ActorFilter))
		{
			FoundActor = Actor;
			return true;
		}
	}

	// Didn't find an actor of the filter type so return the first actor in our array
	FoundActor = ActorArray[0];
	return true;
}

bool URPGBFL_MainFunctions::AddActorToPool(AActor* TargetActor)
{
	if(!TargetActor)
	{
		return false;
	}

	if(UActorPoolWorldSubsystem* Subsystem = UActorPoolWorldSubsystem::GetActorPoolWorldSubsystem(TargetActor))
	{
		return Subsystem->AddActorToPool(TargetActor);	
	}

	return false;
}

FActiveEffectPackHandle URPGBFL_MainFunctions::PlayEffectPackAtLocation(FEffectPack EffectPack, AActor* SourceActor,
                                                                        AActor* TargetActor, EEffectActivationType ActivationType, FTransform Transform)
{
	if(UFXManagerSubsystem* Manager = UFXManagerSubsystem::GetFXManager()) return Manager->PlayEffectAtLocation(SourceActor, TargetActor, EffectPack, ActivationType, Transform);

	return FActiveEffectPackHandle();
}

FActiveEffectPackHandle URPGBFL_MainFunctions::PlayEffectPackAttached(FEffectPack EffectPack, AActor* SourceActor,
	AActor* TargetActor, EEffectActivationType ActivationType, USceneComponent* AttachComponent)
{
	if (UFXManagerSubsystem* Manager = UFXManagerSubsystem::GetFXManager()) return Manager->PlayEffectAttached(SourceActor, TargetActor, AttachComponent, EffectPack, ActivationType);

	return FActiveEffectPackHandle();
}

void URPGBFL_MainFunctions::StopActiveEffectPackWithHandle(FActiveEffectPackHandle& Handle)
{
	if (UFXManagerSubsystem* Manager = UFXManagerSubsystem::GetFXManager()) Manager->StopActivePack(Handle);
}

void URPGBFL_MainFunctions::AddForceFeedbackOnController(AController* Controller,
	UForceFeedbackEffect* ForceFeedbackEffect, bool bIsLooping)
{
	if(!ForceFeedbackEffect)
	{
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if(PlayerController)
	{
		FForceFeedbackParameters Params;
		Params.bLooping = bIsLooping;
		PlayerController->ClientPlayForceFeedback(ForceFeedbackEffect, Params);
	}
}

void URPGBFL_MainFunctions::AddLooseGameplayTagsToActor(AActor* TargetActor, const FGameplayTagContainer Tags)
{
	const IAbilitySystemInterface* Interface = Cast<IAbilitySystemInterface>(TargetActor);

	if(UAbilitySystemComponent* ASC = Interface ? Interface->GetAbilitySystemComponent() : nullptr)
	{
		ASC->AddLooseGameplayTags(Tags);
	}
}

void URPGBFL_MainFunctions::RemoveLooseGameplayTagsToActor(AActor* TargetActor, const FGameplayTagContainer Tags)
{
	const IAbilitySystemInterface* Interface = Cast<IAbilitySystemInterface>(TargetActor);

	if (UAbilitySystemComponent* ASC = Interface ? Interface->GetAbilitySystemComponent() : nullptr)
	{
		ASC->RemoveLooseGameplayTags(Tags);
	}
}
