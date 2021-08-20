#pragma once

#include "Module.h"

class Survival : public IModule {
public:
	bool liquidNuker = false;
	bool antiHunger = false;
	bool treeNuker = false;
	bool oreNuker = false;
	bool spawntp = false;
	bool liquidN = true;
	bool nofall = false;
	bool itemtp = false;
	bool treeN = true;
	bool test = false;
	bool oreN = true;
	bool fly = false;
	int testTimer = 0;
	//int delay = 0;
	int timer = 20;
	int range = 4;

	bool config = false;

	int delay = 0;
	vec3_t prevPos;
	int Odelay = 0;

	Survival();
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual const char* getModuleName();
	virtual void onTick(C_GameMode* gm) override;
	virtual void onSendPacket(C_Packet* packet) override;
	virtual void onMove(C_MoveInputHandler* input) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};