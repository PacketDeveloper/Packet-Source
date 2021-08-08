#pragma once
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class TPAura : public IModule {
public:
	float range = 9.f;
	bool push = true;
	int delay = 0;
	int delay0 = 0;
	float rangeSpeed = 1.f;
	int entDistance = 2;
	float entHeight = 1.62f;

	TPAura();
	~TPAura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
};