#include "InPvPFucker.h"

InPvPFucker::InPvPFucker() : IModule(0, Category::EXPLOIT, "fucks inpvp so hard it dies") {
	registerEnumSetting("Map", &mode, 0);
	mode.addEntry("TW-Kingdom", 0);
	mode.addEntry("TW-Solar", 1);
	registerBoolSetting("TowerWars", &towerWars, towerWars);
	registerBoolSetting("RedTeam", &redTeam, redTeam);
	registerBoolSetting("Skywars", &skyWars, skyWars);
	registerBoolSetting("AutoPlay", &newGame, newGame);
	registerIntSetting("TPS", &tps, tps, 20, 350);
}

const char* InPvPFucker::getRawModuleName() {
	return "InPvPFucker";
}

const char* InPvPFucker::getModuleName() {
	if (towerWars) {
		name = std::string("InPvPFucker ") + std::string(GRAY) + std::string("TW");
		return name.c_str();
	} else if (skyWars) {
		name = std::string("InPvPFucker ") + std::string(GRAY) + std::string("SW");
		return name.c_str();
	} 
	if (!towerWars || !skyWars) {
		return "InPvPFucker";
	}
}

static std::vector<C_Entity*> targetList;

void InPvPNuker(C_Entity* currentEntity, bool isRegularEntity) {
	static auto fuckYou = moduleMgr->getModule<InPvPFucker>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (currentEntity->getEntityTypeId() != 319)  // Players
		return;

	if (!Target::isValidTarget(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < 250) {
		targetList.push_back(currentEntity);
	}
}

void InPvPFucker::onEnable() {

}

void InPvPFucker::onTick(C_GameMode* gm) {
	if (towerWars && skyWars) {
		clientMessageF("[%sPacket%s] %sYou cant have both modes on....", GRAY, WHITE, RED);
		towerWars = false;
		skyWars = false;
	}

	targetList.clear();
	g_Data.forEachEntity(InPvPNuker);

	if (count == 15) {
		count = 1;
	} else {
		count++;
	}

	if (towerWars) {
			PointingStruct* pstruct = g_Data.getClientInstance()->getPointerStruct();
			Odelay++;
			if (Odelay >= delay) {
				g_Data.rightclickCount++;
				gm->buildBlock(new vec3_ti(pstruct->block), pstruct->blockSide);
				Odelay = 0;
			}

		vec3_t* pos = gm->player->getPos();
		for (int x = (int)pos->x - 5; x < pos->x + 5; x++) {
			for (int z = (int)pos->z - 5; z < pos->z + 5; z++) {
				for (int y = (int)pos->y - 5; y < pos->y + 5; y++) {
					vec3_ti blockPos = vec3_ti(x, y, z);
					bool getFlag = false;
					bool blue = false;
					bool red = false;
					int id = (int)gm->player->region->getBlock(blockPos)->toLegacy()->blockId;

					if (id == 176 && towerWars) getFlag = true;  // Banner
					if (id == 26 && towerWars) blue = true;  // Team - Blue
					if (id == 35 && towerWars) red = true;   // Team - Red

					if (getFlag) {
						gm->destroyBlock(&blockPos, 0);
						gm->buildBlock(&blockPos, 0);
						return;
					}
					/*if (blue) {
						blueTeam = true;
						return;
					} else {
						blueTeam = false;
					}
					if (red) {
						redTeam = true;
						return;
					} else {
						redTeam = false;
					}*/
				}
			}
		}

		// TODO: Get flag and bring back to base
		*g_Data.getClientInstance()->minecraft->timer = static_cast<float>(this->tps);
		if (redTeam) { // Red team (duh)
			vec3_t pos;
			if (mode.getSelectedValue() == 0) { // Map: Kingdom
				if (count == 2) { // Blue Banner
					pos.x = -205;
					pos.y = 70;
					pos.z = 909;

					g_Data.getLocalPlayer()->setPos(pos);
				}
				if (count == 7) { // Base
					pos.x = -245;
					pos.y = 55;
					pos.z = 750;
					g_Data.getLocalPlayer()->setPos(pos);
				}
			} else if (mode.getSelectedValue() == 1) { // Map: Solar
				if (count == 2) {  // Blue Banner
					pos.x = -260;
					pos.y = 67;
					pos.z = 684;

					g_Data.getLocalPlayer()->setPos(pos);
				}
				if (count == 7) {  // Base
					pos.x = -220;
					pos.y = 55;
					pos.z = 850;
					g_Data.getLocalPlayer()->setPos(pos);
				}
			}
		} else { // Blue team
			vec3_t pos;
			if (mode.getSelectedValue() == 0) { // Map: Kingdom
				if (count == 2) { // Red Banner
					pos.x = -285;
					pos.y = 70;
					pos.z = 751;

					g_Data.getLocalPlayer()->setPos(pos);
				}
				if (count == 7) { // Base
					pos.x = -245;
					pos.y = 55;
					pos.z = 750;
					g_Data.getLocalPlayer()->setPos(pos);
				}
			}
			if (mode.getSelectedValue() == 1) { // Map: Solar
				if (count == 2) { // Red Banner
					pos.x = -180;
					pos.y = 65;
					pos.z = 850;

					g_Data.getLocalPlayer()->setPos(pos);
				}
				if (count == 7) {  // Base
					pos.x = -220;
					pos.y = 55;
					pos.z = 684;
					g_Data.getLocalPlayer()->setPos(pos);
				}
			}
		}
	}
	if (skyWars) {
		auto player = g_Data.getLocalPlayer();
		vec3_t pos;
		*g_Data.getClientInstance()->minecraft->timer = 800;
		float calcYaw = (gm->player->yaw + 90) * (PI / 180);
		float calcPitch = (gm->player->pitch) * -(PI / 180);

		float teleportX2 = cos(calcYaw) * cos(calcPitch) * 3.5f;
		float teleportZ2 = sin(calcYaw) * cos(calcPitch) * 3.5f;
		C_MovePlayerPacket teleportPacket2;
		for (int i = 0; i < targetList.size(); i++) {
			vec3_t pos = *targetList[i]->getPos();
			teleportPacket2 = C_MovePlayerPacket(g_Data.getLocalPlayer(), vec3_t(pos.x - teleportX2, pos.y, pos.z - teleportZ2));
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket2);
			gm->attack(targetList[i]);
			teleportPacket2 = C_MovePlayerPacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket2);
		}
		player->velocity.x *= 0;
		player->velocity.y = 0;
		player->velocity.z *= 0;
		if (count == 2) {
			pos.x = 0;
			pos.y = 110;
			pos.z = 0;
			g_Data.getLocalPlayer()->setPos(pos);
		}
	}

	if (newGame) {
		g_Data.forEachEntity([](C_Entity* ent, bool b) {
			std::string name = ent->getNameTag()->getText();
			int id = ent->getEntityTypeId();
			if (name.find("Play Again") != std::string::npos && g_Data.getLocalPlayer()->getPos()->dist(*ent->getPos()) <= 50) {
				g_Data.getCGameMode()->attack(ent);
			}
		});
	}
}

