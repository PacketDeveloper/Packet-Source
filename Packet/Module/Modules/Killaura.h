#pragma once
#include "Module.h"
#include "../ModuleManager.h"
#include "../../../Utils/Target.h"

class Killaura : public IModule
{
private:
	int Odelay = 0;
	float height = 1.f;
	bool autoweapon = false;
	bool showtarget = false;
	bool render = false;
	int renderStart = 0;
	bool hold = false;

public:
	bool targetListA = false;
	vec2_t testRot;
	bool test = false;
	bool useSprint = true;
	bool targethudEnabled = false;
	int targethud = 0;
	bool rot = true;
	bool click = false;
	bool strafe = false;
	int delay = 0;
	bool packetcock = false;
	bool silent = false;
	bool distanceCheck = false;
	bool isMobAura = false;
	bool hurttime = false;
	float range = 8;

	int* offset = nullptr;
	int* offset2 = nullptr;
	int offsetProt = 0;
	int offsetProt2 = 0;

	SettingEnum mode = this;

	Killaura();
	~Killaura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onLevelRender() override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onSendPacket(C_Packet* packet) override;
};