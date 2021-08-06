#include "CustomSky.h"

CustomSky::CustomSky() : IModule(0, Category::VISUAL, "CustomSky") {
	registerBoolSetting("Rainbow", &this->rainbow, this->rainbow);
	registerFloatSetting("Red", &this->red, this->red, 0.f, 1.f);
	registerFloatSetting("Green", &this->green, this->green, 0.f, 1.f);
	registerFloatSetting("Blue", &this->blue, this->blue, 0.f, 1.f);
}

const char* CustomSky::getModuleName() {
	return "CustomSky";
}