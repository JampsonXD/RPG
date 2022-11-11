// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quest.h"
#include "QuestSubsystem.h"
#include "Components/ActorComponent.h"
#include "QuestSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestStarted, FQuest, Quest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestCompleted, FQuest, Quest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestFailed, FQuest, Quest);

UCLASS(ClassGroup=(QuestSystem), EditInlineNew, meta=(BlueprintSpawnableComponent))
class QUESTSYSTEM_API UQuestSystemComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

private:

	/* Cached Weak Pointer to our Quest Subsystem */
	UPROPERTY()
	TWeakObjectPtr<UQuestSubsystem> QuestSubsystem;

	/* Map of Quest Ids to Quest States. Contains all active quests that are player currently has access to */
	UPROPERTY()
	TMap<FName, UQuestState*> ActiveQuestStates;

public:
	/* Delegates */

	UPROPERTY(BlueprintAssignable, Category = "Quest System Component | Delegates")
	FQuestStarted OnQuestStarted;

	UPROPERTY(BlueprintAssignable, Category = "Quest System Component | Delegates")
	FQuestCompleted OnQuestCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Quest System Component | Delegates")
	FQuestFailed OnQuestFailed;

	/* End Delegates */

public:

	virtual void InitializeComponent() override;

public:

	/* Starts a Quest and adds it to the set of active quests if it is not already active.
	 * 
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest System Component")
	bool StartQuest(const FName QuestId);

	/* Updates a Tasks Progress if it needs a specific amount of targets completed.
	 * Finishes a Task if the Task does not require a set amount of targets
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest System Component")
	void UpdateTaskProgress(const FName QuestId, const FName TaskId, int Delta);

	/* Finishes a Task even if the specific target goal is not completed */
	UFUNCTION(BlueprintCallable, Category = "Quest System Component")
	void FinishTask(const FName QuestId, const FName TaskId);

private:

	void AddActiveQuestState(UQuestState* QuestState, const FQuest* QuestDefinition);

	UFUNCTION()
	UQuestState* GetQuestState(FName QuestId);

	UFUNCTION()
	UTaskState* GetTaskState(FName QuestId, FName TaskId);

public:

	UFUNCTION(BlueprintPure)
	UQuestSubsystem* GetQuestSubsystem() const;

	UFUNCTION()
	void SetQuestSubsystem(UQuestSubsystem* Subsystem);
 
};
