#include "CameraMod.h"

CameraMod::CameraMod() : IModule(0, Category::VISUAL, "DeiDeeDeiDerrtDord") {
	this->registerBoolSetting("NoHurtcam", &this->nohurtMode, this->nohurtMode);
	this->registerBoolSetting("Fullbright", &this->fullbrightMode, this->fullbrightMode);
}

const char* CameraMod::getModuleName() {
	return "Camera";
}

float originalGamma = -1;

void CameraMod::onTick(C_GameMode* gm) {
	if (fullbrightMode) {
		if (gammaPtr != nullptr && *gammaPtr != 10)
			*gammaPtr = 10;
	}
}

void CameraMod::onEnable() {
	if (fullbrightMode) {
		if (gammaPtr != nullptr) {
			originalGamma = *gammaPtr;
			*gammaPtr = 10;
		}
	}
}

void CameraMod::onDisable() {
	if (fullbrightMode) {
		if (gammaPtr != nullptr) {
			if (originalGamma >= 0 && originalGamma <= 1)
				*gammaPtr = originalGamma;
			else
				*gammaPtr = 0.5f;
		}
	}
}