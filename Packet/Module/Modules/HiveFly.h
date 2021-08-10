#pragma once
#include "Module.h"
class HiveFly : public IModule {
public:
	bool boost = true;
	bool strafeMode = false;
	bool Blink = false;
	bool dmgBoost = false;
	bool speedWasEnabled = false;
	bool scfWasEnabled = false;
	bool timerBoost = false;
	int counter = 1;
	int testTimer = 1;
	int counter2 = 1;
	float speedA = 0.30183640122413635;    // Speed
	float speed2 = -0.004993199370801449;  // Value
	HiveFly();
	~HiveFly();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onMove(C_MoveInputHandler* input) override;
	virtual void onDisable() override;
};