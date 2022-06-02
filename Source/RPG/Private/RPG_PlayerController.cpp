// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_PlayerController.h"

#include "InventorySystemComponent.h"
#include "QuestSystemComponent.h"
#include "Characters/RPG_PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/WCRPG_DamagePopup.h"

ARPG_PlayerController::ARPG_PlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InventorySystemComponent = CreateDefaultSubobject<UInventorySystemComponent>(TEXT("Inventory System Component"));

	QuestSystemComponent = CreateDefaultSubobject<UQuestSystemComponent>(TEXT("Quest System Component"));
}

UInventorySystemComponent* ARPG_PlayerController::GetInventorySystemComponent() const
{
	return InventorySystemComponent;
}

UQuestSystemComponent* ARPG_PlayerController::GetQuestSystemComponent() const
{
	return QuestSystemComponent;
}

void ARPG_PlayerController::AddPendingDamagePopup(FDamagePopupData PopupData)
{
	// Add our Popup data to our queue
	DamagePopups.Enqueue(PopupData);

	// If our world exists and our timer is not already active
	UWorld* World = GetWorld();
	if(World && !World->GetTimerManager().IsTimerActive(DamagePopupTimerHandle))
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &ARPG_PlayerController::ShowDamagePopup);

		World->GetTimerManager().SetTimer(DamagePopupTimerHandle, TimerDelegate, DamagePopupInterval, false);
	}
}

void ARPG_PlayerController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);
	InventorySystemComponent->InitActorInfo(this, PossessedPawn);
}

void ARPG_PlayerController::ShowDamagePopup()
{
	FDamagePopupData Data;
	DamagePopups.Dequeue(Data);

	ARPG_PlayerCharacter* PlayerCharacter = Cast<ARPG_PlayerCharacter>(GetCharacter());
	if(DamagePopupWidgetComponentClass && PlayerCharacter)
	{
		UWCRPG_DamagePopup* DamagePopup = NewObject<UWCRPG_DamagePopup>(PlayerCharacter, DamagePopupWidgetComponentClass);
		DamagePopup->RegisterComponent();
		DamagePopup->SetWorldLocation(Data.DamagePopupLocation);
		DamagePopup->SetupDamagePopup(Data);
	}
	
	// Check if our Queue isn't empty and if so set another timer to show
	if(!DamagePopups.IsEmpty() && GetWorld())
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &ARPG_PlayerController::ShowDamagePopup);

		GetWorld()->GetTimerManager().SetTimer(DamagePopupTimerHandle, TimerDelegate, DamagePopupInterval, false);
	}
}
