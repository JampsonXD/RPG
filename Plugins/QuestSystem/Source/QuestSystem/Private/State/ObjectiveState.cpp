// Fill out your copyright notice in the Description page of Project Settings.

#include "State/ObjectiveState.h"
#include "QuestSystemComponent.h"
#include "State/TaskState.h"

UObjectiveState::UObjectiveState(const FObjectInitializer& ObjectInitializer)
{

}

void UObjectiveState::Initialize(UQuestSystemComponent* QuestSystemComponent, UQuestState* Parent, FObjective ObjectiveDef)
{
	OwningQuestSystemComponent = QuestSystemComponent;
	ParentQuestState = Parent;
	ObjectiveDefinition = ObjectiveDef;

	for(const FTask& TaskDef : ObjectiveDefinition.Tasks)
	{
		UTaskState* TaskState = NewObject<UTaskState>(GetOuter(), UTaskState::StaticClass());
		TaskState->InitializeTaskState(&TaskDef);
		TaskState->OnTaskStatusChanged.AddUniqueDynamic(this, &UObjectiveState::NotifyTaskStatusChanged);
		Tasks.Add(TaskState);
	}
}

UQuestSystemComponent* UObjectiveState::GetQuestSystemComponent() const
{
	if(OwningQuestSystemComponent.IsValid())
	{
		return OwningQuestSystemComponent.Get();
	}

	return nullptr;
}

UQuestState* UObjectiveState::GetOwningQuestState() const
{
	if(ParentQuestState.IsValid())
	{
		return ParentQuestState.Get();
	}

	return nullptr;
}

TArray<UTaskState*>& UObjectiveState::GetTasks()
{
	return Tasks;
}

void UObjectiveState::NotifyTaskStatusChanged(UTaskState* TaskState, ETaskStatus OldStatus, ETaskStatus NewStatus)
{

}
