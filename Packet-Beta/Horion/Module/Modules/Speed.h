#pragma once
#include "..\ModuleManager.h"
#include "Module.h"

class Speed : public IModule {
private:
	bool jumpMode = true;
	bool onGround = false;
	float height = 0.40f;
	int counter = 1;

public:
	int timer = 20;
	bool lowMode = true;
	bool hive = false;
	float speed = 0.325f;
	Speed();
	~Speed();

			SettingEnum mode = this;

	// Inherited via IModule
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onMove(C_MoveInputHandler* input) override;
};