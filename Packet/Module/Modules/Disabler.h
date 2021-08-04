#pragma once
#include "Module.h"

class Disabler : public IModule {
public:
	SettingEnum mode = this;

	Disabler();
	~Disabler();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onSendPacket(C_Packet* packet) override;
};