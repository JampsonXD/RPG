// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGBFL_MainFunctions.h"

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
