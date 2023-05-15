// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FXTypes.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_PlayEffectPack.generated.h"

/**
 * 
 */
UCLASS()
class FXMANAGER_API UANS_PlayEffectPack : public UAnimNotifyState
{
	GENERATED_BODY()

	UPROPERTY()
	FActiveEffectPackHandle Handle;
	
public:
	
	UPROPERTY(EditAnywhere)
	FEffectPack EffectPack;

	virtual FString GetNotifyName_Implementation() const override;
	
protected:
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
