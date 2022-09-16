// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPoolWorldSubsystem.h"

#include "PooledActorInterface.h"


const TSoftObjectPtr<UDataTable> UActorPoolWorldSubsystem::DefaultActorPoolDataTable = 
	TSoftObjectPtr<UDataTable>(FSoftObjectPath(TEXT("DataTable'/ActorPoolingSystem/DT_DefaultPoolData.DT_DefaultPoolData'")));

bool UActorPoolWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UActorPoolWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UActorPoolWorldSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UActorPoolWorldSubsystem::SetupActorPoolDefaults()
{
	DefaultActorPoolData = DefaultActorPoolDataTable.LoadSynchronous();

	/* If we are pointing to a valid data table, create actor pools based on row information.
	 * Create Pool handles checking if classes are valid and handling pool size errors.
	 */
	if (DefaultActorPoolData)
	{
		TArray<FDefaultActorPoolData*> PoolData;
		DefaultActorPoolData->GetAllRows("", PoolData);

		for (int i = 0; i < PoolData.Num(); i++)
		{
			if (const FDefaultActorPoolData* Data = PoolData[i])
			{
				CreatePool(Data->ActorClass, Data->MinimumPoolSize, Data->MaximumPoolSize, Data->PoolSize);
			}
		}
	}
}

AActor* UActorPoolWorldSubsystem::RequestActorFromPool(TSubclassOf<AActor> ActorClass, const FActorPopData& PopData)
{
	if(!IsValidActorClass(ActorClass))
	{
		return nullptr;
	}

	if(AActor* Actor = PopActorOfType(ActorClass, PopData))
	{
		return Actor;
	}

	// We might not have had an actor available so force spawn and return one 
	if(PoolMap.Contains(ActorClass))
	{
		AActor* Actor = ForceSpawnActor(ActorClass);
		ToggleActorPoolSettings(Actor, false);
		IPooledActorInterface::Execute_OnPoolLeft(Actor, PopData);
		return Actor;
	}

	// Didn't contain a pool for the requested class, create a pool and return an actor of the pool is created
	CreatePool(ActorClass, DefaultMinimumPoolSize, DefaultPoolSize);
	return RequestActorFromPool(ActorClass, PopData);
}

TArray<AActor*> UActorPoolWorldSubsystem::RequestActorsFromPool(TSubclassOf<AActor> ActorClass,
	const FActorPopData& PopData)
{
	TArray<AActor*> NewArray;
	return NewArray;
}

bool UActorPoolWorldSubsystem::AddActorToPool(AActor* Actor)
{
	if(!Actor || !Actor->Implements<UPooledActorInterface>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor does not implement pooled actor interface and can not be added to a pool."))
		return false;
	}

	const bool bHasPool = PoolMap.Contains(Actor->GetClass());

	if(!bHasPool)
	{
		CreatePool(Actor->GetClass(), DefaultMinimumPoolSize, DefaultMaximumPoolSize, DefaultPoolSize);
	}

	if (FActorPool* Pool = PoolMap.Find(Actor->GetClass()))
	{
		Pool->Push(Actor);
		ToggleActorPoolSettings(Actor, true);
		IPooledActorInterface::Execute_OnPoolEntered(Actor);
		return true;
	}

	return false;
}

