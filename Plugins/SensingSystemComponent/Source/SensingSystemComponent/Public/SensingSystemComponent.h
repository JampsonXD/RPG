// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetType.h"
#include "Components/ActorComponent.h"
#include "SensingSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorSensed, AActor*, SensedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorLost, AActor*, LostActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENSINGSYSTEM_API USensingSystemComponent : public UActorComponent
{
	GENERATED_BODY()


private:

	/* Actors our Sensing Component is currently sensing */
	UPROPERTY()
	TArray<AActor*> SensingActors;

	/* Actor found in the world our component associated with
	 * Sensing components can be on things such as the player state
	 * or player controller, so this could be a reference to the actors pawn.
	 */
	UPROPERTY()
	AActor* AvatarActor;

public:

	/* Called every time a new actor is sensed */
	UPROPERTY(BlueprintAssignable)
	FOnActorSensed OnActorSensed;

	/* Called every time we lose an actor from being sensed */
	UPROPERTY(BlueprintAssignable)
	FOnActorLost OnActorLost;

	/* Determines how we gather possible targets for sensing */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced)
	TArray<UCollectionType*> GatherTargetCollections;

	/* Sensing Filter that we will run through with our gathered targets */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced)
	TArray<UFilterSensingData*> SensingFilters;

	/* The amount of pre-allocated slots we will have for sensing actors */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int SensingActorPreAllocatedSlots;

public:

	// Sets default values for this component's properties
	USensingSystemComponent();

	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	/* Called on tick to update our sensing actors */
	UFUNCTION()
	virtual void UpdateSensingActors();

	/** Filters gathered actors that are available for sensing
	 * @return : Returns whether our filtered actors array is not empty
	 *
	 */
	UFUNCTION()
	virtual TArray<AActor*> FilterSensingActors(const TArray<AActor*>& GatheredActors);

	UFUNCTION()
	virtual void AddSensingActor(AActor* SensingActor);

	UFUNCTION()
	virtual void RemoveSensingActor(AActor* LostActor);

public:

	/* Gets the avatar actor associated with our component */
	UFUNCTION(BlueprintPure)
	AActor* GetAvatarActor() const;

	/* Sets the avatar actor for our sensing component */
	UFUNCTION(BlueprintCallable)
	void InitializeSensingComponent(AActor* InAvatarActor);

	/* Gets a copy of all sensing actors */
	UFUNCTION(BlueprintPure)
	TArray<AActor*> GetSensingActors() const;


private:

	UFUNCTION()
	bool CanShrinkSensedActors() const;
};
