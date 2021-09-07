#pragma once
#include "Module.h"
class ChestStealer : public IModule {
private:
	int delay = 0;
	int setDelay = 7;
	bool enhanced = true;

public:
	bool autoDisable = false;
	ChestStealer();
	~ChestStealer();

	// Inherited via IModule
	virtual void onTick(C_GameMode* gm) override;
	virtual void chestScreenController_tick(C_ChestScreenController* c);
	virtual const char* getModuleName() override;
};
