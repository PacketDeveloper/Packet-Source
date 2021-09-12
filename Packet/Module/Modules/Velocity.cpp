#include "Velocity.h"

#include "../../Module/ModuleManager.h"

Velocity::Velocity() : IModule(0, Category::MOVEMENT, "AntiKB Module") {
	this->registerFloatSetting("Linear", &this->xModifier, this->xModifier, 0.f, 1.f);
	this->registerFloatSetting("Height", &this->yModifier, this->yModifier, 0.f, 1.f);
}

Velocity::~Velocity() {
}

const char* Velocity::getRawModuleName() {
	return "Velocity";
}

const char* Velocity::getModuleName() {
	name = std::string("Velocity ") + std::string(GRAY) + std::to_string((int)xModifier) + std::string(".") + std::to_string((int)(xModifier * 10) - ((int)xModifier * 10));
	fullname = name + std::string(", ") + std::to_string((int)yModifier) + std::string(".") + std::to_string((int)(yModifier * 10) - ((int)yModifier * 10));
	return fullname.c_str();
}

void Velocity::onTick(C_GameMode* gm) {
}