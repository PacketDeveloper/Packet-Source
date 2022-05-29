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
	if (g_Data.isRightClickDown() && g_Data.canUseMoveKeys()) {
		Odelay++;
		if (Odelay >= delay) {
			PointingStruct* pstruct = g_Data.getClientInstance()->getPointerStruct();
			g_Data.rightclickCount++;
			gm->buildBlock(new vec3_ti(pstruct->block), pstruct->blockSide);
		}
		Odelay = 0;
	}
}