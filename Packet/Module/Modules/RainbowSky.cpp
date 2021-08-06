#include "RainbowSky.h"

RainbowSky::RainbowSky() : IModule(0, Category::VISUAL, "Gaybow sky!") {
}

RainbowSky::~RainbowSky() {
}

const char* RainbowSky::getModuleName() {
	return ("RainbowSky");
}
