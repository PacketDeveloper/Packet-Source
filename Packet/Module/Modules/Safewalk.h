#pragma once

#include "Module.h"

class Safewalk : public IModule {
private:
	bool onEdge = false;

public: 

	Safewalk();
		SettingEnum mode = this;

	virtual void onDisable() override;
	virtual const char* getModuleName();
	virtual void onTick(C_GameMode* gm) override;
};