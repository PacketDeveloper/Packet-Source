#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class FastPlace : public IModule {
private:
	int delay = 0;
	int Odelay = 0;
	bool weapons = false;
	bool breakBlocks = false;
	bool rightclick = true;
	bool hold = true;

public:
	FastPlace();
	~FastPlace();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};