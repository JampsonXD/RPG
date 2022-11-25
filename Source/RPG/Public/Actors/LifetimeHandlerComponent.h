// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LifetimeChecker.h"
#include "PooledActorInterface.h"
#include "Components/ActorComponent.h"
#include "LifetimeHandlerComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RPG_API ULifetimeHandlerComponent : public UActorComponent, public IPooledActorInterface
{
	
	GENERATED_BODY()

private:

	UPROPERTY()
	ULifetimeChecker* LifetimeChecker;

public:
	// Sets default values for this component's properties
	ULifetimeHandlerComponent();

	/* Class that will be used to check if our actors lifetime is over */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULifetimeChecker> LifetimeCheckClass;

	/* Whether we should automatically start our lifetime once the component is initialized */
	UPROPERTY(EditDefaultsOnly)
	bool bShouldStartOnComponentInitialized;

	UPROPERTY()
	bool bHasActiveLifetimeChecker;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OnLifetimeCompleted();


public:

	UFUNCTION(BlueprintCallable)
	void StartLifetime();

	UFUNCTION(BlueprintCallable)
	void StopLifetime();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void OnPoolEntered_Implementation() override;
	
	virtual void OnPoolLeft_Implementation(const FActorPopData& PopData) override;
};
