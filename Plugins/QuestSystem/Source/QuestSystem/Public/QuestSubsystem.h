// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quest.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestSubsystem.generated.h"

class UQuestState;
/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UQuestSubsystem(const FObjectInitializer& ObjectInitializer);

	/* Native access to getting our Quest Subsystem Instance from our Game Instance.*/
	UFUNCTION()
	static UQuestSubsystem* GetQuestSubsystem(const UObject* ContextObject);

	// Begin Subsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End Subsystem

private:

	UPROPERTY()
	TMap<FName, FQuest> QuestDefinitions;

private:

	UFUNCTION()
	void InitializeQuestDefinitions();

public:

	/* Returns a copy of the quest definitions stored currently in the subsystem */
	UFUNCTION(BlueprintPure, Category = "Questing Subsystem")
	TArray<FQuest> GetQuestDefinitions() const;

	/* Tries to return a copy of the quest definition for a specific quest id */
	bool TryGetQuestDefinition(const FName QuestId, const FQuest*& Quest) const;

};
