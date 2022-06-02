// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestTypes.generated.h"

/**
 * 
 */

class ATaskTrigger;
USTRUCT(BlueprintType)
struct FQuestTask
{
	GENERATED_BODY()

	FQuestTask()
	{
		TaskName = FName("Default");
		TaskDescription = FText::GetEmpty();
		Trigger = nullptr;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TaskName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText TaskDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATaskTrigger* Trigger;
	
};

UENUM()
enum class QuestStatus : uint8
{
	Locked,
	InProgress,
	Completed
};

UENUM()
enum class TaskStatus : uint8
{
	Blocked,
	InProgress,
	Completed
};