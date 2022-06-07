// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UWRPG_ProgressBar.h"
#include "GAS/RPG_AttributeSet.h"

UUWRPG_ProgressBar::UUWRPG_ProgressBar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CurrentValue = 0.00f;
	MaxValue = 0.00f;
}

void UUWRPG_ProgressBar::UpdateProgress() const
{
	FFormatOrderedArguments Args;
	Args.Add(CurrentValue);
	Args.Add(MaxValue);

	const FText Text = FText::Format(FText::FromString(TEXT("{0} / {1}")), Args);
	ProgressText->SetText(Text);
	
	ProgressBar->SetPercent(GetProgressBarPercentage());
}

float UUWRPG_ProgressBar::GetProgressBarPercentage() const
{
	if(MaxValue == 0.00f)
	{
		return 0.00f;
	}

	return CurrentValue / MaxValue;
}

void UUWRPG_ProgressBar::UpdateProgressUsingAttribute(const URPG_AbilitySystemComponent* ASC)
{
	const float Current = ASC->GetNumericAttribute(GameplayAttribute);
	const float Max = ASC->GetNumericAttribute(GameplayAttributeMax);

	CurrentValue = Current;
	MaxValue = Max;
	UpdateProgress();
}

void UUWRPG_ProgressBar::SetProgressCurrentValue(float& InCurrentValue)
{
	CurrentValue = InCurrentValue;

	UpdateProgress();
}

void UUWRPG_ProgressBar::SetProgressMaxValue(float& InMaxValue)
{
	MaxValue = InMaxValue;
	UpdateProgress();
}
