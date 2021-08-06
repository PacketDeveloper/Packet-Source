#include "Velocity.h"

#include "../../Module/ModuleManager.h"

Velocity::Velocity() : IModule(0, Category::MOVEMENT, "AntiKB Module") {
	this->registerFloatSetting("Linear", &this->xModifier, this->xModifier, 0.f, 1.f);
	this->registerFloatSetting("Height", &this->yModifier, this->yModifier, 0.f, 1.f);
}

Velocity::~Velocity() {
}

const char* Velocity::getModuleName() {
	return ("Velocity");
}

void Velocity::onTick(C_GameMode* gm) {
}