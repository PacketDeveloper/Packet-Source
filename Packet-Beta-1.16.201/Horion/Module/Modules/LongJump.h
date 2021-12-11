#pragma once

#include "Module.h"

class LongJump : public IModule {
private:
	float glideMod = -0.16f;
	int counter = 1;
	bool dmgMode = true;
	bool newMode = false;
	float tpDis = 0.f;
	bool damage = false;
	bool slowDown = true;
	bool floatMode = false;
	float height = 1.f;
	int testTimer = 0;
	float speed = 2.f;
	bool lowMode = true;
	bool disable;

public:
	LongJump();
	//virtual void onTick(C_GameMode* gm) override;
	virtual const char* getModuleName();
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onMove(C_MoveInputHandler* input) override;
};