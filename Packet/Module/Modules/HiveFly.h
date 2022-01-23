#pragma once
#include "Module.h"
class HiveFly : public IModule {
private:
public:
	HiveFly();
	~HiveFly();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onMove(C_MoveInputHandler* input) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onLevelRender();
	virtual void onSendPacket(C_Packet* packet);
};