#pragma once
#include "..\ModuleManager.h"
#include "Module.h"
class Step : public IModule {
public:
	bool reverse = false;
	float height = 1.5f;
	int time = 0;

	Step();
	~Step();
	std::string name = "Step";

	// Inherited via IModule
	virtual const char* getRawModuleName() override;
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
