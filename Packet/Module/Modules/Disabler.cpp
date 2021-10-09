#include "Disabler.h"

#include "../../../Utils/Target.h"
#include "../ModuleManager.h"

Disabler::Disabler() : IModule(0, Category::EXPLOIT, "Disables AntiCheats") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Nethergames", 0);
	mode.addEntry("Mineville", 1);
	//mode.addEntry("Hive", 2);
#ifdef _DEBUG
	mode.addEntry("Hive2", 3);
#endif
}

Disabler::~Disabler() {
}

static std::vector<C_Entity*> disableList;

void disableAC(C_Entity* currentEntity, bool isRegularEntity) {
	if (currentEntity == nullptr)
		return;

	if (!Target::isValidTarget(currentEntity))
		return;

	if (currentEntity->getEntityTypeId() != 319)
		return;

	disableList.push_back(currentEntity);
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
	if (mode.getSelectedValue() == 2 || mode.getSelectedValue() == 3) {
		name = std::string("Disabler ") + std::string(GRAY) + std::string("Hive");
		return name.c_str();
	}
}

void Disabler::onEnable() {
	if (mode.getSelectedValue() == 3) {
		counter = 1;
		shouldDisable = false;
		c2 = 1;
	}
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
	if (mode.getSelectedValue() == 3) {  // Hive private
#ifdef _DEBUG
		auto player = g_Data.getLocalPlayer();
		disableList.clear();
		g_Data.forEachEntity(disableAC);

		if (counter == 3) {
			counter = 1;
		} else {
			counter++;
		}
		if (c2 == INFINITY) {
			c2 = 1;
		} else {
			c2++;
		}

		if (player->damageTime >= 1) {
			if (counter == 2) {
				speed->setEnabled(true);
			} else {
				speed->setEnabled(false);
			}
			if (c2 >= 28 && !disableList.empty()) {
				player->knockback(player, 0, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f);
				//clientMessageF("kb");
			}
			shouldDisable = true;
		} else if (shouldDisable) {
			speed->setEnabled(false);
			setEnabled(false);
		}
#endif
	}
}

void Disabler::onSendPacket(C_Packet* packet) {
	//if (g_Data.canUseMoveKeys()) {
	auto player = g_Data.getLocalPlayer();
	if (packet->isInstanceOf<C_MovePlayerPacket>()) {
		if (mode.getSelectedValue() == 1 && !player->onGround) {
			C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			movePacket->onGround = true;
		}
	}
	if (packet->isInstanceOf<C_MovePlayerPacket>()) {
		if (mode.getSelectedValue() == 1 && !player->onGround) {
			C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			if (g_Data.canUseMoveKeys()) {
				movePacket->onGround = true;
			}
		}
	}
	//}
}