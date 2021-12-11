#pragma once

#include "Module.h"

class HiveFly : public IModule {
public:
	HiveFly();
	virtual const char* getModuleName();
	virtual void onTick(C_GameMode* gm) override;
	virtual void onMove(C_MoveInputHandler* input) override;
	float hivespeed = 0.3f;
	virtual void onDisable() override;

};