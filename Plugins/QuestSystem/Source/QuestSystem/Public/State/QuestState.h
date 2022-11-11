// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quest.h"
#include "QuestSystemInterface.h"
#include "UObject/NoExportTypes.h"
#include "QuestState.generated.h"

class UTaskState;
class UObjectiveState;
class UQuestSystemComponent;
/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestState : public UObject, public IQuestSystemInterface
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TWeakObjectPtr<UQuestSystemComponent> OwningQuestSystemComponent;

	const FQuest* QuestDefinition;

	UPROPERTY()
	int ObjectiveIndex;

	UPROPERTY()
	TArray<UObjectiveState*> Objectives;

	UPROPERTY()
	TMap<FName, UTaskState*> FastTaskLookupMap;

public:

	UQuestState(const FObjectInitializer& ObjectInitializer);

	void InitializeQuestState(UQuestSystemComponent* QuestSystemComponent, const FQuest* QuestDef);

	/* Tasks */

	UFUNCTION()
	void UpdateTaskProgress(const FName& TaskId, const int& Delta);

	UFUNCTION()
	void FinishTask(const FName& TaskId);

	UFUNCTION()
	bool IsTaskActive(const FName& TaskId);

	UFUNCTION()
	UTaskState* FindTaskState(const FName& TaskId);

	UFUNCTION(BlueprintCallable)
	virtual UQuestSystemComponent* GetQuestSystemComponent() const override;

	UFUNCTION()
	FQuest GetQuestDefinition() const;

private:
	UFUNCTION()
	void OnTaskCompleted(const FName& TaskId);

	/* End Tasks */

	/* Objectives */
	
};
