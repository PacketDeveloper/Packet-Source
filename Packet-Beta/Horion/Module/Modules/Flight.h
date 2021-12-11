#pragma once
#include "Module.h"
class Flight : public IModule {
private:
	float glideModEffective = 0;
	float glideMod = 0.00f;
	bool hiveMode = false;
	bool damage = false;

public:
	C_MovePlayerPacket* jetpack = nullptr;
	int counter = 1;
	int counter2 = 1;
	int counter3 = 1;
	float speed2 = 0.f;
	float speed = 1.f;

	// BlockFly
	bool buildBlock(vec3_t blockBelow);
	int placeCounter = 1;
	int placeDelay = 8;
	bool selectBlock();
	int prevSlot = 0;

		SettingEnum mode = this;

	Flight();
	~Flight();

	// Inherited via IModule
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual bool isFlashMode() override;
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual const char* getRawModuleName() override;
	virtual void onMove(C_MoveInputHandler* input) override;
};