// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AbilityTasks/AT_WaitForMultiOverlap.h"
#include "Kismet/KismetSystemLibrary.h"

UAT_WaitForMultiOverlap::UAT_WaitForMultiOverlap(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UAT_WaitForMultiOverlap::Activate()
{
	Super::Activate();

	if(const UWorld* World = GetWorld())
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &UAT_WaitForMultiOverlap::Update);
		World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, UpdateInterval, true);
		Update();
	}
}

void UAT_WaitForMultiOverlap::ExternalCancel()
{
	Super::ExternalCancel();
}

FString UAT_WaitForMultiOverlap::GetDebugString() const
{
	return Super::GetDebugString();
}

void UAT_WaitForMultiOverlap::OnDestroy(bool bAbilityEnded)
{
	Super::OnDestroy(bAbilityEnded);

	TimerHandle.Invalidate();
}

void UAT_WaitForMultiOverlap::Update()
{
	TArray<AActor*> NewOverlappingActors;
	bool bFoundOverlappingActors = GetOverlappingActorsFromSphereTrace(NewOverlappingActors);

	TArray<AActor*> RemovingActors;
	TArray<AActor*> AddingActors;

	// Get our adding actors
	for(int i = 0; i < OverlappingActors.Num(); i++)
	{
		AActor* Actor = OverlappingActors[i];

		// If the overlapping actor is not contained in the new array, we are going to be removing this actor
		if(!NewOverlappingActors.Contains(Actor))
		{
			RemovingActors.Add(Actor);
			OverlappingActors.Remove(Actor);
			i--;
		}
		// If both are contained, they aren't going to be added or removed
		else
		{
			NewOverlappingActors.Remove(Actor);
		}
	}

	if(RemovingActors.Num() > 0)
	{
		FGameplayAbilityTargetData_ActorArray* Data = new FGameplayAbilityTargetData_ActorArray();
		Data->TargetActorArray.Append(RemovingActors);
		FGameplayAbilityTargetDataHandle Handle;
		Handle.Add(Data);
		OnActorRemoved.Broadcast(Handle);
	}

	if(NewOverlappingActors.Num() > 0)
	{
		OverlappingActors.Append(NewOverlappingActors);
		FGameplayAbilityTargetData_ActorArray* Data = new FGameplayAbilityTargetData_ActorArray();
		Data->TargetActorArray.Append(NewOverlappingActors);
		FGameplayAbilityTargetDataHandle Handle;
		Handle.Add(Data);
		OnActorAdded.Broadcast(Handle);
	}
}

bool UAT_WaitForMultiOverlap::GetOverlappingActorsFromSphereTrace(TArray<AActor*>& InOverlappingActors) const
{
	AActor* AvatarActor = GetAvatarActor();
	
	if(!AvatarActor)
	{
		return false;
	}
	
	return UKismetSystemLibrary::SphereOverlapActors(AvatarActor, AvatarActor->GetActorLocation(), SphereRadius, OverlapCollisionChannels, OptionalActorFilter, IgnoreActors, InOverlappingActors);
}

UAT_WaitForMultiOverlap* UAT_WaitForMultiOverlap::WaitForMultiSphereOverlap(UGameplayAbility* OwningAbility,
	FName TaskInstanceName, float Radius, float UpdateTiming, TArray<TEnumAsByte<EObjectTypeQuery>> CollisionChannels,
	TSubclassOf<AActor> OptionalActorClassFilter, TArray<AActor*> ActorsToIgnore)
{
	UAT_WaitForMultiOverlap* Object = NewAbilityTask<UAT_WaitForMultiOverlap>(OwningAbility, TaskInstanceName);
	Object->SphereRadius = Radius;
	Object->OverlapCollisionChannels = CollisionChannels;
	Object->OptionalActorFilter = OptionalActorClassFilter;
	Object->IgnoreActors = ActorsToIgnore;
	Object->UpdateInterval = UpdateTiming;
	return Object;
}
