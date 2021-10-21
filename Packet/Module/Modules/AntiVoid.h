#pragma once
#include "..\ModuleManager.h"
#include "Module.h"

class AntiVoid : public IModule {
private:
	bool foundVoid = false;
	bool voidCheck = true;
	int distance = 7;
	vec3_t savedPos;

public:
	AntiVoid();
	~AntiVoid();

	C_MoveInputHandler* inputHandler = nullptr;
	SettingEnum mode = this;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onDisable() override;
	virtual void onTick(C_GameMode* gm) override;
};
