// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quest.h"
#include "TaskState.generated.h"

class UObjectiveState;
class UQuestSystemComponent;

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UTaskState : public UObject
{
	GENERATED_BODY()

private:

	const FTask* TaskDefinition;

	UPROPERTY()
	ETaskStatus TaskStatus;

	UPROPERTY()
	int CurrentValue;

public:

	UPROPERTY()
	FOnTaskStatusChanged OnTaskStatusChanged;

	UPROPERTY()
	FOnTaskProgressUpdated OnTaskProgressUpdated;

public:

	UTaskState(const FObjectInitializer& ObjectInitializer);

	void InitializeTaskState(const FTask* TaskDef);

	const FTask& GetTaskDefinition() const;

	UFUNCTION()
	const ETaskStatus& GetTaskStatus() const;

	/* Updates our current value towards our tasks target
	 * Returns true if our current value meets our target after adding the delta value
	 */
	UFUNCTION()
	bool UpdateTaskProgress(int Delta);

	UFUNCTION()
	bool IsTaskActive() const;

	UFUNCTION()
	bool IsTaskComplete() const;

	UFUNCTION()
	bool IsTaskFailed() const;

	UFUNCTION()
	bool IsMandatoryTask() const;

	UFUNCTION()
	bool CompleteTask();

	UFUNCTION()
	bool FailTask();

	UFUNCTION()
	void ChangeTaskStatus(ETaskStatus NewStatus);

};
