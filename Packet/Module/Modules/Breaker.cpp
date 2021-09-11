#include "Breaker.h"

Breaker::Breaker() : IModule(0, Category::MISC, "Destroys Beds, Eggs, Treasures Etc.") {
	registerBoolSetting("Treasures", &treasures, treasures);
	registerBoolSetting("Redstone", &rOre, rOre);
	registerBoolSetting("Diamond", &dOre, dOre);
	registerBoolSetting("Emerald", &eOre, eOre);
	registerBoolSetting("Gold", &gOre, gOre);
	registerBoolSetting("Chests", &chests, chests);
	registerBoolSetting("Cakes", &cakes, cakes);
	registerBoolSetting("Beds", &beds, beds);
	registerBoolSetting("Eggs", &eggs, eggs);
	//registerBoolSetting("Switch", &switchSlot, switchSlot);
	registerIntSetting("Range", &range, range, 1, 10);
	//registerIntSetting("Slot", &slot, slot, 1, 9);
}

Breaker::~Breaker() {
}

const char* Breaker::getModuleName() {
	//name = std::string("Breaker ") + std::string(GRAY) + std::to_string((int)range);
	//return name.c_str();
	return ("Breaker");
}

void Breaker::onTick(C_GameMode* gm) {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	vec3_t* pos = gm->player->getPos();
	for (int x = (int)pos->x - range; x < pos->x + range; x++) {
		for (int z = (int)pos->z - range; z < pos->z + range; z++) {
			for (int y = (int)pos->y - range; y < pos->y + range; y++) {
				vec3_ti blockPos = vec3_ti(x, y, z);
				bool destroy = false;
				bool eat = false;
				int id = (int)gm->player->region->getBlock(blockPos)->toLegacy()->blockId;

				if (id == 26 && beds) destroy = true;      // Beds
				if (id == 122 && eggs) destroy = true;     // Dragon Eggs
				if (id == 92 && cakes) eat = true;        // Cakes
				if (id == 73 && rOre) destroy = true;      // Redstone Ore
				if (id == 56 && dOre) destroy = true;      // Diamond Ore
				if (id == 129 && eOre) destroy = true;     // Emerald Ore
				if (id == 14 && gOre) destroy = true;     // Gold Ore
				if (id == 54 && chests) destroy = true;    // Chests

				if (destroy) {
					if (g_Data.canUseMoveKeys()) {
						gm->destroyBlock(&blockPos, 0);
						if (!moduleMgr->getModule<NoSwing>()->isEnabled())
							g_Data.getLocalPlayer()->swingArm();
						return;
					}
					if (switchSlot) {
						supplies->selectedHotbarSlot = slot;
						destroy = false;
					}
				}
				if (eat) {
					if (g_Data.canUseMoveKeys()) {
						gm->buildBlock(&blockPos, 0);
						if (!moduleMgr->getModule<NoSwing>()->isEnabled())
							g_Data.getLocalPlayer()->swingArm();
						return;
					}
					if (switchSlot) {
						supplies->selectedHotbarSlot = slot;
						eat = false;
					}
				}
			}
		}

		if (treasures) {
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