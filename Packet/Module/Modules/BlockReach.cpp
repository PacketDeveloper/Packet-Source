#include "BlockReach.h"

BlockReach::BlockReach() : IModule(0, Category::PLAYER, "Longer reach, but for blocks!") {
}

BlockReach::~BlockReach() {
}

const char* BlockReach::getModuleName() {
	return "BlockReach";
}

void BlockReach::onTick(C_GameMode* gm) {
}