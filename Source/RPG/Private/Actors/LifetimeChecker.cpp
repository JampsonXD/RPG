// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LifetimeChecker.h"

void ULifetimeChecker::Deinitialize()
{
	
}

void ULifetimeChecker::LifetimeCompleted()
{
	OnLifetimeCompleted.ExecuteIfBound();
}

ULifetimeChecker::ULifetimeChecker(const FObjectInitializer& ObjectInitializer)
{
	bIsTickable = false;
}

void ULifetimeChecker::Initialize(AActor* Actor)
{
	
}

void ULifetimeChecker::StartChecker(AActor* Actor)
{
	WatchingActor = Actor;
	Initialize(Actor);
	K2_Initialize(Actor);
}

void ULifetimeChecker::StopChecker()
{
	Deinitialize();
	K2_Deinitialize();
}

void ULifetimeChecker::Tick(float DeltaTime)
{
	
}
