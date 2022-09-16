// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolTypes.h"

bool FActorPool::ShouldGrow() const
{
	return Pool.Num() < MinimumPoolSize;
}

bool FActorPool::ShouldShrink() const
{
	return Pool.Num() > MaximumPoolSize;
}

void FActorPool::Push(AActor* Actor)
{
	Pool.Push(Actor);
}

AActor* FActorPool::Pop()
{
	if(!Pool.IsEmpty())
	{
		return Pool.Pop();
	}

	return nullptr;
}

int FActorPool::Num() const
{
	return Pool.Num();
}
