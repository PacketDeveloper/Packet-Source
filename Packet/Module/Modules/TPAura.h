#pragma once
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class TPAura : public IModule {
public:
	bool hurttime = false;
	bool silent = false;
	float range = 250;
	float cCounter = 1;
	bool push = false;
	bool multi = true;
	int targethud = 0;
	int Odelay = 0;
	int delay = 0;
	float rangeSpeed = 1.f;
	int entDistance = 2;
	float entHeight = 1.62f;

	TPAura();
	~TPAura();

	// Inherited via IModule
	virtual void onEnable() override;
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};