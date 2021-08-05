#pragma once
#include "Module.h"

class Disabler : public IModule {
public:
	int counter = 1;
	SettingEnum mode = this;

	Disabler();
	~Disabler();

	// Inherited via IModule
	virtual void onEnable() override;
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onSendPacket(C_Packet* packet) override;
};