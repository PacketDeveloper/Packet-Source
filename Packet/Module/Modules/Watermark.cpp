#include "Watermark.h"

Watermark::Watermark() : IModule(0, Category::VISUAL, "goa frea thiom dsisti - deq") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Packet", 0);
	mode.addEntry("Fadeaway", 1);
	mode.addEntry("Retard", 2);
	registerBoolSetting("FirstLetter", &firstLetter, &firstLetter);
	//registerBoolSetting("UsedLater", &test, &test);
	this->registerFloatSetting("Opacity", &this->opacity, this->opacity, 0.f, 1.f);
}

Watermark::~Watermark() {
}

const char* Watermark::getModuleName() {
	return ("Watermark");
}

void Watermark::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
}

void Watermark::onTick(C_GameMode* gm) {
}