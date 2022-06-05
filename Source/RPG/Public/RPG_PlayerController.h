// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemInterface.h"
#include "InventorySystemInterface.h"
#include "RPG_Types.h"
#include "GameFramework/PlayerController.h"
#include "UI/UWRPG_HUD.h"
#include "UI/WCRPG_DamagePopup.h"
#include "RPG_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPG_PlayerController : public APlayerController, public IInventorySystemInterface, public IQuestSystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory System Component", meta = (AllowPrivateAccess = "true"))
	class UInventorySystemComponent* InventorySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest System Component", meta = (AllowPrivateAccess = "true"))
	class UQuestSystemComponent* QuestSystemComponent;

	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Player Controller | HUD")
	TSubclassOf<UUWRPG_HUD> HUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Player Controller | UI | Inventory")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	// HUD Widget
	UPROPERTY(BlueprintReadOnly, Category = "RPG Player Controller | UI")
	UUWRPG_HUD* RPGHUD;

	// Inventory Widget
	UPROPERTY(BlueprintReadOnly, Category = "RPG Player Controller | UI")
	UUserWidget* InventoryWidget;

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

	void SetInventoryWidget(UUserWidget* InWidget) { InventoryWidget = InWidget; }

	void SetRPGHUD(UUWRPG_HUD* InWidget) { RPGHUD = InWidget; }

public:

	ARPG_PlayerController(const FObjectInitializer& ObjectInitializer);

	virtual UInventorySystemComponent* GetInventorySystemComponent() const override;

	virtual UQuestSystemComponent* GetQuestSystemComponent() const override;

	void AddPendingDamagePopup(FDamagePopupData PopupData);

	UFUNCTION(BlueprintCallable, Category = "RPG Player Controller | HUD")
	void SetupRPGHUD();

	UFUNCTION(BlueprintCallable, Category = "RPG Player Controller | UI | Inventory")
	void SetupInventoryWidget();

	UFUNCTION(BlueprintPure, Category = "RPG Player Controller | HUD")
	UUWRPG_HUD* GetRPGHUD() const;

	UFUNCTION(BlueprintPure, Category = "RPG Player Controller | UI | Inventory")
	UUserWidget* GetInventoryWidget() const;

	UFUNCTION(BlueprintPure, Category = "RPG Player Controller | UI | Inventory")
	bool IsInventoryOpen() const;

	UFUNCTION(BlueprintCallable, Category = "RPG Player Controller | UI")
	void ToggleInventory();
};
