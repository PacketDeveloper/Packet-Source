#include "Velocity.h"

#include "../../Module/ModuleManager.h"

Velocity::Velocity() : IModule(0, Category::MOVEMENT, "AntiKB Module") {
	registerFloatSetting("Linear", &xModifier, xModifier, 0.f, 1.f);
	registerFloatSetting("Height", &yModifier, yModifier, 0.f, 1.f);
}

Velocity::~Velocity() {
}

const char* Velocity::getModuleName() {
	return ("Velocity");
}

void Velocity::onTick(C_GameMode* gm) {
}