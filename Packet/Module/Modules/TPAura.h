#pragma once
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class TPAura : public IModule {
public:
	bool hurttime = false;
	bool attack = true;
	bool silent = false;
	float range = 250;
	float cCounter = 1;
	bool push = false;
	bool multi = true;
	int targethud = 0;
	int Odelay = 0;
	bool hive = false;
	int delay = 0;
	float rangeSpeed = 1.f;
	int entDistance = 2;
	float entHeight = 1.62f;
	bool rangeSet = false;
	float teleportY = 2;
	vec3_t prevPos;
	int tpBack = 1;

	// Rotations
	bool targetListEmpty = true;
	bool rotations = true;
	vec2_t tpAuraRot;

	TPAura();
	~TPAura();

		SettingEnum mode = this;

	// Inherited via IModule
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};