// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestState.h"
#include "QuestSystemInterface.h"
#include "UObject/NoExportTypes.h"
#include "ObjectiveState.generated.h"

class UQuestSystemComponent;
/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UObjectiveState : public UObject, public IQuestSystemInterface
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<UQuestSystemComponent> OwningQuestSystemComponent;

	UPROPERTY()
	TWeakObjectPtr<UQuestState> ParentQuestState;

	UPROPERTY()
	FObjective ObjectiveDefinition;

	UPROPERTY()
	TArray<UTaskState*> Tasks;

public:

	UObjectiveState(const FObjectInitializer& ObjectInitializer);

	void Initialize(UQuestSystemComponent* QuestSystemComponent, UQuestState* Parent, FObjective ObjectiveDef);

	UFUNCTION(BlueprintCallable)
	virtual UQuestSystemComponent* GetQuestSystemComponent() const override;

	UFUNCTION()
	UQuestState* GetOwningQuestState() const;

	UFUNCTION()
	TArray<UTaskState*>& GetTasks();

public:

	void NotifyTaskStatusChanged(UTaskState* TaskState, ETaskStatus OldStatus, ETaskStatus NewStatus);
	
};
