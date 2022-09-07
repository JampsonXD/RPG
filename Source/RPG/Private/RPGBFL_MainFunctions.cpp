// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGBFL_MainFunctions.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "RPG_GameSingleton.h"

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

UFXManager* URPGBFL_MainFunctions::GetRPGFXManager()
{
	if(URPG_GameSingleton* Singleton = URPG_GameSingleton::GetSingleton())
	{
		return Singleton->GetFXManager();
	}

	return nullptr;
}

bool URPGBFL_MainFunctions::GetFirstActorOfClassOrFirstActor(TArray<AActor*> ActorArray,
                                                             TSubclassOf<AActor> ActorFilter, AActor*& FoundActor)
{
	FoundActor = nullptr;

	// Validate we dont have an empty array
	if(ActorArray.Num() < 1)
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

FActiveEffectPackHandle URPGBFL_MainFunctions::PlayEffectPackAtLocation(FEffectPack EffectPack, AActor* SourceActor,
	AActor* TargetActor, EEffectActivationType ActivationType, FTransform Transform)
{
	return URPG_GameSingleton::GetSingleton()->GetFXManager()->PlayEffectAtLocation(SourceActor, TargetActor, EffectPack, ActivationType, Transform);
}

FActiveEffectPackHandle URPGBFL_MainFunctions::PlayEffectPackAttached(FEffectPack EffectPack, AActor* SourceActor,
	AActor* TargetActor, EEffectActivationType ActivationType, USceneComponent* AttachComponent)
{
	return URPG_GameSingleton::GetSingleton()->GetFXManager()->PlayEffectAttached(SourceActor, TargetActor, AttachComponent, EffectPack, ActivationType);
}

void URPGBFL_MainFunctions::StopActiveEffectPackWithHandle(FActiveEffectPackHandle& Handle)
{
	URPG_GameSingleton::GetSingleton()->GetFXManager()->StopActivePack(Handle);
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
