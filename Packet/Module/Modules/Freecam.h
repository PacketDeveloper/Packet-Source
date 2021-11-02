#pragma once
#include "Module.h"
class Freecam : public IModule {
private:
	bool enabled = true;
	float speed = 1.f;
	vec3_t oldPos;

public:
	Freecam();
	~Freecam();

	// Inherited via IModule
	virtual void onMove(C_MoveInputHandler* input) override;
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};