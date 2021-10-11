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
	if (g_Data.canUseMoveKeys()) {
		if (breakBlocks && !g_Data.isLeftClickDown())
			return;
		PointingStruct* pointing = g_Data.getClientInstance()->getPointerStruct();
		gm->destroyBlock(&pointing->block, pointing->blockSide);
	}
}