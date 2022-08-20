// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_PlayerController.h"

#include "AbilitySystemGlobals.h"
#include "BFL_Inventory.h"
#include "InventorySystemComponent.h"
#include "QuestSystemComponent.h"
#include "RPGBFL_MainFunctions.h"
#include "RPG_PlayerState.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Characters/RPG_PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/WCRPG_DamagePopup.h"

ARPG_PlayerController::ARPG_PlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	QuestSystemComponent = CreateDefaultSubobject<UQuestSystemComponent>(TEXT("Quest System Component"));
}

UInventorySystemComponent* ARPG_PlayerController::GetInventorySystemComponent() const
{
	if(InventorySystemComponent.IsValid())
	{
		return InventorySystemComponent.Get();
	}

	return UBFL_Inventory::GetInventorySystemComponent(PlayerState);
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

void ARPG_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	WaitAbilitySystemComponentAndPlayerState();
}

void ARPG_PlayerController::CustomBeginPlay()
{
	ARPG_PlayerState* PS = GetPlayerState<ARPG_PlayerState>();
	InventorySystemComponent = PS->GetInventorySystemComponent();

	URPG_AbilitySystemComponent* ASC = Cast<URPG_AbilitySystemComponent>(PS->GetAbilitySystemComponent());

	SetupRPGHUD(ASC);
	SetupInventoryWidget();
}

void ARPG_PlayerController::WaitAbilitySystemComponentAndPlayerState()
{
	ARPG_PlayerState* PS = GetPlayerState<ARPG_PlayerState>();
	URPG_AbilitySystemComponent* ASC = PlayerState ? Cast<URPG_AbilitySystemComponent>(PS->GetAbilitySystemComponent()) : nullptr;

	// If one of these is invalid, create a timer and try again until both of these are valid
	if(!PS || !ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Player State or Ability System Component has not validated yet!"), *GetName());
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &ARPG_PlayerController::WaitAbilitySystemComponentAndPlayerState);
		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.10f, false);
		return;
	}

	CustomBeginPlay();
	K2_CustomBeginPlay();
}

void ARPG_PlayerController::SetupRPGHUD(URPG_AbilitySystemComponent* ASC)
{
	if(GetRPGHUD())
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried setting up RPG HUD when it is already initialized!"));
		return;
	}

	if(!HUDClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried setting up invalid HUD Class for s%!"), *GetName());
		return;
	}

	if(!ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried setting up HUD with an invalid ASC for s%!"), *GetName());
		return;
	}

	// Create our HUD widget and add it to the player viewport
	UUWRPG_HUD* Widget = CreateWidget<UUWRPG_HUD>(this, HUDClass);
	SetRPGHUD(Widget);
	Widget->AddToViewport();
	Widget->InitializeHUD(ASC);
}

void ARPG_PlayerController::SetupInventoryWidget()
{
	if(GetInventoryWidget())
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried setting up RPG Inventory when it is already initialized!"));
		return;
	}

	if(!InventoryWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried setting up invalid Inventory Class for s%!"), *GetName());
		return;
	}

	// Create our Inventory widget, add it to the player viewport, and set its mode to hidden
	UUWRPG_Inventory* Widget = CreateWidget<UUWRPG_Inventory>(this, InventoryWidgetClass);
	SetInventoryWidget(Widget);
	Widget->AddToViewport();
	Widget->SetVisibility(ESlateVisibility::Hidden);
	Widget->SetupInventoryWidget(GetInventorySystemComponent());
}

UUWRPG_HUD* ARPG_PlayerController::GetRPGHUD() const
{
	return RPGHUD;
}

UUWRPG_Inventory* ARPG_PlayerController::GetInventoryWidget() const
{
	return InventoryWidget;
}

bool ARPG_PlayerController::IsInventoryOpen() const
{
	return bInventoryOpen;
}

void ARPG_PlayerController::ToggleInventory()
{
	UUserWidget* Inventory = GetInventoryWidget();
	if(!Inventory)
	{
		return;
	}
	
	// If our inventory is open, close our inventory and set UI mode to game
	if(IsInventoryOpen())
	{
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
		Inventory->SetVisibility(ESlateVisibility::Hidden);
		SetShowMouseCursor(false);
		SetInventoryOpen(false);
		return;
	}

	
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, Inventory, EMouseLockMode::LockAlways, false);
	Inventory->SetVisibility(ESlateVisibility::Visible);
	SetInventoryOpen(true);
	SetShowMouseCursor(true);
}

void ARPG_PlayerController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);
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
