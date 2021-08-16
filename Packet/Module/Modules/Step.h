#pragma once
#include "Module.h"
class Step : public IModule {
public:
	bool reverse = false;
	float height = 1.5f;

	Step();
	~Step();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