bool UActorPoolWorldSubsystem::CreatePool(TSubclassOf<AActor> ActorClass, int MinimumPoolSize, int MaximumPoolSize, int Amount)
{
	if(!IsValidActorClass(ActorClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor Class is invalid, could not create pool."))
		return false;
	}

	if(PoolMap.Find(ActorClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("Pool already exists for class, could not create pool."))
		return false;
	}

	// Verify our values for the pool are proper, don't allow people to create negative pool sizes or make the min and max values out of order
	MinimumPoolSize = FMath::Max(MinimumPoolSize, 1);
	Amount = FMath::Max(MinimumPoolSize, Amount);
	MaximumPoolSize = FMath::Max(Amount, MaximumPoolSize);

	FActorPool* ActorPool = new FActorPool(MinimumPoolSize, MaximumPoolSize);
	FillPool(ActorClass, *ActorPool, Amount);
	PoolMap.Add(ActorClass, *ActorPool);
	return true;
}

bool UActorPoolWorldSubsystem::RemovePool(TSubclassOf<AActor> ActorClass)
{
	if (!IsValidActorClass(ActorClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor Class is invalid, could not remove pool of an invalid type."))
		return false;
	}

	// Make sure a pool exists for the class
	if(FActorPool* ActorPool = PoolMap.Find(ActorClass))
	{
		ReleaseFromPool(*ActorPool, ActorPool->Pool.Num());
		PoolMap.Remove(ActorClass);
		delete ActorPool;
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("Actor Pool does not exist, can not remove a pool that does not exist."))
	return false;
}

bool UActorPoolWorldSubsystem::ModifyPoolMinimumSize(TSubclassOf<AActor> ActorClass, const int NewMinimumPoolSize)
{
	return false;
}

AActor* UActorPoolWorldSubsystem::PopActorOfType(TSubclassOf<AActor> ActorClass, const FActorPopData& PopData)
{
	if (FActorPool* Pool = PoolMap.Find(ActorClass))
	{
		AActor* Actor = Pool->Pop();
		ToggleActorPoolSettings(Actor, false);
		IPooledActorInterface::Execute_OnPoolLeft(Actor, PopData);
		if (Pool->ShouldGrow())
		{
			FillPool(ActorClass, *Pool, Pool->MinimumPoolSize - Pool->Num());
		}
		return Actor;
	}

	return nullptr;
}

AActor* UActorPoolWorldSubsystem::ForceSpawnActor(TSubclassOf<AActor> ActorClass) const
{
	UWorld* World = GetWorld();

	if(!World || !ActorClass)
	{
		return nullptr;
	}

	AActor* NewActor = World->SpawnActor(ActorClass, &PoolingLocation);
	return NewActor;
}

bool UActorPoolWorldSubsystem::IsValidActorClass(const TSubclassOf<AActor>& ActorClass)
{
	return ActorClass && ActorClass->ImplementsInterface(UPooledActorInterface::StaticClass());
}

void UActorPoolWorldSubsystem::ToggleActorPoolSettings(AActor* Actor, const bool bOnEnteringPool) const
{
	Actor->SetActorTickEnabled(!bOnEnteringPool);
	Actor->SetReplicates(!bOnEnteringPool);
	Actor->SetActorHiddenInGame(bOnEnteringPool);
	Actor->SetActorEnableCollision(!bOnEnteringPool);

	if(bOnEnteringPool)
	{
		Actor->SetActorLocation(PoolingLocation);
	}
}

void UActorPoolWorldSubsystem::FillPool(UClass* Class, FActorPool& ActorPool, const int ActorSpawnAmount) const
{
	UWorld* World = GetWorld();
	const FVector* SpawnLocation = &PoolingLocation;
	FActorPopData PopData;
	for(int i = 0; i < ActorSpawnAmount; i++)
	{
		AActor* Actor = ForceSpawnActor(Class);
		ToggleActorPoolSettings(Actor, true);
		IPooledActorInterface::Execute_OnPoolEntered(Actor);
		ActorPool.Push(Actor);
	}
}

void UActorPoolWorldSubsystem::ReleaseFromPool(FActorPool& ActorPool, const int ActorRemoveAmount)
{
	if(ActorPool.Pool.IsEmpty())
	{
		return;
	}

	for (int i = 0; i < ActorRemoveAmount; i++)
	{
		if(AActor* Actor = ActorPool.Pop())
		{
			if(Actor)
			{
				Actor->Destroy();
			}

			if(ActorPool.Num() <= 0)
			{
				break;
			}
		}
	}
}
