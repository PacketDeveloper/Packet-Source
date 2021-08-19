#include "../ModuleManager.h"
#include "../../DrawUtils.h"
#include "Survival.h"

Survival::Survival() : IModule(0, Category::MISC, "Automates Survival") {
	//registerBoolSetting("AntiHunger", &antiHunger, antiHunger);
	registerBoolSetting("LiquidNuker", &liquidNuker, liquidNuker);
	registerBoolSetting("TreeNuker", &treeNuker, treeNuker);
	registerBoolSetting("OreNuker", &oreNuker, oreNuker);
	registerBoolSetting("NoFall", &nofall, nofall);
	registerBoolSetting("Fly", &fly, fly);
	registerBoolSetting("Test", &test, test);
	registerIntSetting("Timer", &timer, timer, 20, 500);
}

const char* Survival::getModuleName() {
	return "Survival";
}

void Survival::onEnable() {
}

void Survival::onTick(C_GameMode* gm) {
	*g_Data.getClientInstance()->minecraft->timer = static_cast<float>(timer);
	auto player = g_Data.getLocalPlayer();
	if (spawntp) {
		player->setSleeping(true);
		setEnabled(false);
	}
	if (test) {
	}
	if (treeNuker) {
		vec3_t* pos = gm->player->getPos();
		for (int x = (int)pos->x - range; x < pos->x + range; x++) {
			for (int z = (int)pos->z - range; z < pos->z + range; z++) {
				for (int y = (int)pos->y - range; y < pos->y + range; y++) {
					vec3_ti blockPos = vec3_ti(x, y, z);
					bool destroy = false;
					int id = gm->player->region->getBlock(blockPos)->toLegacy()->blockId;
					if (id == 17 && treeN) destroy = true;  // Logs
					if (id == 18 && treeN) destroy = true;  // Leaves
					if (destroy) {
						gm->destroyBlock(&blockPos, 0);
						return;
					}
				}
			}
		}
	}
	if (liquidNuker) {
		vec3_t* pos = gm->player->getPos();
		for (int x = (int)pos->x - range; x < pos->x + range; x++) {
			for (int z = (int)pos->z - range; z < pos->z + range; z++) {
				for (int y = (int)pos->y - range; y < pos->y + range; y++) {
					vec3_ti blockPos = vec3_ti(x, y, z);
					bool destroy = false;
					int id = gm->player->region->getBlock(blockPos)->toLegacy()->blockId;
					if (id == 10 && liquidN) destroy = true;  // Flowing Lava
					if (id == 11 && liquidN) destroy = true;  // Lava
					if (id == 8 && liquidN) destroy = true;   // Flowing Water
					if (id == 9 && liquidN) destroy = true;   // Water
					if (destroy) {
						gm->destroyBlock(&blockPos, 0);
						return;
					}
				}
			}
		}
	}
	if (oreNuker) {
		vec3_t* pos = gm->player->getPos();
		for (int x = (int)pos->x - range; x < pos->x + range; x++) {
			for (int z = (int)pos->z - range; z < pos->z + range; z++) {
				for (int y = (int)pos->y - range; y < pos->y + range; y++) {
					vec3_ti blockPos = vec3_ti(x, y, z);
					bool destroy = false;
					int id = gm->player->region->getBlock(blockPos)->toLegacy()->blockId;
					if (id == 14 && oreN) destroy = true;
					if (id == 15 && oreN) destroy = true;
					if (id == 16 && oreN) destroy = true;
					if (id == 21 && oreN) destroy = true;
					if (id == 56 && oreN) destroy = true;
					if (id == 73 && oreN) destroy = true;
					if (id == 74 && oreN) destroy = true;
					if (id == 129 && oreN) destroy = true;
					if (id == 153 && oreN) destroy = true;
					if (destroy) {
						gm->destroyBlock(&blockPos, 0);
						return;
					}
				}
			}
		}
	}
	if (treeNuker) { // Place saplings on dirt
		vec3_t* pos = gm->player->getPos();
		for (int x = (int)pos->x - range; x < pos->x + range; x++) {
			for (int z = (int)pos->z - range; z < pos->z + range; z++) {
				for (int y = (int)pos->y - range; y < pos->y + range; y++) {
					vec3_ti blockPos = vec3_ti(x, y + 1, z);
					bool build = false;
					int id = gm->player->region->getBlock(blockPos)->toLegacy()->blockId;
					if (id == 3 && treeN) build = true;  // Dirt
					if (build) {
						gm->buildBlock(&blockPos, 0);
						return;
					}
				}
			}
		}
	}
}

void Survival::onMove(C_MoveInputHandler* input) {
}

void Survival::onSendPacket(C_Packet* packet) {
	if (nofall) {
		auto player = g_Data.getLocalPlayer();
		if (player == nullptr) return;

		if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
			C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			movePacket->onGround = true;
		}
		if (packet->isInstanceOf<C_MovePlayerPacket>()) {
			C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			movePacket->onGround = true;
		}
	}
}

void Survival::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (treeNuker) {
	}
}

void Survival::onDisable() {
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
}