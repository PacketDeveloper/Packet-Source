#include "InstaBreak.h"

InstaBreak::InstaBreak() : IModule(0, Category::EXPLOIT, "Break any block instantly") {
	registerBoolSetting("Hold", &breakBlocks, breakBlocks);
}

InstaBreak::~InstaBreak() {
}

const char* InstaBreak::getModuleName() {
	return ("InstaBreak");
}

void InstaBreak::onTick(C_GameMode* gm) {
	bool hold = true;
<<<<<<< HEAD
	if (g_Data.canUseMoveKeys() && breakBlocks) {
		if (hold && !g_Data.isLeftClickDown())
			return;
		PointingStruct* pointing = g_Data.getClientInstance()->getPointerStruct();
		gm->destroyBlock(&pointing->block, pointing->blockSide);
	}
}
=======
	if ((GameData::isLeftClickDown() || !hold) && GameData::canUseMoveKeys()) {

		PointingStruct* pointing = g_Data.getClientInstance()->getPointerStruct();

		if (breakBlocks) {
			gm->destroyBlock(&pointing->block, pointing->blockSide);
		}
	}
}
>>>>>>> 2d6938d96a0dbefd445141e7f1edfc47b99dfcf0
