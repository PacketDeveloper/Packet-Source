#pragma once

#include "Module.h"

class TestModule : public IModule {
private:
	float glideModEffective = 0;
	int counter = 1;
	int testCounter = 0;
	bool test = false;
	bool autoDisable = true;
	bool alertBox = false;
	bool blink = false;
	bool serverMode = false;
	bool banUMode = false;
	bool istpMode = false;
	bool istpModeTwo = false;
	bool dmgflyMode = false;
	bool dmgMode = false;
	float tpSpeed = 3288.50f;
	bool randomThing = false;
	int testTimer = 0;

public:
	TestModule();
	virtual const char* getModuleName();
	virtual void onDisable() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual bool isFlashMode() override;
	virtual void onMove(C_MoveInputHandler* input) override;
};