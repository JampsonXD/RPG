// Fill out your copyright notice in the Description page of Project Settings.


#include "BPFL_SensingSystem.h"
#include "SensingSystemInterface.h"

USensingSystemComponent* UBPFL_SensingSystem::GetSensingSystemComponentFromActor(const AActor* TargetActor)
{
	if(const ISensingSystemInterface* Interface = Cast<ISensingSystemInterface>(TargetActor))
	{
		return Interface->GetSensingSystemComponent();
	}

	return nullptr;
}
