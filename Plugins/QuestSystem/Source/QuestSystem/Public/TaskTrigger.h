// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestSystemComponent.h"
#include "TaskTrigger.generated.h"

UCLASS()
class QUESTSYSTEM_API ATaskTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATaskTrigger();

	/** Called when we step into this task from a quest, does not have to be the active quest
	 *@param QSC : QuestSystemComponent that is asking to start this task, will cache reference
	 *@param Quest : Quest that has called this task; Might use later for Watching and updating
	 *more than one quest that is using the task at the given time
	 */
	void StartTask(UQuestSystemComponent* QSC, FQuest& Quest);

	void FinishedTask(UQuestSystemComponent* QSC, FQuest& QuestFinished);
	
protected:
	
	// Owning Quest System Component for this Task. Once the task is completed, this is who we will update to let them know they can move to the next Task
	UPROPERTY()
	UQuestSystemComponent* OwningTaskQSC;

	UPROPERTY()
	FQuest OwningQuest;
};
