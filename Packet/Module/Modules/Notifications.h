#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class Notifications : public IModule {
private:
public:
	bool firstLetter = false;
	bool modInfo = false;
	bool enable = true;
	bool disable = true;
	float opacity = 0.65f;

	Notifications();
	~Notifications();

	SettingEnum mode = this;

	// Inherited via IModule
	virtual void onEnable() override;
	virtual const char* getModuleName() override;
};