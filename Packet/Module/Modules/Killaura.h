#pragma once
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class Killaura : public IModule {
private:
	int renderStart = 0;
	bool test = false;

public:
	bool distanceCheck = false;
	bool useSprint = true;
	bool multi = true;
	bool click = false;
	bool hold = false;
	bool mobAura = false;
	bool hurttime = false;
	bool render = true;
	int targethud = 0;
	float range = 8;
	int Odelay = 0;
	int delay = 0;

	// Rotations
	bool targetListEmpty = true;
	bool rotations = true;
	vec4_t testRot;

	std::string name = "Killaura";
	SettingEnum mode = this;

	Killaura();
	~Killaura();

	// Inherited via IModule
	virtual void onSendPacket(C_Packet* packet) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onLevelRender() override;
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual const char* getRawModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};