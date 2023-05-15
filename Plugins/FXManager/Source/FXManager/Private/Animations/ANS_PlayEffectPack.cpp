// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/ANS_PlayEffectPack.h"

#include "FXManagerSubsystem.h"

FString UANS_PlayEffectPack::GetNotifyName_Implementation() const
{
	return "Play Duration Effect Pack";
}

void UANS_PlayEffectPack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                      const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	UFXManagerSubsystem* FXManager = UFXManagerSubsystem::GetFXManager();
	AActor* Owner = MeshComp->GetOwner();
	if(FXManager && Owner)
	{
		Handle = FXManager->PlayEffectAttached(Owner, nullptr, MeshComp, EffectPack, EEffectActivationType::Active);
	}
}

void UANS_PlayEffectPack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if(UFXManagerSubsystem* FXManager = UFXManagerSubsystem::GetFXManager())
	{
		FXManager->StopActivePack(Handle);
	}
}
