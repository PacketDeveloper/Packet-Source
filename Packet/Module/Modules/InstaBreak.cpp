#include "InstaBreak.h"

InstaBreak::InstaBreak() : IModule(0, Category::EXPLOIT, "Break any block instantly") {
	registerBoolSetting("Hold", &breakBlocks, breakBlocks); 
	//registerBoolSetting("Hive", &hive, hive); 
}

InstaBreak::~InstaBreak() {
}

const char* InstaBreak::getModuleName() {
	return ("InstaBreak");
}

void InstaBreak::onEnable() {
	i = 0;
}

void InstaBreak::onTick(C_GameMode* gm) {
	if (g_Data.canUseMoveKeys()) {
		if (hive) {

		} else if (breakBlocks) {
			if (!g_Data.isLeftClickDown())
				return;
			PointingStruct* pointing = g_Data.getClientInstance()->getPointerStruct();
			bool isDestroyed = false;
			gm->startDestroyBlock(pointing->block, 1, isDestroyed);
			gm->destroyBlock(&pointing->block, pointing->blockSide);
		}
	}
}