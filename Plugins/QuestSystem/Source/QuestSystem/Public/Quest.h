// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "QuestTypes.h"
#include "Engine/DataTable.h"
#include "Quest.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FTask
{
	GENERATED_BODY()

	FTask()
	{
		bIsMandatoryTask = true;
		bIsSequential = false;
		TargetValue = 1;
	}

	/* Mandatory Identification for a task of a quest.
	 * These are required to be unique to the quest.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Task Definition")
	FName Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Task Definition")
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Task Definition")
	FText Description;

	// Whether the task needs to be completed for the objective to be completed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Task Definition")
	bool bIsMandatoryTask;

	/* Whether the task is required to be completed in sequential order.
	 * If false, the task can be completed as long as the objective is active */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Task Definition")
	bool bIsSequential;

	/* The amount of times the target of the task needs to be completed before the task is completed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Task Definition")
	int TargetValue;
};


USTRUCT(BlueprintType)
struct FObjective
{
	GENERATED_BODY()

	FObjective()
	{
		
	}

	/* Optional Identification for an objective of a quest. These should be unique to the
	 * Quest if used. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Objective Definition")
	FName Id;

	// Information on the main purpose of the objective
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Objective Definition")
	FText Title;

	// Description of the goal that is trying to be accomplished with the task
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Objective Definition")
	FText Description;

	/* List of Tasks that are related to this objective. These tasks do not need to be in sequential order but are by default.
	 * Not all tasks need to be finished for the objective to be finished successfully
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Objective Definition")
	TArray<FTask> Tasks;

};

USTRUCT(BlueprintType)
struct FQuest : public FTableRowBase
{
	GENERATED_BODY()

	FQuest()
	{

	}
	
	/* Identification given to a quest, needed for querying quests
	 * Quest Id's must all be unique, an error will be thrown at runtime if this is not the case */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Definition")
	FName Id;

	// Title of the quest given to the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Definition")
	FText Title;

	// Description of the quest given to the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Definition")
	FText Description;
	
	// Sequential set of objectives the task has
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Definition")
	TArray<FObjective> Objectives;

};
