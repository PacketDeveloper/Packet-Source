#include "Fucker.h"

Fucker::Fucker() : IModule(0, Category::MISC, "Destroys Beds, Eggs, Treasures Etc. A litte buggy on Mineplex.") {
	registerBoolSetting("Treasures", &this->treasures, this->treasures);
	registerBoolSetting("Redstone", &this->rOre, this->rOre);
	//registerBoolSetting("Diamond", &this->dOre, this->dOre);
	//registerBoolSetting("Emerald", &this->eOre, this->eOre);
	//registerBoolSetting("Chests", &this->chests, this->chests);
	registerBoolSetting("Cakes", &this->cakes, this->cakes);
	registerBoolSetting("Beds", &this->beds, this->beds);
	registerBoolSetting("Eggs", &this->eggs, this->eggs);
	registerBoolSetting("---------", &this->air, this->air);
	registerBoolSetting("Nuker", &this->nuker, this->nuker);
	registerBoolSetting("Ores", &this->ores, this->ores);
	/*registerBoolSetting("Cobblestone", &this->cobble, this->cobble);
	registerBoolSetting("Leaves", &this->leaves, this->leaves);
	registerBoolSetting("Sand", &this->sand, this->sand);
	registerBoolSetting("grass", &this->grass, this->grass);
	registerBoolSetting("stone", &this->stone, this->stone);
	registerBoolSetting("Iron", &this->iron, this->iron);
	registerBoolSetting("Gold", &this->gold, this->gold);
	registerBoolSetting("Dirt", &this->dirt, this->dirt);*/
	registerIntSetting("Range", &this->range, this->range, 1, 10);
	//registerBoolSetting("Barrels", &this->barrels, this->barrels);
}

Fucker::~Fucker() {
}

const char* Fucker::getModuleName() {
	return ("Breaker");
}

