#include "Watermark.h"

Watermark::Watermark() : IModule(0, Category::VISUAL, "goa frea thiom dsisti - deq") {
	registerBoolSetting("FirstLetter", &firstLetter, &firstLetter);
	registerEnumSetting("Theme", &mode, 0);
	mode.addEntry("Packet", 0);
	mode.addEntry("New", 1);
	//mode.addEntry("Fadeaway", 2);
	//mode.addEntry("Test", 3);
	registerFloatSetting("Opacity", &opacity, opacity, 0.f, 1.f);
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