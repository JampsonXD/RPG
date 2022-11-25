// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LifetimeHandlerComponent.h"


// Sets default values for this component's properties
ULifetimeHandlerComponent::ULifetimeHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	bHasActiveLifetimeChecker = false;
	bShouldStartOnComponentInitialized = false;
}


// Called when the game starts
void ULifetimeHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	if(bShouldStartOnComponentInitialized && LifetimeCheckClass)
	{
		StartLifetime();
	}
	
}

void ULifetimeHandlerComponent::OnLifetimeCompleted()
{
	StopLifetime();
	LifetimeChecker = nullptr;
	GetOwner()->LifeSpanExpired();
}

void ULifetimeHandlerComponent::StartLifetime()
{
	if(!LifetimeChecker)
	{
		if(!LifetimeCheckClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("Lifetime Handler Component has null Lifetime Checker Class and cannot start!"));
			return;
		}
		
		LifetimeChecker = NewObject<ULifetimeChecker>(this, LifetimeCheckClass);
	}

	LifetimeChecker->StartChecker(GetOwner());
	LifetimeChecker->OnLifetimeCompleted.BindUObject(this, &ULifetimeHandlerComponent::OnLifetimeCompleted);

	bHasActiveLifetimeChecker = true;
}

void ULifetimeHandlerComponent::StopLifetime()
{
	bHasActiveLifetimeChecker = false;
	
	if(!LifetimeChecker) return;
	LifetimeChecker->StopChecker();
	LifetimeChecker->OnLifetimeCompleted.Unbind();
}


// Called every frame
void ULifetimeHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!bHasActiveLifetimeChecker)
	{
		return;
	}

	/* Tick our checker class if it requires ticking and is valid */
	if(LifetimeChecker && LifetimeChecker->IsTickable())
	{
		LifetimeChecker->Tick(DeltaTime);
		LifetimeChecker->K2_Tick(DeltaTime);
	}
}

void ULifetimeHandlerComponent::OnPoolEntered_Implementation()
{
	StopLifetime();
}

void ULifetimeHandlerComponent::OnPoolLeft_Implementation(const FActorPopData& PopData)
{
	StartLifetime();
}

