// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AbilityTasks/AT_WaitForMultiOverlap.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
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
		TimerHandle = World->GetTimerManager().SetTimerForNextTick(TimerDelegate);
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
}

void UAT_WaitForMultiOverlap::Update()
{
	TArray<AActor*> NewOverlappingActors;
	bool bFoundOverlappingActors = GetOverlappingActorsFromSphereTrace(NewOverlappingActors);

	if(OverlappingActors.IsEmpty() && !bFoundOverlappingActors)
	{
		
	}
}

bool UAT_WaitForMultiOverlap::GetOverlappingActorsFromSphereTrace(TArray<AActor*>& InOverlappingActors) const
{
	AActor* AvatarActor = GetAvatarActor();
	
	if(!AvatarActor)
	{
		return false;
	}
	
	UKismetSystemLibrary::SphereOverlapActors(AvatarActor, AvatarActor->GetActorLocation(), SphereRadius, OverlapCollisionChannels, OptionalActorFilter, IgnoreActors, InOverlappingActors);

	return InOverlappingActors.Num() > 0;
}

UAT_WaitForMultiOverlap* UAT_WaitForMultiOverlap::WaitForMultiSphereOverlap(UGameplayAbility* OwningAbility,
	FName TaskInstanceName, float Radius, TArray<TEnumAsByte<EObjectTypeQuery>> CollisionChannels,
	TSubclassOf<AActor> OptionalActorClassFilter, TArray<AActor*> ActorsToIgnore)
{
	UAT_WaitForMultiOverlap* Object = NewAbilityTask<UAT_WaitForMultiOverlap>(OwningAbility, TaskInstanceName);
	Object->SphereRadius = Radius;
	Object->OverlapCollisionChannels = CollisionChannels;
	Object->OptionalActorFilter = OptionalActorClassFilter;
	Object->IgnoreActors = ActorsToIgnore;
	return Object;
}
