// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetType.h"

TArray<AActor*> UCollectionType::GatherActorsForSensing_Implementation(USensingSystemComponent* SensingSystemComponent, AActor* SensingSystemOwner)
{
	return TArray<AActor*>();
}

bool UFilterSensingData::FilterActorForSensing(USensingSystemComponent* SensingSystemComponent,
	AActor* SensingSystemOwner, const AActor* TargetActor) const
{
	return true;
}
