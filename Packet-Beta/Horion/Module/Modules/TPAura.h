#pragma once
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class TPAura : public IModule {
public:
	int targethud = 0;
	bool targethudEnabled = false;
	bool rot = false;
	bool silent = false;
	bool cock = false;
	bool packetcock = false;
	bool isMulti = true;
	bool distanceCheck = false;
	bool isMobAura = false;
	bool hurttime = false;
	int delay = 0;
	int Odelay = 0;
	bool push = false;
	bool enabled = true;
	float range = 250;

	TPAura();
	~TPAura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onSendPacket(C_Packet* packet) override;
};