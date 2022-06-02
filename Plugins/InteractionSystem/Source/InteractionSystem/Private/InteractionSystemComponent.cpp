// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSystemComponent.h"

#include "DrawDebugHelpers.h"
#include "InteractableInterface.h"
#include "InteractionSystemInterface.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UInteractionSystemComponent::UInteractionSystemComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Defaults
	bCanModifyInteractingActorOnTick = false;
	bInDebugMode = false;
	InteractionDistance = 1000.f;
}


void UInteractionSystemComponent::SetCurrentInteractingActor(AActor* NewInteractingActor)
{
	// If our new interacting actor is our current interacting actor, return
	if(CurrentInteractingActor == NewInteractingActor)
	{
		return;
	}

	AActor* OldActor = CurrentInteractingActor;
	CurrentInteractingActor = NewInteractingActor;

	if(CurrentInteractingActorChanged.IsBound())
	{
		CurrentInteractingActorChanged.Broadcast(CurrentInteractingActor, OldActor);
	}
}

void UInteractionSystemComponent::PerformInteractionTrace(const IInteractionSystemInterface* InteractionInterface, FHitResult& HitResult) const
{
	// Setup our Trace
	const FInteractionTracePoints TracePoints =  InteractionInterface->GetTracePoints();
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetOwner());
	const FVector EndLocation = TracePoints.StartLocation + (TracePoints.ForwardVector * InteractionDistance);
	EDrawDebugTrace::Type DebugType;
	bInDebugMode ? DebugType = EDrawDebugTrace::ForOneFrame : DebugType = EDrawDebugTrace::None;

	UKismetSystemLibrary::LineTraceSingle(this,
		TracePoints.StartLocation,
		EndLocation,
		ETraceTypeQuery::TraceTypeQuery2,
		true,
		IgnoreActors,
		DebugType,
		HitResult,
		true,
		DebugLineTraceColor,
		DebugLineTraceHitColor);
}

bool UInteractionSystemComponent::UpdateInteractingActor()
{
	// Get our Starting and Ending Trace Locations
	IInteractionSystemInterface* InteractionInterface = Cast<IInteractionSystemInterface>(GetOwner());
	if(!InteractionInterface)
	{
		return false;
	}

	// Perform our trace and get the actor from hit result
	FHitResult HitResult;
	PerformInteractionTrace(InteractionInterface, HitResult);
	AActor* HitActor = HitResult.GetActor();

	// If we have a valid blocking hit and that actor implements our interaction system interface
	if(HitResult.IsValidBlockingHit() && HitActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		SetCurrentInteractingActor(HitActor);
		if(bInDebugMode)
		{
			UKismetSystemLibrary::DrawDebugCapsule(this, HitActor->GetActorLocation(), HitActor->GetSimpleCollisionHalfHeight(), HitActor->GetSimpleCollisionRadius(), FRotator::ZeroRotator, FLinearColor::White);
		}
		return true;
	}

	SetCurrentInteractingActor(nullptr);
	return false;
}

AActor* UInteractionSystemComponent::GetInteractingActor() const
{
	return CurrentInteractingActor;
}

bool UInteractionSystemComponent::TryInteract(AActor* OptionalActor)
{
	AActor* InteractingActor = OptionalActor ? OptionalActor : CurrentInteractingActor;

	// Verify our actor is valid
	if(!InteractingActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interacting Actor is NULL"));
		return false;
	}

	// Verify the actor implements the interactable interface
	if(!InteractingActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor %s does not implement interact interface!"), *InteractingActor->GetName())
		return false;
	}
	
	// Return whether our interaction began successfully and start our interaction
	const FInteractInfo InteractInfo = FInteractInfo::CreateInteractInfo(this);
	if (bool bInteractStarted = IInteractableInterface::Execute_PreInteract(InteractingActor, InteractInfo))
	{
		IInteractableInterface::Execute_Interact(InteractingActor, InteractInfo);
		return true;
	}

	return false;
}

// Called every frame
void UInteractionSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// This is where we will do our tracing and check for actors to interact with
	if(bCanModifyInteractingActorOnTick)
	{
		UpdateInteractingActor();
	}
}

