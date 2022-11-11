// Fill out your copyright notice in the Description page of Project Settings.

#include "State/QuestState.h"
#include "QuestSystemComponent.h"
#include "State/ObjectiveState.h"
#include "State/TaskState.h"

UQuestState::UQuestState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UQuestState::InitializeQuestState(UQuestSystemComponent* QuestSystemComponent, const FQuest* QuestDef)
{
	OwningQuestSystemComponent = QuestSystemComponent;
	QuestDefinition = QuestDef;

	for(const FObjective ObjectiveDef : QuestDef->Objectives)
	{
		UObjectiveState* Objective = NewObject<UObjectiveState>(GetOuter(), UObjectiveState::StaticClass());
		Objective->Initialize(GetQuestSystemComponent(), this, ObjectiveDef);
		Objectives.Add(Objective);

		/* Add Tasks Id's and Task States to our Fast Task Lookup Map so we do not need to loop through each objective
		 * whenever we need to query a task
		 */
		for(UTaskState* Task: Objective->GetTasks())
		{
			FastTaskLookupMap.Add(Task->GetTaskDefinition().Id, Task);
		}
	}
}

void UQuestState::UpdateTaskProgress(const FName& TaskId, const int& Delta)
{

}

void UQuestState::FinishTask(const FName& TaskId)
{

}

bool UQuestState::IsTaskActive(const FName& TaskId)
{
	if(UTaskState** Task = FastTaskLookupMap.Find(TaskId))
	{
		return (*Task)->IsTaskActive();
	}

	return false;
}

UTaskState* UQuestState::FindTaskState(const FName& TaskId)
{
	if(UTaskState** TaskState = FastTaskLookupMap.Find(TaskId))
	{
		return *TaskState;
	}

	return nullptr;
}

UQuestSystemComponent* UQuestState::GetQuestSystemComponent() const
{
	if(OwningQuestSystemComponent.IsValid())
	{
		return OwningQuestSystemComponent.Get();
	}

	return nullptr;
}

FQuest UQuestState::GetQuestDefinition() const
{
	return *QuestDefinition;
}

void UQuestState::OnTaskCompleted(const FName& TaskId)
{

}