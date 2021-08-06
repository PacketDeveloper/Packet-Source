#include "TPAura.h"

TPAura::TPAura() : IModule(0, Category::COMBAT, "TPS to the closest entity") {
	registerBoolSetting("Silent", &silent, silent);
	registerBoolSetting("Push", &push, push);
	registerIntSetting("TP Delay", &delay, delay, 0, 10);
	registerFloatSetting("Range", &range, range, 1.f, 250.f);
}

const char* TPAura::getModuleName() {
	return ("TPAura");
}

// gonna remake tpaura as it crashes alot

TPAura::~TPAura() {
}

void TPAura::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		setEnabled(false);
}

void TPAura::onTick(C_GameMode* gm) {

}

void TPAura::onSendPacket(C_Packet* packet) {
	/*if (!silent) {
		if (packet->isInstanceOf<C_MovePlayerPacket>() && g_Data.getLocalPlayer() != nullptr && silent) {
			if (!targetList.empty()) {
				auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
				vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
				movePacket->pitch = angle.x;
				movePacket->headYaw = angle.y;
				movePacket->yaw = angle.y;
			}
		}
	}*/
}