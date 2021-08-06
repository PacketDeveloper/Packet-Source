#include "InfiniteReach.h"

InfiniteReach::InfiniteReach() : IModule(0, Category::COMBAT, "Killaura with infinite reach") {
	this->registerBoolSetting("multiaura", &this->isMulti, this->isMulti);
	this->registerFloatSetting("range", &this->range, this->range, 1, 250);
	this->registerIntSetting("delay", &this->delay, this->delay, -50, 15);
}

InfiniteReach::~InfiniteReach() {
}

const char* InfiniteReach::getModuleName() {
	return ("InfiniteAura");
}

void InfiniteReach::onTick(C_GameMode* gm) {
	/*//Loop through all our players and retrieve their information
	targetList0.clear();

	g_Data.forEachEntity(findEntities);
	Odelay++;

	if (targetList0.size() > 0 && Odelay >= delay) {
		if (!moduleMgr->getModule<NoSwing>()->isEnabled()) 
			g_Data.getLocalPlayer()->swingArm();

		float calcYaw = (gm->player->yaw + 90) * (PI / 180);
		float calcPitch = (gm->player->pitch) * -(PI / 180);

		float teleportX = cos(calcYaw) * cos(calcPitch) * 3.5f;
		float teleportZ = sin(calcYaw) * cos(calcPitch) * 3.5f;
		C_MovePlayerPacket teleportPacket;

		// Attack all entitys in targetList
		if (isMulti) {
			for (int i = 0; i < targetList0.size(); i++) {
				vec3_t pos = *targetList0[i]->getPos();
				teleportPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), vec3_t(pos.x - teleportX, pos.y, pos.z - teleportZ));
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket);
				g_Data.getCGameMode()->attack(targetList0[i]);
				teleportPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket);
			}
		} else {
			vec3_t pos = *targetList0[0]->getPos();
			teleportPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), vec3_t(pos.x - teleportX, pos.y, pos.z - teleportZ));
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket);
			g_Data.getCGameMode()->attack(targetList0[0]);
			teleportPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket);
		}
		Odelay = 0;
	}*/
}
