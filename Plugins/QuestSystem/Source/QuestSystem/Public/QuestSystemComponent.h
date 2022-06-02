// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Quest.h"
#include "QuestSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNewActiveQuest, const FQuest, NewActiveQuest, const FQuest, OldActiveQuest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewQuestAdded, const FQuest, NewQuest);

UCLASS(ClassGroup=(QuestSystem), EditInlineNew, meta=(BlueprintSpawnableComponent))
class QUESTSYSTEM_API UQuestSystemComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest System Component")
	TArray<FQuest> GetQuests() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest System Component")
	FQuest GetActiveQuest() const;

	/** Used to initialize our Quest Component, Default Quests are granted here **/
	void InitQuestComponent();

	
	/** Tries to add a new Quest to the set of quests this component will look over
	* @param NewQuest : Quest to try to add to our component
	* @param bSetAsActiveQuest : Whether we should set our new Quest as the Active Quest
	* @return bool : Returns whether the Quest was added successfully or not
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest System Component")
	bool AddQuest(FQuest NewQuest, bool bSetAsActiveQuest = false);

	/** Swaps the Active Quest with a new Quest
	 * @param NewActiveQuest : Quest we are trying to swap
	 * @return  bool : Whether the swap was successful or not
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest System Component")
	bool SwapActiveQuest(FQuest& NewActiveQuest);

	UFUNCTION(BlueprintCallable, Category = "Quest System Component")
	void QuestTaskFinished(FQuest& InQuest);

	/** Quest Delegates Getters **/
	UFUNCTION()
	FOnNewActiveQuest GetOnNewActiveQuestDelegate() const;
	
protected:

	// Current list of quests this component owns and are not finished
	UPROPERTY()
	TArray<FQuest> Quests;

	// The Quest this component is actively tracking
	UPROPERTY()
	FQuest ActiveQuest;

	// Default Quests to give the actor
	UPROPERTY(EditAnywhere, Category = "Quest | Default")
	TArray<FQuest> DefaultQuests;

	/** Quest Delegates **/

	// A new Active Quest was set
	UPROPERTY(BlueprintAssignable)
	FOnNewActiveQuest NewActiveQuestDelegate;

	// A new Quest was added to our Quest Array
	UPROPERTY(BlueprintAssignable)
	FOnNewQuestAdded NewQuestAddedDelegate;

	/** Function that we will call whenever we want to change our Active Quest, calls delegates
	*@param NewQuest : Quest we are setting as our new Active Quest
	*/
	UFUNCTION()
	void SetActiveQuest(const FQuest& NewQuest);

	/** Checks to see if we already have this quest added to our Quests Array
	 *@param InQuest : Quest we will be checking with against our Quest Array
	 *@return bool : Returns true if we already have the quest
	 */
	UFUNCTION()
	bool ContainsQuest(const FQuest& InQuest);

	/** Adds our default Quests to our Component **/
	void SetupDefaultQuests();
 
};
