// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UWRPG_HUD.h"

void UUWRPG_HUD::UpdateMaxHealth(float InMaxHealth)
{
	HealthBar->SetProgressMaxValue(InMaxHealth);
}

void UUWRPG_HUD::UpdateCurrentHealth(float InCurrentHealth)
{
	HealthBar->SetProgressCurrentValue(InCurrentHealth);
}

void UUWRPG_HUD::UpdateMaxMana(float InMaxMana)
{
	ManaBar->SetProgressMaxValue(InMaxMana);
}

void UUWRPG_HUD::UpdateCurrentMana(float InCurrentMana)
{
	ManaBar->SetProgressCurrentValue(InCurrentMana);
}

void UUWRPG_HUD::InitializeHUD(URPG_AbilitySystemComponent* ASC)
{
	SetRPGAbilitySystemComponent(ASC);

	HealthBar->UpdateProgressUsingAttribute(ASC);
	ManaBar->UpdateProgressUsingAttribute(ASC);

	K2_InitializeHUD(ASC);
}
