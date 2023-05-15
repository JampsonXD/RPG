// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AN_PlayEffectPack.h"

#include "FXManagerSubsystem.h"

FString UAN_PlayEffectPack::GetNotifyName_Implementation() const
{
	return "Play Effect Pack";
}

void UAN_PlayEffectPack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UFXManagerSubsystem* FXManager = UFXManagerSubsystem::GetFXManager();
	AActor* Owner = MeshComp->GetOwner();
	if(FXManager && Owner)
	{
		FXManager->PlayEffectAttached(Owner, nullptr, MeshComp, EffectPack, EEffectActivationType::Instant);
	}
}
