// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSubsystem.h"

#include "Core/QuestDeveloperSettings.h"

UQuestSubsystem::UQuestSubsystem(const FObjectInitializer& ObjectInitializer)
{

}

UQuestSubsystem* UQuestSubsystem::GetQuestSubsystem(const UObject* ContextObject)
{
	if (!ContextObject)
	{
		return nullptr;
	}

	const UWorld* World = ContextObject->GetWorld();
	if (!World)
	{
		return nullptr;
	}

	return World->GetGameInstance()->GetSubsystem<UQuestSubsystem>();
}

void UQuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Populate our Quest Definitions Map
	InitializeQuestDefinitions();
}

void UQuestSubsystem::Deinitialize()
{
	Super::Deinitialize();


}

void UQuestSubsystem::InitializeQuestDefinitions()
{
	QuestDefinitions.Empty();

	// Get default object of our developer settings
	const UQuestDeveloperSettings* DefaultObject = GetDefault<UQuestDeveloperSettings>();
	const TArray<TSoftObjectPtr<UDataTable>>& QuestDefinitionDataTables = DefaultObject->QuestDefinitionDataTables;

	if (QuestDefinitionDataTables.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Quest Definitions Data Table is empty, no definitions have been added to Questing Subsystem."))
		return;
	}

	/* Loop through our Quest Definitions. For each Data Table Soft Object Pointer, make sure we point to a valid Data Table,
	 * Load the Data Table, Get all rows of the Data Table as an array of FQuests, and add them to our Quest Definitions
	 */
	for(const TSoftObjectPtr<UDataTable>& DataTablePointer: QuestDefinitionDataTables)
	{
		// Check if our soft object pointer is pointing to a valid path or if it is already pointing to a valid object
		if(DataTablePointer.IsPending() || DataTablePointer.IsValid())
		{
			UDataTable* Table = DataTablePointer.LoadSynchronous();
			TArray<FQuest*> Quests;
			Table->GetAllRows("", Quests);

			for(const FQuest* Quest: Quests)
			{
				if(Quest)
				{
					/* If our Quest Definitions already contains an Id, this means we have overlapping Quests and this could cause issues. */
					ensure(!QuestDefinitions.Contains(Quest->Id));
					QuestDefinitions.Add(Quest->Id, *Quest);

					/* Loop through our Quests Tasks and make sure they all contain unique id's
					 * Uses a Set to keep track of Id's and ensures that none are already contained */
					TSet<FName> TaskIdSet;
					for(const FObjective& Objective: Quest->Objectives)
					{
						for(const FTask Task: Objective.Tasks)
						{
							ensure(!TaskIdSet.Contains(Task.Id));
							TaskIdSet.Add(Task.Id);
						}
					}
				}
			}
		}
	}
}

TArray<FQuest> UQuestSubsystem::GetQuestDefinitions() const
{
	TArray<FQuest> Definitions;
	QuestDefinitions.GenerateValueArray(Definitions);

	return Definitions;
}

bool UQuestSubsystem::TryGetQuestDefinition(const FName QuestId, const FQuest*& Quest) const
{
	if(const FQuest* FoundQuest = QuestDefinitions.Find(QuestId))
	{
		Quest = FoundQuest;
		return true;
	}

	return false;
}
