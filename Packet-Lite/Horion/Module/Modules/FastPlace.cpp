#include "FastPlace.h"

FastPlace ::FastPlace() : IModule(0, Category::PLAYER, "Places blocks very fast") {
	//this->registerBoolSetting("rightclick", &this->rightclick, rightclick);
	//this->registerBoolSetting("only weapons", &this->weapons, this->weapons);
	//this->registerBoolSetting("break blocks", &this->breakBlocks, this->breakBlocks);
	//this->registerIntSetting("delay", &this->delay, this->delay, 0, 20);
	//this->registerBoolSetting("hold", &this->hold, this->hold);
}

FastPlace ::~FastPlace() {
}

const char* FastPlace ::getModuleName() {
	return ("FastPlace");
}

void FastPlace ::onTick(C_GameMode* gm) {
	if (rightclick) {
		if ((GameData::isRightClickDown() || !hold) && GameData::canUseMoveKeys()) {
			PointingStruct* pstruct = g_Data.getClientInstance()->getPointerStruct();
			Odelay++;
			if (Odelay >= delay) {
				g_Data.rightclickCount++;
				gm->buildBlock(new vec3_ti(pstruct->block), pstruct->blockSide);
				Odelay = 0;
			}
		}
	}
}