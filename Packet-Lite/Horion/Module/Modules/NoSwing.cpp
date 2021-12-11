#include "NoSwing.h"

NoSwing::NoSwing() : IModule(0, Category::MOVEMENT, "Disable arm swing animation for killaura, triggerbot, fucker etc.") {
}

NoSwing::~NoSwing() {
}

const char* NoSwing::getModuleName() {
	return ("NoSwing");
}