#pragma once
#include "Module.h"
class Flight : public IModule {
private:
	float effectiveValue = 0;
	float value = 0.00f;
	bool hiveMode = false;
	bool damage = false;
	bool boost = false;
	bool speedWasEnabled = false;
	float height = 0.f;
	float jumpHeight = 0.f;
	int tick = 0;

public:
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

	// Hive
	bool throwPearl = false;
	bool blink = false;
	int prevSlot = 0;
	int Odelay = 0;

		std::string name = "Flight";
		C_MovePlayerPacket* jetpack = nullptr;
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
	virtual void onSendPacket(C_Packet* packet) override;
	virtual void onMove(C_MoveInputHandler* input) override;

	std::vector<C_MovePlayerPacket> movePacket1;
	std::vector<PlayerAuthInputPacket> authPacket1;
	std::vector<C_MovePlayerPacket*> MovePlayerPacketHolder;
	std::vector<PlayerAuthInputPacket*> PlayerAuthInputPacketHolder;
	inline std::vector<C_MovePlayerPacket*>* getMovePlayerPacketHolder() { return &MovePlayerPacketHolder; };
	inline std::vector<PlayerAuthInputPacket*>* getPlayerAuthInputPacketHolder() { return &PlayerAuthInputPacketHolder; };
};