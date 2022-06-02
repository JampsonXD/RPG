// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskTrigger.h"

// Sets default values
ATaskTrigger::ATaskTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(false);

	// Actor is hidden 
	SetActorHiddenInGame(true);
}

void ATaskTrigger::StartTask(UQuestSystemComponent* QSC, FQuest& Quest)
{
	// Enable our Actor when a Task has started
	SetActorHiddenInGame(false);
	PrimaryActorTick.SetTickFunctionEnable(true);
	
	OwningTaskQSC = QSC;
	OwningQuest = Quest;
}

void ATaskTrigger::FinishedTask(UQuestSystemComponent* QSC, FQuest& QuestFinished)
{
	QSC->QuestTaskFinished(QuestFinished);
}

