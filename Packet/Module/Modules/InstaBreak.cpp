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
	if ((GameData::isLeftClickDown() || !hold) && GameData::canUseMoveKeys()) {

		PointingStruct* pointing = g_Data.getClientInstance()->getPointerStruct();

		if (breakBlocks) {
			gm->destroyBlock(&pointing->block, pointing->blockSide);
		}
	}
}
