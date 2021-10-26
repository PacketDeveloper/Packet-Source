#include "Derp.h"

Derp::Derp() : IModule(0, Category::MISC, "lol you stupid") {
	registerBoolSetting("Headless", &headless, headless);
	registerBoolSetting("Twerk", &twerk, twerk);
	registerBoolSetting("Spin", &spin, spin);
	registerBoolSetting("Silent", &silent, silent);
	registerFloatSetting("Delay", &delay, delay, 0.f, 10.f);
}

Derp::~Derp() {
}

const char* Derp::getModuleName() {
	return "Derp";
}

void Derp::onEnable() {

}

void Derp::onTick(C_GameMode* gm) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	auto player = g_Data.getLocalPlayer();

	if (silent)
		return;

	Odelay++;
	if (Odelay > delay) {
		if (headless) {
			gm->player->pitch = -180;
		}
		if (twerk) {
			g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = true;
		} else {
			g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
		}
		if (spin) {

		}
	}
	Odelay = 0;
}

void Derp::onSendPacket(C_Packet* packet) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	auto player = g_Data.getLocalPlayer();
	vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;
	blockBelow.y -= g_Data.getLocalPlayer()->height;
	blockBelow.y -= 0.5f;

	auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
	auto* authPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
	if (packet->isInstanceOf<C_MovePlayerPacket>() || packet->isInstanceOf<PlayerAuthInputPacket>()) {
		if (g_Data.getLocalPlayer() != nullptr && g_Data.canUseMoveKeys() && silent) {
			if (headless) movePacket->pitch = -180;
			if (headless) authPacket->pitch = -180;
		}
	}
}
