// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define RPG_PRINTSCREEN(Message) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString(__FUNCTION__) + "(" + FString::FromInt(__LINE__) + ")" + Message);

class FRPGGameModule: public IModuleInterface
{
	public:

	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

	virtual bool IsGameModule() const override
	{
		return true;
	}

	virtual void RegisterNativeGameplayTags();
};
