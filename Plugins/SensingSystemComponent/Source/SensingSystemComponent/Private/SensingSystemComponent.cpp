// Fill out your copyright notice in the Description page of Project Settings.


#include "SensingSystemComponent.h"

USensingSystemComponent::USensingSystemComponent()
{
	SensingActorPreAllocatedSlots = 10;

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void USensingSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// Verify our collection class is valid
	ensure(GatherTargetCollections.Num() > 0);
}

void USensingSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateSensingActors();
}

void USensingSystemComponent::UpdateSensingActors()
{
	TArray<AActor*> OldSensingActors = SensingActors;
	TArray<AActor*> NewSensingActors;
	// Gather all newly sensed actors
	for (const auto Collection : GatherTargetCollections)
	{
		NewSensingActors.Append(Collection->GatherActorsForSensing(this, GetOwner()));
	}

	/* If we gathered any nearby sensed actors and any met our filters criteria */
	if(!NewSensingActors.IsEmpty())
	{
		NewSensingActors = FilterSensingActors(NewSensingActors);
		// Remove any actors we sensed before and are still sensing
		for(int i = 0; i < NewSensingActors.Num() && OldSensingActors.Num() > 0; i++)
		{
			AActor* IndexActor = NewSensingActors[i];
			if(OldSensingActors.FindByPredicate([IndexActor](const AActor* OldActor)
			{
					return OldActor == IndexActor;
			}))
			{
				OldSensingActors.RemoveSwap(IndexActor);
				NewSensingActors.RemoveSwap(IndexActor);
				i--;
			}
		}

		if(!NewSensingActors.IsEmpty())
		{
			for(int i = 0; i < NewSensingActors.Num(); i++)
			{
				AddSensingActor(NewSensingActors[i]);
			}
		}
	}

	if(!OldSensingActors.IsEmpty())
	{
		for(int i = 0; i < OldSensingActors.Num(); i++)
		{
			RemoveSensingActor(OldSensingActors[i]);
		}
	}
}

TArray<AActor*> USensingSystemComponent::FilterSensingActors(const TArray<AActor*>& GatheredActors)
{
	/* Gathered actors will have at least one actor, verify our filter is valid or return early */
	if(SensingFilters.IsEmpty())
	{
		return GatheredActors;
	}

	TArray<AActor*> FilteredActors = GatheredActors;

	// Loop through each filter and keep actors that pass all tests
	for (const auto Filter : SensingFilters)
	{
		for (int i = 0; i < FilteredActors.Num(); i++)
		{
			const AActor* Actor = FilteredActors[i];
			if(!Filter->FilterActorForSensing(this, GetAvatarActor(), Actor) || !Filter->K2_FilterActorForSensing(this, GetAvatarActor(), Actor))
			{
				FilteredActors.RemoveAt(i);
				--i;
			}
		}
	}
	
	return FilteredActors;
}

void USensingSystemComponent::AddSensingActor(AActor* SensingActor)
{
	if(!SensingActor)
	{
		return;
	}

	SensingActors.Add(SensingActor);
	OnActorSensed.Broadcast(SensingActor);
}

void USensingSystemComponent::RemoveSensingActor(AActor* LostActor)
{
	if (!LostActor)
	{
		// Do something
	}

	SensingActors.RemoveSwap(LostActor, CanShrinkSensedActors() ? true : false);
	OnActorLost.Broadcast(LostActor);
}

AActor* USensingSystemComponent::GetAvatarActor() const
{
	return AvatarActor;
}

void USensingSystemComponent::InitializeSensingComponent(AActor* InAvatarActor)
{
	AvatarActor = InAvatarActor;

	PrimaryComponentTick.SetTickFunctionEnable(AvatarActor ? true : false);
}

TArray<AActor*> USensingSystemComponent::GetSensingActors() const
{
	return SensingActors;
}

bool USensingSystemComponent::CanShrinkSensedActors() const
{
	return SensingActors.Num() > SensingActorPreAllocatedSlots;
}

