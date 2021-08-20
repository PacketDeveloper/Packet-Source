#include "Nuker.h"

Nuker::Nuker() : IModule(0, Category::MISC, "Break multiple blocks at once") {
	this->registerBoolSetting("auto destroy", &this->autodestroy, this->autodestroy);

	this->registerBoolSetting("veinminer", &this->veinMiner, this->veinMiner);
	this->registerIntSetting("radius", &this->nukerRadius, this->nukerRadius, 1, 15);
	this->registerIntSetting("Upo", &this->up, this->up, 1, 15);
	this->registerIntSetting("Downo", &this->down, this->down, 1, 15);
}

Nuker::~Nuker() {
}

const char* Nuker::getModuleName() {
	return ("Nuker");
}

void Nuker::onTick(C_GameMode* gm) {
	if (!autodestroy) return;
	vec3_t* pos = gm->player->getPos();
	for (int x = (int)pos->x - nukerRadius; x < pos->x + nukerRadius; x++) {
		for (int z = (int)pos->z - nukerRadius; z < pos->z + nukerRadius; z++) {
			for (int y = (int)pos->y - down; y < pos->y + up; y++) {
				vec3_ti blockPos = vec3_ti(x, y, z);
				bool destroy = true;
				int id = gm->player->region->getBlock(blockPos)->toLegacy()->blockId;

				if (destroy) {
					gm->destroyBlock(&blockPos, 1);
				}
			}
		}
	}
}