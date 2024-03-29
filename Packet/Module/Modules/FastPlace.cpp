#include "FastPlace.h"

FastPlace::FastPlace() : IModule(0, Category::PLAYER, "Places blocks fast") {
	registerIntSetting("delay", &delay, delay, 0, 10);
}

FastPlace::~FastPlace() {
}

const char* FastPlace::getModuleName() {
	return ("FastPlace");
}

void FastPlace::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	PointingStruct* pointing = g_Data.getClientInstance()->getPointerStruct();
	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(pointing->block);
	int blockID = (int)block->toLegacy()->blockId;
	if (blockID == 0)
		return;

	if (g_Data.isRightClickDown() && g_Data.canUseMoveKeys()) {
		Odelay++;
		if (Odelay >= delay) {
			PointingStruct* pstruct = g_Data.getClientInstance()->getPointerStruct();
			g_Data.rightclickCount++;
			if (blockID != 0) gm->buildBlock(new vec3_ti(pointing->block), pointing->blockSide);
		}
		Odelay = 0;
	}
}