#pragma once
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class BehindAura : public IModule {
private:
	float lerpSpeed = 1;
	float behindDist = 2.f;
	bool useLerp = true;
	bool usePos = false;
	bool useAttack = false;
	bool useTick = false;

	bool basicCheck = true;
	bool calcYawCheck = false;

public:
	bool isMobAura = false;
	bool hurttime = false;
	float range = 6;

	BehindAura();
	~BehindAura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onAttack(C_Entity* attackedEnt) override;
};