void InPvPFucker::onSendPacket(C_Packet* packet) {
	auto player = g_Data.getLocalPlayer();

	// Disabler
	if (packet->isInstanceOf<C_MovePlayerPacket>()) {
		if (!player->onGround) {
			C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			movePacket->onGround = true;
		}
	}
	if (packet->isInstanceOf<C_MovePlayerPacket>()) {
		if (!player->onGround && g_Data.isInGame() && g_Data.canUseMoveKeys()) {
			C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			if (g_Data.canUseMoveKeys()) {
				movePacket->onGround = true;
			}
		}
	}
}

void InPvPFucker::onPostRender(C_MinecraftUIRenderContext* renderCtx) {

}

void InPvPFucker::onDisable() {
	*g_Data.getClientInstance()->minecraft->timer = 20;
	// TP back to base
	if (towerWars) {
		if (redTeam) {  // Red team (duh)
			vec3_t pos;
			if (mode.getSelectedValue() == 0) {  // Map: Kingdom
				pos.x = -245;
				pos.y = 55;
				pos.z = 750;
				g_Data.getLocalPlayer()->setPos(pos);
			} else if (mode.getSelectedValue() == 1) {  // Map: Solar
				pos.x = -220;
				pos.y = 55;
				pos.z = 850;
				g_Data.getLocalPlayer()->setPos(pos);
			}
		} else {  // Blue team
			vec3_t pos;
			if (mode.getSelectedValue() == 0) {  // Map: Kingdom
				pos.x = -245;
				pos.y = 55;
				pos.z = 750;
				g_Data.getLocalPlayer()->setPos(pos);
			}
			if (mode.getSelectedValue() == 1) {  // Map: Solar
				pos.x = -220;
				pos.y = 55;
				pos.z = 684;
				g_Data.getLocalPlayer()->setPos(pos);
			}
		}
	}
}