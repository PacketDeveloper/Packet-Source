#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class EntityControl : public IModule {
public:
	bool control = false;
	float speed = 0.f;
	float range = 8;
	
	EntityControl();
	~EntityControl();

	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual const char* getModuleName();
	virtual void onTick(C_GameMode* gm) override;
	virtual void onMove(C_MoveInputHandler* input) override;
};