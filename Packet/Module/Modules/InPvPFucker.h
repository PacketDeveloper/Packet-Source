#pragma once

#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class InPvPFucker : public IModule {
public:
	bool towerWars = false;
	bool skyWars = false;
	bool newGame = false;
	int Odelay = 0;
	int count = 0;
	int delay = 0;
	int tps = 250;

	// Teams
	bool blueTeam = false;
	bool redTeam = false;

	InPvPFucker();

	std::string name = ("InPvPFucker");
	SettingEnum mode = this;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual const char* getRawModuleName() override;
	virtual void onSendPacket(C_Packet* packet) override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};