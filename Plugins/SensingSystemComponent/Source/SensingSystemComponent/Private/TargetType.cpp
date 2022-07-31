// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetType.h"

TArray<AActor*> UCollectionType::GatherActorsForSensing_Implementation(USensingSystemComponent* SensingSystemComponent, AActor* SensingSystemOwner)
{
	return TArray<AActor*>();
}

TArray<AActor*> UFilterSensingData::FilterActorsForSensing(USensingSystemComponent* SensingSystemComponent,
	AActor* SensingSystemOwner, const TArray<AActor*>& TargetActors) const
{
	return TargetActors;
}
