#pragma once
#include "Module.h"
#include "../ModuleManager.h"

class Breaker : public IModule {
private:
	bool treasures = false;
	bool carrots = false;
	bool chests = false;
	bool cakes = false;
	bool beds = false;
	bool eggs = false;
	bool rOre = false;
	bool dOre = false;
	bool eOre = false;
	int counter = 1;
	int Odelay = 0;
	int delay = 0;
	int range = 5;

public:
	Breaker();
	~Breaker();

	// Inherited via IModule
	virtual void onDisable() override;
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};