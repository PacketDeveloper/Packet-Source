#include "NoFall.h"

NoFall::NoFall() : IModule(0, Category::MOVEMENT, "NoFuK") {
	registerEnumSetting("Mode", &this->mode, 0);
	mode.addEntry("Packet", 0);
	mode.addEntry("Hive", 1);
}

const char* NoFall::getModuleName() {
	return "NoFall";
}

void NoFall::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (mode.getSelectedValue() == 0) {  // Packet
		C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
	}
	if (mode.getSelectedValue() == 1) {  // Hive
		vec3_t pPos = g_Data.getLocalPlayer()->eyePos0;
		vec3_t blockBelow = player->eyePos0;
		blockBelow.y -= player->height;
		blockBelow.y -= 1.5f;
		vec3_t pos;
		if (player->fallDistance >= 4) {
			if (((player->region->getBlock(blockBelow)->blockLegacy))->blockId != 0 && ((player->region->getBlock(blockBelow)->blockLegacy))->material->isSolid) {
				gm->player->velocity = vec3_t(0, 0, 0);
				pos.x = 0.f + pPos.x;
				pos.y = 0.3f + pPos.y;
				pos.z = 0.f + pPos.z;
				g_Data.getLocalPlayer()->setPos(pos);
			}
		}
	}
}