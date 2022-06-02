// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QuestBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:

	/** Returns a QuestSystemComponent from an Actor if IQuestSystemInterface is implemented 
	 * @param InActor : Actor that we will get the QuestSystemComponent from
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest System Helpers")
	static class UQuestSystemComponent* GetQuestSystemComponent(const AActor* InActor);
};
