// Fill out your copyright notice in the Description page of Project Settings.


#include "State/TaskState.h"
#include "QuestSystemComponent.h"
#include "State/ObjectiveState.h"

UTaskState::UTaskState(const FObjectInitializer& ObjectInitializer)
{
	TaskStatus = ETaskStatus::Inactive;
	CurrentValue = 0;
	TaskDefinition = nullptr;
}

void UTaskState::InitializeTaskState(const FTask* TaskDef)
{
	TaskDefinition = TaskDef;
	TaskStatus = ETaskStatus::Inactive;
}

const FTask& UTaskState::GetTaskDefinition() const
{
	return *TaskDefinition;
}

const ETaskStatus& UTaskState::GetTaskStatus() const
{
	return TaskStatus;
}

bool UTaskState::UpdateTaskProgress(int Delta)
{
	if(!IsTaskActive())
	{
		return false;
	}

	const int OldValue = CurrentValue;
	CurrentValue = FMath::Min(CurrentValue + Delta, TaskDefinition->TargetValue);

	if(CurrentValue == TaskDefinition->TargetValue)
	{
		ChangeTaskStatus(ETaskStatus::Completed);
	}

	if(OnTaskProgressUpdated.IsBound())
	{
		OnTaskProgressUpdated.Broadcast(this, OldValue, CurrentValue);
	}

	return true;
}

bool UTaskState::IsTaskActive() const
{
	return TaskStatus == ETaskStatus::Active;
}

bool UTaskState::IsTaskComplete() const
{
	return TaskStatus == ETaskStatus::Completed;
}

bool UTaskState::IsTaskFailed() const
{
	return TaskStatus == ETaskStatus::Failed;
}

bool UTaskState::IsMandatoryTask() const
{
	return TaskDefinition->bIsMandatoryTask;
}

bool UTaskState::CompleteTask()
{
	if(!IsTaskActive())
	{
		return false;
	}

	ChangeTaskStatus(ETaskStatus::Completed);
	return true;
}

bool UTaskState::FailTask()
{
	if(!IsTaskActive())
	{
		return false;
	}

	ChangeTaskStatus(ETaskStatus::Failed);
	return true;
}

void UTaskState::ChangeTaskStatus(ETaskStatus NewStatus)
{
	if(NewStatus == TaskStatus)
	{
		return;
	}

	const ETaskStatus Temp = TaskStatus;
	TaskStatus = NewStatus;

	if(OnTaskStatusChanged.IsBound())
	{
		OnTaskStatusChanged.Broadcast(this, Temp, TaskStatus);
	}
}
