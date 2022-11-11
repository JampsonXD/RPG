// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemComponent.h"
#include "State/QuestState.h"
#include "State/TaskState.h"

UQuestSystemComponent::UQuestSystemComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UQuestSystemComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// Cache a weak pointer to our Quest Subsystem
	SetQuestSubsystem(UQuestSubsystem::GetQuestSubsystem(this));
}

bool UQuestSystemComponent::StartQuest(const FName QuestId)
{
	// Only start a quest if it is not already started
	if(ActiveQuestStates.Contains(QuestId))
	{
		return false;
	}

	const FQuest* QuestDef = nullptr;
	// Make sure the QuestId we passed in has a quest definition
	if (!QuestSubsystem->TryGetQuestDefinition(QuestId, QuestDef))
	{
		return false;
	}

	UQuestState* QuestState = NewObject<UQuestState>(this, UQuestState::StaticClass());
	AddActiveQuestState(QuestState, QuestDef);
	return true;
}

void UQuestSystemComponent::UpdateTaskProgress(const FName QuestId, const FName TaskId, int Delta)
{
	if(UTaskState* TaskState = GetTaskState(QuestId, TaskId))
	{
		TaskState->UpdateTaskProgress(Delta);
	}
}

void UQuestSystemComponent::FinishTask(const FName QuestId, const FName TaskId)
{
	if(UTaskState* TaskState = GetTaskState(QuestId, TaskId))
	{
		TaskState->CompleteTask();
	}
}

void UQuestSystemComponent::AddActiveQuestState(UQuestState* QuestState, const FQuest* QuestDefinition)
{
	if(!QuestState)
	{
		return;
	}

	if(ActiveQuestStates.Contains(QuestDefinition->Id))
	{
		return;
	}

	ActiveQuestStates.Add(QuestDefinition->Id, QuestState);
	QuestState->InitializeQuestState(this, QuestDefinition);
}

UQuestState* UQuestSystemComponent::GetQuestState(FName QuestId)
{
	if(UQuestState** QuestState = ActiveQuestStates.Find(QuestId))
	{
		return *QuestState;
	}

	return nullptr;
}

UTaskState* UQuestSystemComponent::GetTaskState(FName QuestId, FName TaskId)
{
	if(UQuestState* QS = GetQuestState(QuestId))
	{
		return QS->FindTaskState(TaskId);
	}

	return nullptr;
}

UQuestSubsystem* UQuestSystemComponent::GetQuestSubsystem() const
{
	return QuestSubsystem.Get();
}

void UQuestSystemComponent::SetQuestSubsystem(UQuestSubsystem* Subsystem)
{
	QuestSubsystem = Subsystem;
}
