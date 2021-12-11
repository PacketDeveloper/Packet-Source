#include "Teleport.h"

Teleport::Teleport() : IModule(0, Category::MOVEMENT, "Gives you a big block reach.") {
	//registerBoolSetting("Only Hand", &this->onlyHand, this->onlyHand);
	//registerBoolSetting("Test", &this->bypass, this->bypass); vec3_ti block = g_Data.getUuidInstance()->getPointerStruct()->uuid;Reset
}

Teleport::~Teleport() {
}

const char* Teleport::getModuleName() {
	return "InfiniteBlockReach";
}

void Teleport::onTick(C_GameMode* gm) {

}
