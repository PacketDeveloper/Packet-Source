#pragma once
#include "Module.h"
class Scaffold : public IModule {
private:
	bool timerBoost = true;
	bool switchMode = true;
	bool extendMode = true;
	bool staircase = true;
	bool airplace = false;
	bool rotations = true;
	bool spoof = false;
	float towerSpeed = 0.45f;
	int prevSlot = 0;
	int expand = 1;

	bool tryScaffold(vec3_t blockBelow);
	bool tryTower(vec3_t blockBelow);
	bool selectBlock();
	bool findBlock();

	int yLock = -1;

public:
	bool towerMode = true;
	int timer = 20;
	int tCounter = 1;
	bool lockY = false;
	bool speedLockY = false;

	// stupid
	bool forward = false;
	bool backwards = false;

	Scaffold();
	~Scaffold();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onMove(C_MoveInputHandler* input) override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};