void Fucker::onTick(C_GameMode* gm) {
	if (strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "geo.hivebedrock.network") == 0) {  // Only on The Hive
		//*g_Data.getClientInstance()->minecraft->timer = 5.f;
	}
	vec3_t* pos = gm->player->getPos();
	for (int x = (int)pos->x - range; x < pos->x + range; x++) {
		for (int z = (int)pos->z - range; z < pos->z + range; z++) {
			for (int y = (int)pos->y - range; y < pos->y + range; y++) {
				vec3_ti blockPos = vec3_ti(x, y, z);
				bool destroy = false;
				int id = gm->player->region->getBlock(blockPos)->toLegacy()->blockId;

				// Normal Settings
				if (id == 26 && this->beds) destroy = true;      // Beds
				if (id == 122 && this->eggs) destroy = true;     // Dragon Eggs
				if (id == 92 && this->cakes) destroy = true;     // Cakes
				if (id == 73 && this->rOre) destroy = true;      // Redstone Ore
				if (id == 54 && this->chests) destroy = true;    // Chests

				// Nuker
				if (id == 1 && this->nuker) destroy = true;  // Stone
				if (id == 2 && this->nuker) destroy = true;  // Grass
				if (id == 3 && this->nuker) destroy = true;  // Dirt
				//if (id == 24 && this->nuker) destroy = true;   // Sandstone
				//if (id == 12 && this->nuker) destroy = true;   // Sand
				if (id == 4 && this->nuker) destroy = true;  // Cobblestone
				//if (id == 17 && this->nuker) destroy = true;   // Logs
				//if (id == 18 && this->nuker) destroy = true;   // Leaves
				if (id == 5 && this->nuker) destroy = true;    // Planks
				if (id == 98 && this->nuker) destroy = true;   // Stone Brick
				if (id == 97 && this->nuker) destroy = true;   // Stone Brick
				if (id == 159 && this->nuker) destroy = true;  // Hardend Clay
				//if (id == 44 && this->nuker) destroy = true;   // Slabs
				if (id == 158 && this->nuker) destroy = true;  // Slabs
				if (id == 155 && this->nuker) destroy = true;  // Quartz
				if (id == 156 && this->nuker) destroy = true;  // Quartz
				if (id == 134 && this->nuker) destroy = true;  // Stairs
				if (id == 135 && this->nuker) destroy = true;  // Stairs
				if (id == 136 && this->nuker) destroy = true;  // Stairs
				if (id == 139 && this->nuker) destroy = true;  // Stairs
				//if (id == 53 && this->nuker) destroy = true;   // Oak Stairs
				if (id == 163 && this->nuker) destroy = true;  // Stairs
				if (id == 164 && this->nuker) destroy = true;  // Stairs
				if (id == 160 && this->nuker) destroy = true;  // Colored Glass Panes
				if (id == 102 && this->nuker) destroy = true;  // Glass panes
				//if (id == 20 && this->nuker) destroy = true;   // Glass
				if (id == 8 && this->nuker) destroy = true;  // Water
				if (id == 9 && this->nuker) destroy = true;  // Water
				if (id == 6 && this->nuker) destroy = true;  // Water
				if (id == 7 && this->nuker) destroy = true;  // Water
				//if (id == 8 && this->nuker) destroy = true;    // Water
				//if (id == 9 && this->nuker) destroy = true;    // Water
				if (id == 10 && this->nuker) destroy = true;   // Water
				if (id == 11 && this->nuker) destroy = true;   // Water
				if (id == 12 && this->nuker) destroy = true;   // Water
				if (id == 13 & this->nuker) destroy = true;    // Water
				if (id == 14 && this->nuker) destroy = true;   // Water
				if (id == 15 && this->nuker) destroy = true;   // Water
				if (id == 16 && this->nuker) destroy = true;   // Water
				if (id == 17 && this->nuker) destroy = true;   // Water
				if (id == 18 && this->nuker) destroy = true;   // Water
				if (id == 19 && this->nuker) destroy = true;   // Water
				if (id == 20 && this->nuker) destroy = true;   // Water
				if (id == 21 && this->nuker) destroy = true;   // Water
				if (id == 22 && this->nuker) destroy = true;   // Water
				if (id == 23 && this->nuker) destroy = true;   // Water
				if (id == 24 && this->nuker) destroy = true;   // Water
				if (id == 25 && this->nuker) destroy = true;   // Water
				if (id == 26 && this->nuker) destroy = true;   // Water
				if (id == 27 && this->nuker) destroy = true;   // Water
				if (id == 28 && this->nuker) destroy = true;   // Water
				if (id == 29 && this->nuker) destroy = true;   // Water
				if (id == 30 && this->nuker) destroy = true;   // Water
				if (id == 31 && this->nuker) destroy = true;   // Water
				if (id == 32 && this->nuker) destroy = true;   // Water
				if (id == 33 && this->nuker) destroy = true;   // Water
				if (id == 34 && this->nuker) destroy = true;   // Water
				if (id == 35 && this->nuker) destroy = true;   // Water
				if (id == 36 && this->nuker) destroy = true;   // Water
				if (id == 37 && this->nuker) destroy = true;   // Water
				if (id == 38 && this->nuker) destroy = true;   // Water
				if (id == 39 && this->nuker) destroy = true;   // Water
				if (id == 40 && this->nuker) destroy = true;   // Water
				if (id == 41 && this->nuker) destroy = true;   // Water
				if (id == 42 && this->nuker) destroy = true;   // Water
				if (id == 43 && this->nuker) destroy = true;   // Water
				if (id == 44 && this->nuker) destroy = true;   // Water
				if (id == 45 && this->nuker) destroy = true;   // Water
				if (id == 46 && this->nuker) destroy = true;   // Water
				if (id == 47 && this->nuker) destroy = true;   // Water
				if (id == 48 && this->nuker) destroy = true;   // Water
				if (id == 49 && this->nuker) destroy = true;   // Water
				if (id == 50 && this->nuker) destroy = true;   // Water
				if (id == 51 && this->nuker) destroy = true;   // Water
				if (id == 52 && this->nuker) destroy = true;   // Water
				if (id == 53 && this->nuker) destroy = true;   // Water
				if (id == 54 && this->nuker) destroy = true;   // Water
				if (id == 55 && this->nuker) destroy = true;   // Water
				if (id == 56 && this->nuker) destroy = true;   // Water
				if (id == 57 && this->nuker) destroy = true;   // Water
				if (id == 58 && this->nuker) destroy = true;   // Water
				if (id == 59 && this->nuker) destroy = true;   // Water
				if (id == 60 && this->nuker) destroy = true;   // Water
				if (id == 61 && this->nuker) destroy = true;   // Water
				if (id == 62 && this->nuker) destroy = true;   // Water
				if (id == 63 && this->nuker) destroy = true;   // Water
				if (id == 63 && this->nuker) destroy = true;   // Water
				if (id == 64 && this->nuker) destroy = true;   // Water
				if (id == 65 && this->nuker) destroy = true;   // Water
				if (id == 66 && this->nuker) destroy = true;   // Water
				if (id == 67 && this->nuker) destroy = true;   // Water
				if (id == 68 && this->nuker) destroy = true;   // Water
				if (id == 69 && this->nuker) destroy = true;   // Water
				if (id == 70 && this->nuker) destroy = true;   // Water
				if (id == 71 && this->nuker) destroy = true;   // Water
				if (id == 72 && this->nuker) destroy = true;   // Water
				if (id == 73 && this->nuker) destroy = true;   // Water
				if (id == 74 && this->nuker) destroy = true;   // Water
				if (id == 75 && this->nuker) destroy = true;   // Water
				if (id == 76 && this->nuker) destroy = true;   // Water
				if (id == 77 && this->nuker) destroy = true;   // Water
				if (id == 78 && this->nuker) destroy = true;   // Water
				if (id == 79 && this->nuker) destroy = true;   // Water
				if (id == 80 && this->nuker) destroy = true;   // Water
				if (id == 81 && this->nuker) destroy = true;   // Water
				if (id == 82 && this->nuker) destroy = true;   // Water
				if (id == 83 && this->nuker) destroy = true;   // Water
				if (id == 84 && this->nuker) destroy = true;   // Water
				if (id == 85 && this->nuker) destroy = true;   // Water
				if (id == 86 && this->nuker) destroy = true;   // Water
				if (id == 87 && this->nuker) destroy = true;   // Water
				if (id == 88 && this->nuker) destroy = true;   // Water
				if (id == 89 && this->nuker) destroy = true;   // Water
				if (id == 90 && this->nuker) destroy = true;   // Water
				if (id == 91 && this->nuker) destroy = true;   // Water
				if (id == 92 && this->nuker) destroy = true;   // Water
				if (id == 93 && this->nuker) destroy = true;   // Water
				if (id == 94 && this->nuker) destroy = true;   // Water
				if (id == 95 && this->nuker) destroy = true;   // Water
				if (id == 96 && this->nuker) destroy = true;   // Water
				if (id == 97 && this->nuker) destroy = true;   // Water
				if (id == 98 && this->nuker) destroy = true;   // Water
				if (id == 99 && this->nuker) destroy = true;   // Water
				if (id == 100 && this->nuker) destroy = true;  // Water
				if (id == 101 && this->nuker) destroy = true;  // Water
				if (id == 102 && this->nuker) destroy = true;  // Water
				if (id == 103 && this->nuker) destroy = true;  // Water
				if (id == 245 && this->nuker) destroy = true;  // Water
				if (id == 286 && this->nuker) destroy = true;  // Water
				if (id == 150 && this->nuker) destroy = true;  // Water
				if (id == 187 && this->nuker) destroy = true;  // Water
				if (id == 196 && this->nuker) destroy = true;  // Water
				if (id == 201 && this->nuker) destroy = true;  // Water
				if (id == 140 && this->nuker) destroy = true;  // Water
				if (id == 130 && this->nuker) destroy = true;  // Water
				if (id == 120 && this->nuker) destroy = true;  // Water
				if (id == 110 && this->nuker) destroy = true;  // Water
				if (id == 150 && this->nuker) destroy = true;  // Water
				if (id == 160 && this->nuker) destroy = true;  // Water
				if (id == 170 && this->nuker) destroy = true;  // Water
				if (id == 180 && this->nuker) destroy = true;  // Water
				if (id == 190 && this->nuker) destroy = true;  // Water
				if (id == 210 && this->nuker) destroy = true;  // Water
				if (id == 220 && this->nuker) destroy = true;  // Water
				if (id == 250 && this->nuker) destroy = true;  // Water
				if (id == 260 && this->nuker) destroy = true;  // Water
				if (id == 270 && this->nuker) destroy = true;  // Water
				if (id == 280 && this->nuker) destroy = true;  // Water
				if (id == 290 && this->nuker) destroy = true;  // Water
				if (id == 148 && this->nuker) destroy = true;  // Water
				if (id == 167 && this->nuker) destroy = true;  // Water
				if (id == 467 && this->nuker) destroy = true;  // Water
				if (id == 237 && this->nuker) destroy = true;  // Water
				if (id == 236 && this->nuker) destroy = true;  // Water
				if (id == 432 && this->nuker) destroy = true;  // Water

				// Ores
				if (id == 56 && this->ores) destroy = true;        // Diamond Ore
				if (id == 21 && this->ores) destroy = true;        // Lapis Ore
				if (id == 129 && this->ores) destroy = true;       // Emerald Ore
				if (id == 15 && this->ores) destroy = true;        // Iron Ore
				if (id == 14 && this->ores) destroy = true;        // Gold Ore

				if (destroy) {
					gm->destroyBlock(&blockPos, 0);
					if (!moduleMgr->getModule<NoSwing>()->isEnabled()) 
						g_Data.getLocalPlayer()->swingArm();
					return;
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

void Fucker::onDisable() {
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
}