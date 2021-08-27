#include "BlockOutline.h"

BlockOutline::BlockOutline() : IModule(0, Category::VISUAL, "read the name") {
	registerBoolSetting("Rainbow", &rainbow, rainbow);
	registerFloatSetting("Red", &red, red, 0, 1);
	registerFloatSetting("Green", &green, green, 0, 1);
	registerFloatSetting("Blue", &blue, blue, 0, 1);
	registerFloatSetting("Width", &width, width, 0.3, 1);
}

BlockOutline::~BlockOutline() {
}

const char* BlockOutline::getModuleName() {
	return ("BlockOutline");
}

void BlockOutline::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	auto player = g_Data.getLocalPlayer();
	if (g_Data.canUseMoveKeys() && g_Data.getLocalPlayer() != nullptr && this->isEnabled()) {
		PointingStruct* pointing = g_Data.getClientInstance()->getPointerStruct();
		vec3_t block = pointing->block.toVec3t();
		block = block.floor();
		vec3_t cock = pointing->block.toVec3t();
		cock = cock.floor();
		cock.x += 1.f;
		cock.y += 1.f;
		cock.z += 1.f;
		if (!rainbow) {
			DrawUtils::setColor(red, green, blue, 1);
		}
		DrawUtils::drawBox(block, cock, width, 1);
	}
}