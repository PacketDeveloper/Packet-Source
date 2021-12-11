#pragma once
#include "Module.h"
class Scaffold : public IModule {
private:
	bool spoof = false;
	bool staircaseMode = false;
	bool lockY = false;
	int timer = 125;
	bool tryScaffold(vec3_t blockBelow);
	bool findBlock();

	int yLock = -1;

public:
	Scaffold();
	~Scaffold();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};