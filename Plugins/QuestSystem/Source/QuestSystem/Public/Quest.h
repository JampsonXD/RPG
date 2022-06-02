// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "QuestTypes.h"
#include "UObject/Object.h"
#include "Quest.generated.h"

/**
 * 
 */

class UQuestDataAsset;
USTRUCT(BlueprintType)
struct FQuest
{
	GENERATED_BODY()

	FQuest()
	{
		CharacterGiven = TEXT("Default");
		QuestID = nullptr;
	}
	
	// Who gave us the Quest
	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	FName CharacterGiven;
	
	//  Primary Asset ID to load, also used for identification of the quest as well
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	UQuestDataAsset* QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	float CurrentTaskIndex;
	
	// Set of Tasks that we must complete to finish the Quest
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<FQuestTask> QuestTasks;

	/** Operator Overloading so we can do comparisons easier **/
	FORCEINLINE bool operator==(const FQuest& OtherQuest) const
	{
		return QuestID == OtherQuest.QuestID;
	}
};
