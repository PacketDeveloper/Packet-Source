#pragma once
#include "Module.h"
class ChestStealer : public IModule {
private:
	bool enhanced = true;
	int Odelay = 0;
	int delay = 10;

public:
	bool autoDisable = false;
	ChestStealer();
	~ChestStealer();

	// Inherited via IModule
	virtual void chestScreenController_tick(C_ChestScreenController* c);
	virtual const char* getModuleName() override;
};
