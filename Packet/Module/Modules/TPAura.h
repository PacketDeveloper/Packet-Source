#pragma once
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class TPAura : public IModule {
public:
	bool visualize = true;
	bool multi = false;
	float range = 250;
	int targethud = 0;
	int Odelay = 0;
	int delay = 0;

	// Rotations
	bool targetListEmpty = true;
	vec4_t tpAuraRot;

	TPAura();
	~TPAura();

		//std::string name = "TPAura";
		SettingEnum mode = this;

	// Inherited via IModule
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onLevelRender() override;
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	//virtual const char* getRawModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};