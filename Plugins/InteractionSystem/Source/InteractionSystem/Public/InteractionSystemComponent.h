// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionSystemInterface.h"
#include "Components/ActorComponent.h"
#include "InteractionSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentInteractingActorChanged, AActor*, NewActor, AActor*, OldActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INTERACTIONSYSTEM_API UInteractionSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionSystemComponent(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(BlueprintAssignable)
	FOnCurrentInteractingActorChanged CurrentInteractingActorChanged;
	
	// Whether our component should modify the current interacting actor on tick
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction System | Defaults")
	bool bCanModifyInteractingActorOnTick;

	// Current Actor we are Interacting with
	UPROPERTY(BlueprintReadOnly, Category = "Interaction System | Defaults")
	AActor* CurrentInteractingActor;

	// How far we will do our Interacting Actor Trace
	UPROPERTY(EditDefaultsOnly, Category = "Interaction System | Defaults")
	float InteractionDistance;

	/*************** Start of Debug Mode ************************/
	// Whether we are in Debug Mode for the Component
	UPROPERTY(EditDefaultsOnly, Category = "Interaction System | Debug")
	bool bInDebugMode;

	// Color our Line Trace should be if in debug mode
	UPROPERTY(EditDefaultsOnly, Category = "Interaction System | Debug")
	FColor DebugLineTraceColor;

	// Color our Line Trace Hit should be if in debug mode
	UPROPERTY(EditDefaultsOnly, Category = "Interaction System | Debug")
	FColor DebugLineTraceHitColor;

	/**************** End of Debug Mode ************************/

	// Function we should call when changing our CurrentInteractingActor, this will call the proper broadcast delegates and such when updating
	UFUNCTION(BlueprintCallable, Category = "Interaction System | Interacting Actor")
	void SetCurrentInteractingActor(AActor* NewInteractingActor);
	void PerformInteractionTrace(const IInteractionSystemInterface* InteractionInterface, FHitResult& HitResult) const;

	// Called on Tick, checks to see if we should set a new interacting actor and does so based on the line trace we are given back
	bool UpdateInteractingActor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Getter for our Current Interacting Actor
	UFUNCTION(BlueprintCallable, Category = "Interaction System | Interact")
	AActor* GetInteractingActor() const;

	// Try Interacting with an actor, will use the CurrentInteractingActor if no actor is passed in, returns true if the interaction succeeds
	UFUNCTION(BlueprintCallable, Category = "Interaction System | Interact")
	bool TryInteract(AActor* OptionalActor = nullptr);

		
};
