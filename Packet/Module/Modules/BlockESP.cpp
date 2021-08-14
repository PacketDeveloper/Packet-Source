#include "../../DrawUtils.h"
#include "BlockESP.h"

BlockESP::BlockESP() : IModule(0, Category::VISUAL, "Destroys Beds, Eggs, Treasures Etc.") {
	registerBoolSetting("Diamond", &this->dOre, this->dOre);
	registerBoolSetting("Emerald", &this->eOre, this->eOre);
	registerBoolSetting("Redstone", &this->rOre, this->rOre);
	registerBoolSetting("Gold", &this->gOre, this->gOre);
	registerBoolSetting("Iron", &iOre, iOre);
	//registerBoolSetting("", &this->eOre, this->eOre);
	registerIntSetting("Range", &this->range, this->range, 1, 10);
	registerFloatSetting("Width", &this->width, this->width, 0.1, 2);
}

BlockESP::~BlockESP() {
}

const char* BlockESP::getModuleName() {
	return ("BlockESP");
}

void BlockESP::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.canUseMoveKeys() && g_Data.getLocalPlayer() != nullptr) {
		auto player = g_Data.getLocalPlayer();
		vec3_t* pos = player->getPos();
		for (int x = (int)pos->x - range; x < pos->x + range; x++) {
			for (int z = (int)pos->z - range; z < pos->z + range; z++) {
				for (int y = (int)pos->y - range; y < pos->y + range; y++) {
					vec3_t blockPos = vec3_t(x, y, z);
					int id = (int)player->region->getBlock(blockPos)->toLegacy()->blockId;
					bool dRender = false;
					bool eRender = false;
					bool rRender = false;
					bool gRender = false;
					bool iRender = false;

					if (id == 56 && dOre) dRender = true;   // Diamond
					if (id == 129 && eOre) eRender = true;  // Emerald
					if (id == 73 && rOre) rRender = true;   // Redstone
					if (id == 74 && rOre) rRender = true;   // Redstone
					if (id == 14 && gOre) gRender = true;   // Gold
					if (id == 15 && iOre) iRender = true;   // Iron

					if (dRender) {
						DrawUtils::setColor(0.2, 0.9, 1, 1);
						DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), width);
					} else if (eRender) {
						DrawUtils::setColor(0, 0.7, 0, 1);
						DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), width);
					} else if (rRender) {
						DrawUtils::setColor(0.8, 0, 0, 1);
						DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), width);
					} else if (gRender) {
						DrawUtils::setColor(1, 0.7, 0, 1);
						DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), width);
					} else if (iRender) {
						DrawUtils::setColor(1, 0.9, 0.8, 1);
						DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), width);
					}
				}
			}
		}
	}
}