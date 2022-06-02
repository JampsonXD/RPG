// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncTasks/AT_WaitTime.h"

UAT_WaitTime* UAT_WaitTime::WaitForTimePassed(UObject* InWorldContextObject, float TimeToWait, bool bCallOnce)
{
	UAT_WaitTime* Object = NewObject<UAT_WaitTime>();
	Object->WorldContextObject = InWorldContextObject;
	
	UWorld* World = InWorldContextObject->GetWorld();
	if(!World)
	{
		Object->RemoveFromRoot();
		return nullptr;
	}
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(Object, &UAT_WaitTime::TimerFinished, bCallOnce);

	// Make sure we are passing through a valid time
	TimeToWait = FMath::Max(TimeToWait, 0.01f);
	
	World->GetTimerManager().SetTimer(Object->TimerHandle, TimerDelegate, TimeToWait, bCallOnce);
	return Object;
}

void UAT_WaitTime::TimerFinished(bool bCallOnce)
{
	if(WaitFinished.IsBound())
	{
		WaitFinished.Broadcast();
	}

	if(bCallOnce)
	{
		EndTask();
	}
}

void UAT_WaitTime::EndTask()
{
	UWorld* World = WorldContextObject->GetWorld();
	if(World && World->GetTimerManager().TimerExists(TimerHandle))
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}

	SetReadyToDestroy();
	MarkAsGarbage();
}
