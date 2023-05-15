// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FXTypes.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_PlayEffectPack.generated.h"

/**
 * 
 */
UCLASS()
class FXMANAGER_API UAN_PlayEffectPack : public UAnimNotify
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FEffectPack EffectPack;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;
};
