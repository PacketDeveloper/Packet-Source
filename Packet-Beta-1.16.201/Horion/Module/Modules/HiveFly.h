#pragma once
#include "Module.h"
class HiveFly : public IModule {
private:
public:
	int testTimer = 0;
	int counter = 1;
	int boostcounter = 1;
	bool strafeMode = false;
	bool blinkMode = false;
	bool timerMode = false;
	bool damageMode = false;
	bool oldMode = false;
	bool boostMode = true;
	float TimerOFG = 20;
	float speedA = 0.30183640122413635;  // Speed
	float TimerUp = 20;
	int delay = 0;
	int timer = 60;
	bool penis = true;
	float speed = 0.225f;
	float glideMod = -0.03f;
	float glideModEffective = 0;
	float speed2 = -0.004993199370801449;  // Value
	float Up = .01;
	float UpSpeed = 0.1;
	float Test = 0.005f;
	HiveFly();
	~HiveFly();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onDisable() override;
	virtual bool isFlashMode() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onMove(C_MoveInputHandler* input) override;
};