// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGBFL_MainFunctions.h"

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
