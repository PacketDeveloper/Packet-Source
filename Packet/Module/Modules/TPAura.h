#pragma once
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class TPAura : public IModule {
public:
	bool hurttime = false;
	bool silent = false;
	float range = 250;
	bool push = false;
	int targethud = 0;
	int Odelay = 0;
	int delay = 0;

		SettingEnum mode = this;

	TPAura();
	~TPAura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onSendPacket(C_Packet* packet) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};