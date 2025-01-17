// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemInterface.h"
#include "InventorySystemInterface.h"
#include "RPG_Types.h"
#include "GameFramework/PlayerController.h"
#include "UI/UWRPG_HUD.h"
#include "UI/UWRPG_Inventory.h"
#include "UI/WCRPG_DamagePopup.h"
#include "RPG_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPG_PlayerController : public APlayerController, public IInventorySystemInterface, public IQuestSystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest System Component", meta = (AllowPrivateAccess = "true"))
	class UQuestSystemComponent* QuestSystemComponent;
	
protected:

	UPROPERTY(BlueprintReadOnly)
	TSoftObjectPtr<UInventorySystemComponent> InventorySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Player Controller | HUD")
	TSubclassOf<UUWRPG_HUD> HUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Player Controller | UI | Inventory")
	TSubclassOf<UUWRPG_Inventory> InventoryWidgetClass;

	// HUD Widget
	UPROPERTY(BlueprintReadOnly, Category = "RPG Player Controller | UI")
	UUWRPG_HUD* RPGHUD;

	// Inventory Widget
	UPROPERTY(BlueprintReadOnly, Category = "RPG Player Controller | UI")
	UUWRPG_Inventory* InventoryWidget;

	UPROPERTY()
	bool bInventoryOpen;
	
	// Damage Popup Variables used to show on screen
	TQueue<FDamagePopupData> DamagePopups;
	FTimerHandle DamagePopupTimerHandle;

	// Interval for showing our pending popups
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage Popup")
	float DamagePopupInterval;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage Popup")
	TSubclassOf<UWCRPG_DamagePopup> DamagePopupWidgetComponentClass;

	virtual void OnPossess(APawn* InPawn) override;
	
	UFUNCTION()
	void ShowDamagePopup();
	
	void SetInventoryOpen(bool bIsOpen) { bInventoryOpen = bIsOpen; }

	void SetInventoryWidget(UUWRPG_Inventory* InWidget) { InventoryWidget = InWidget; }

	void SetRPGHUD(UUWRPG_HUD* InWidget) { RPGHUD = InWidget; }

	virtual void BeginPlay() override;

	UFUNCTION()
	void CustomBeginPlay();

	UFUNCTION()
	void WaitAbilitySystemComponentAndPlayerState();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Custom Begin Play")
	void K2_CustomBeginPlay();

	UFUNCTION()
	void SetupRPGHUD(URPG_AbilitySystemComponent* ASC);

	UFUNCTION()
	void SetupInventoryWidget();

public:

	ARPG_PlayerController(const FObjectInitializer& ObjectInitializer);

	virtual UInventorySystemComponent* GetInventorySystemComponent() const override;

	virtual UQuestSystemComponent* GetQuestSystemComponent() const override;

	void AddPendingDamagePopup(FDamagePopupData PopupData);

	UFUNCTION(BlueprintPure, Category = "RPG Player Controller | HUD")
	UUWRPG_HUD* GetRPGHUD() const;

	UFUNCTION(BlueprintPure, Category = "RPG Player Controller | UI | Inventory")
	UUWRPG_Inventory* GetInventoryWidget() const;

	UFUNCTION(BlueprintPure, Category = "RPG Player Controller | UI | Inventory")
	bool IsInventoryOpen() const;

	UFUNCTION(BlueprintCallable, Category = "RPG Player Controller | UI")
	void ToggleInventory();
};
