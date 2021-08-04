#include "Reach.h"

#include <Windows.h>
#include "../../../Utils/Logger.h"
#include "../../../Utils/Utils.h"

Reach::Reach() : IModule(0, Category::COMBAT, "Increases your reach") {
	this->registerFloatSetting("Amount", &this->reachValue, this->reachValue, 3.f, 8.f);
}

Reach::~Reach() {
}

const char* Reach::getModuleName() {
	return ("Reach");
}

void Reach::onTick(C_GameMode* gm) {
	if (reachPtr != 0) {
		*reachPtr = reachValue;
	}
}

void Reach::onEnable() {
	static uintptr_t sigOffset = 0x0;
	if (sigOffset == 0x0) {

		sigOffset = FindSignature("0F 10 05 ?? ?? ?? ?? 48 8B 4B 20 F3 0F 11 44 24");

		if (sigOffset != 0x0) {
			int offset = *reinterpret_cast<int*>((sigOffset + 3));  // Get Offset from code
			reachPtr = reinterpret_cast<float*>(sigOffset + offset + 7);
			originalReach = *reachPtr;
		}
	}
	if (!VirtualProtect(reachPtr, sizeof(float), PAGE_EXECUTE_READWRITE, &oldProtect)) {
#ifdef _DEBUG
		logF("couldnt unprotect memory send help");
		__debugbreak();
#endif
	}
}

void Reach::onDisable() {
	*reachPtr = originalReach;
	if (reachPtr != 0)
		VirtualProtect(reachPtr, sizeof(float), oldProtect, &oldProtect);
}