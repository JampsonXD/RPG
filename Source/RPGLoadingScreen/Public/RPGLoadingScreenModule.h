#pragma once

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class IRPGLoadingScreenModule: public IModuleInterface
{
public:
	static inline IRPGLoadingScreenModule& Get()
	{
		return FModuleManager::GetModuleChecked<IRPGLoadingScreenModule>("RPGLoadingScreen");
	}

	// Start loading screen
	virtual void StartInGameLoadingScreen(bool bPlayUntilStopped, float Playtime) = 0;
	
	// Stop loading screen
	virtual void StopInGameLoadingScreen() = 0;
	
};