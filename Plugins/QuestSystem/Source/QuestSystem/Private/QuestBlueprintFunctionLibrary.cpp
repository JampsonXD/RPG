// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestBlueprintFunctionLibrary.h"

#include "QuestSystemInterface.h"

UQuestSystemComponent* UQuestBlueprintFunctionLibrary::GetQuestSystemComponent(const AActor* InActor)
{
	if(!InActor)
	{
		return nullptr;
	}

	const IQuestSystemInterface* Interface = Cast<IQuestSystemInterface>(InActor);
	if(!Interface)
	{
		return nullptr;
	}
	
	return Interface->GetQuestSystemComponent();
}
