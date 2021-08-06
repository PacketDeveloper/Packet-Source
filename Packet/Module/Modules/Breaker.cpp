#include "Breaker.h"

Breaker::Breaker() : IModule(0, Category::MISC, "Destroys Beds, Eggs, Treasures Etc.") {
	registerBoolSetting("Treasures", &treasures, treasures);
	registerBoolSetting("Redstone", &rOre, rOre);
	registerBoolSetting("Diamond", &dOre, dOre);
	registerBoolSetting("Emerald", &eOre, eOre);
	//registerBoolSetting("Carrots", &carrots, carrots);//? - kow
	registerBoolSetting("Chests", &chests, chests);
	registerBoolSetting("Cakes", &cakes, cakes);
	registerBoolSetting("Beds", &beds, beds);
	registerBoolSetting("Eggs", &eggs, eggs);
	registerIntSetting("Range", &range, range, 1, 10);
	registerIntSetting("delay", &delay, delay, 0, 5);
}

Breaker::~Breaker() {
}

const char* Breaker::getModuleName() {
	return ("Breaker");
}

void Breaker::onTick(C_GameMode* gm) {
	vec3_t* pos = gm->player->getPos();
	for (int x = (int)pos->x - range; x < pos->x + range; x++) {
		for (int z = (int)pos->z - range; z < pos->z + range; z++) {
			for (int y = (int)pos->y - range; y < pos->y + range; y++) {
				vec3_ti blockPos = vec3_ti(x, y, z);
				bool destroy = false;
				int id = (int)gm->player->region->getBlock(blockPos)->toLegacy()->blockId;

				if (id == 26 && this->beds) destroy = true;      // Beds
				if (id == 122 && this->eggs) destroy = true;     // Dragon Eggs
				if (id == 92 && this->cakes) destroy = true;     // Cakes
				if (id == 73 && this->rOre) destroy = true;      // Redstone Ore
				if (id == 56 && this->dOre) destroy = true;      // Diamond Ore
				if (id == 129 && this->eOre) destroy = true;     // Emerald Ore
				if (id == 54 && this->chests) destroy = true;    // Chests
				if (id == 141 && this->carrots) destroy = true;  // Carrots

					Odelay++;
				if (destroy) {
						if (Odelay >= delay) {
							if (g_Data.canUseMoveKeys()) {
								gm->destroyBlock(&blockPos, 0);
								if (!moduleMgr->getModule<NoSwing>()->isEnabled())
									g_Data.getLocalPlayer()->swingArm();
								return;
							}
							Odelay = 0;
						}
				}
			}
		}

		if (this->treasures) {
			g_Data.forEachEntity([](C_Entity* ent, bool b) {
				std::string name = ent->getNameTag()->getText();
				int id = ent->getEntityTypeId();
				if (name.find("Treasure") != std::string::npos && g_Data.getLocalPlayer()->getPos()->dist(*ent->getPos()) <= 5) {
					g_Data.getCGameMode()->attack(ent);
					if (!moduleMgr->getModule<NoSwing>()->isEnabled())
						g_Data.getLocalPlayer()->swingArm();
				}
			});
		}
	}
}

void Breaker::onDisable() {
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
}