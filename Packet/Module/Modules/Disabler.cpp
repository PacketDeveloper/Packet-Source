#include "Disabler.h"

#include "../../../Utils/Target.h"
#include "../ModuleManager.h"

Disabler::Disabler() : IModule(0, Category::EXPLOIT, "Disables AntiCheats") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Nethergames", 0);
	mode.addEntry("Mineville", 1);
	mode.addEntry("Hive", 2);
}

Disabler::~Disabler() {
}

const char* Disabler::getRawModuleName() {
	return "Disabler";
}

const char* Disabler::getModuleName() {
	if (mode.getSelectedValue() == 0) {
		name = std::string("Disabler ") + std::string(GRAY) + std::string("NG");
		return name.c_str();
	}
	if (mode.getSelectedValue() == 1) {
		name = std::string("Disabler ") + std::string(GRAY) + std::string("Mineville");
		return name.c_str();
	}
	if (mode.getSelectedValue() == 2) {
		name = std::string("Disabler ") + std::string(GRAY) + std::string("Hive");
		return name.c_str();
	}
}

void Disabler::onEnable() {
	if (mode.getSelectedValue() == 3)
		counter = 1;
}

void Disabler::onTick(C_GameMode* gm) {
	auto speed = moduleMgr->getModule<Speed>();
	auto d = moduleMgr->getModule<Disabler>();
	//if (g_Data.canUseMoveKeys()) {
	if (mode.getSelectedValue() == 0 && !gm->player->onGround) {
		C_MovePlayerPacket pNether(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&pNether);
	}
	if (mode.getSelectedValue() == 1 && !gm->player->onGround) {
		C_MovePlayerPacket Mineville(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&Mineville);
	}
	//}
	if (mode.getSelectedValue() == 2) {
		auto player = g_Data.getLocalPlayer();
#ifdef _DEBUG
		int timeEnabled = 0;
		if (counter == 4) {
			//setEnabled(false);
			counter = 1;
		} else {
			timeEnabled++;
			counter++;
		}
		if (gm->player->damageTime >= 1 && counter == 3) {
			speed->setEnabled(true);
		} else {
			speed->setEnabled(false);
		}
		if (timeEnabled >= 10) setEnabled(false);
#endif
	}
}

void Disabler::onSendPacket(C_Packet* packet) {
	//if (g_Data.canUseMoveKeys()) {
	if (packet->isInstanceOf<C_MovePlayerPacket>()) {
		auto player = g_Data.getLocalPlayer();
		if (mode.getSelectedValue() == 1 && !player->onGround) {
			C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			movePacket->onGround = true;
		}
	}
	if (packet->isInstanceOf<C_MovePlayerPacket>()) {
		auto player = g_Data.getLocalPlayer();
		if (mode.getSelectedValue() == 1 && !player->onGround) {
			C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			if (g_Data.canUseMoveKeys()) {
				movePacket->onGround = true;
			}
		}
	}
	//}
}