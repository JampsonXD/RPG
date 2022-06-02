// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UQuestSystemComponent::UQuestSystemComponent(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Can't tick
	PrimaryComponentTick.bCanEverTick = false;
}

TArray<FQuest> UQuestSystemComponent::GetQuests() const
{
	return Quests;
}

FQuest UQuestSystemComponent::GetActiveQuest() const
{
	return ActiveQuest;
}

void UQuestSystemComponent::InitQuestComponent()
{
	
}

bool UQuestSystemComponent::AddQuest(FQuest NewQuest, bool bSetAsActiveQuest)
{
	if(ContainsQuest(NewQuest))
	{
		return false;
	}

	// Add the Quest to our Quests Array
	Quests.Add(NewQuest);

	// Delegate Call
	if(NewQuestAddedDelegate.IsBound())
	{
		NewQuestAddedDelegate.Broadcast(NewQuest);
	}

	// SetAsActiveQuest is true or we just added our only Quest
	if(bSetAsActiveQuest || Quests.Num() == 1)
	{
		SetActiveQuest(NewQuest);
	}

	return true;
}

bool UQuestSystemComponent::SwapActiveQuest(FQuest& NewActiveQuest)
{
	// Current Quest is already New Active Quest
	if(NewActiveQuest == ActiveQuest)
	{
		return false;
	}

	SetActiveQuest(NewActiveQuest);
	return true;
	
}

void UQuestSystemComponent::QuestTaskFinished(FQuest& InQuest)
{
	UKismetSystemLibrary::PrintString(this, "Task Finished", true, true, FLinearColor::Green, 3.f);
}

FOnNewActiveQuest UQuestSystemComponent::GetOnNewActiveQuestDelegate() const
{
	return NewActiveQuestDelegate;
}

void UQuestSystemComponent::SetActiveQuest(const FQuest& NewQuest)
{
	const FQuest OldQuest = ActiveQuest;
	ActiveQuest = NewQuest;
	if(NewActiveQuestDelegate.IsBound())
	{
		NewActiveQuestDelegate.Broadcast(ActiveQuest, OldQuest);
	}
}

// Quests use Gameplay tags as their Identifiers, check if this quest is already owned by checking if tags match
bool UQuestSystemComponent::ContainsQuest(const FQuest& InQuest)
{
	for(FQuest& Quest : Quests)
	{
		if(Quest == InQuest)
		{
			return true;
		}
	}

	return false;
}

void UQuestSystemComponent::SetupDefaultQuests()
{
	for(const FQuest& Quest : DefaultQuests)
	{
		AddQuest(Quest, false);
	}
}