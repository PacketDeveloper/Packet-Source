#include "Nuker.h"

Nuker::Nuker() : IModule(0, Category::MISC, "Break multiple blocks at once") {
	this->registerBoolSetting("AutoDestroy", &this->autodestroy, this->autodestroy);
	this->registerBoolSetting("VeinMiner", &this->veinMiner, this->veinMiner);
	this->registerBoolSetting("Treeos", &this->tree, this->tree);
	this->registerBoolSetting("Oreos", &this->ore, this->ore);
	this->registerIntSetting("Radius", &this->nukerRadius, this->nukerRadius, 1, 15);
	this->registerIntSetting("Upo", &this->up, this->up, 1, 15);
	this->registerIntSetting("Downo", &this->down, this->down, 1, 15);
}

Nuker::~Nuker() {
}

const char* Nuker::getModuleName() {
	return ("Nuker");
}

void Nuker::onTick(C_GameMode* gm) {
	if (autodestroy) {
		vec3_t* pos = gm->player->getPos();
		for (int x = (int)pos->x - nukerRadius; x < pos->x + nukerRadius; x++) {
			for (int z = (int)pos->z - nukerRadius; z < pos->z + nukerRadius; z++) {
				for (int y = (int)pos->y - down; y < pos->y + up; y++) {
					vec3_ti blockPos = vec3_ti(x, y, z);
					gm->destroyBlock(&blockPos, 1);
				}
			}
		}
	} else if (tree && !autodestroy) {
		vec3_t* pos = gm->player->getPos();
		for (int x = (int)pos->x - nukerRadius; x < pos->x + nukerRadius; x++) {
			for (int z = (int)pos->z - nukerRadius; z < pos->z + nukerRadius; z++) {
				for (int y = (int)pos->y - down; y < pos->y + up; y++) {
					vec3_ti blockPos = vec3_ti(x, y, z);
					bool destroy = false;
					int id = gm->player->region->getBlock(blockPos)->toLegacy()->blockId;

					if (id == 17 || id == 18 && tree) destroy = true;

					if (destroy) {
						gm->destroyBlock(&blockPos, 1);
					}
				}
			}
		}
	}
	if (ore && !autodestroy) {
			vec3_t* pos = gm->player->getPos();
			for (int x = (int)pos->x - nukerRadius; x < pos->x + nukerRadius; x++) {
				for (int z = (int)pos->z - nukerRadius; z < pos->z + nukerRadius; z++) {
					for (int y = (int)pos->y - down; y < pos->y + up; y++) {
						vec3_ti blockPos = vec3_ti(x, y, z);
						bool destroy = false;
						int id = gm->player->region->getBlock(blockPos)->toLegacy()->blockId;

						if (id == 129 || id == 56 && ore) destroy = true;

						if (destroy) {
							gm->destroyBlock(&blockPos, 1);
						}
					}
				}
			}
	}
}