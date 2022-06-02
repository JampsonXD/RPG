// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define RPG_PRINTSCREEN(Message) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString(__FUNCTION__) + "(" + FString::FromInt(__LINE__) + ")" + Message);
