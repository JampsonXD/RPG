// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DeveloperSettings.h"
#include "QuestDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Quest System Settings"))
class QUESTSYSTEM_API UQuestDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UQuestDeveloperSettings(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Definitions")
	TArray<TSoftObjectPtr<UDataTable>> QuestDefinitionDataTables;
	
};
