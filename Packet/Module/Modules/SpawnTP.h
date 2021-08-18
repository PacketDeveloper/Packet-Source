#pragma once

#include "Module.h"

class SpawnTP : public IModule {
public:

	SpawnTP();

	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual const char* getModuleName();
	virtual void onTick(C_GameMode* gm) override;
};