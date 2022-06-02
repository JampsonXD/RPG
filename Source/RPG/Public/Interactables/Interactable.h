// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class RPG_API AInteractable : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool PreInteract_Implementation(FInteractInfo InteractInfo) override;

	virtual void Interact_Implementation(FInteractInfo InteractInfo) override;

	virtual void PostInteract_Implementation(FInteractInfo InteractInfo, EInteractionEndState EndState) override;

};
