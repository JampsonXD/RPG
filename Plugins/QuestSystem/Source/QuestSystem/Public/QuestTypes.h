// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestTypes.generated.h"

/**
 * 
 */

UENUM()
enum class EQuestStatus : uint8
{
	Inactive,
	Active,
	Completed,
	Failed
};

UENUM()
enum class EObjectiveStatus : uint8
{
	Inactive,
	Active,
	Completed,
	Failed
};

UENUM()
enum class ETaskStatus : uint8
{
	Inactive,
	Active,
	Completed,
	Failed
};

class UTaskState;
class UObjectiveState;
class UQuestState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnQuestStatusChanged, UQuestState*, QuestState, EQuestStatus, OldStatus, EQuestStatus, NewStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnObjectiveStatusChanged, UObjectiveState*, ObjectiveState, EObjectiveStatus, OldStatus, EObjectiveStatus, NewStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTaskStatusChanged, UTaskState*, TaskState, ETaskStatus, OldStatus, ETaskStatus, NewStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTaskProgressUpdated, UTaskState*, TaskState, int, OldValue, int, NewValue);
