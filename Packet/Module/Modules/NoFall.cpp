#include "NoFall.h"

NoFall::NoFall() : IModule(0, Category::EXPLOIT, "NoFuK") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Packet", 0);
	mode.addEntry("Motion", 1);
	mode.addEntry("Hive", 2);
	registerIntSetting("Distance", &dist, dist, 3, 8);
}

const char* NoFall::getModuleName() {
	return "NoFall";
}

void NoFall::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	vec3_t pPos = g_Data.getLocalPlayer()->eyePos0;
	vec3_t blockBelow = player->eyePos0;
	blockBelow.y -= player->height;
	blockBelow.y -= 2.62f;
	vec3_t pos;
	if (mode.getSelectedValue() == 0) {  // Packet
		C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
		p.onGround = true;
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
	}
	if (mode.getSelectedValue() == 1 && gm->player->fallDistance >= dist) {  // Motion
		gm->player->velocity.y = 0.0001f;
		gm->player->fallDistance = 0;
	}
	if (mode.getSelectedValue() == 2) {  // Hive
		if (player->fallDistance >= dist) {
			C_BlockLegacy* blockLegacy = (player->region->getBlock(blockBelow)->blockLegacy);
			if (blockLegacy->blockId != 0 && blockLegacy->material->isSolid) {
				gm->player->velocity = vec3_t(0, 0, 0);
				gm->player->fallDistance = 0;
				pos.x = 0.f + pPos.x;
				pos.y = 0.3f + pPos.y;
				pos.z = 0.f + pPos.z;
				g_Data.getLocalPlayer()->setPos(pos);
			}
		}
	}
}