#include "HudModule.h"

#include "../../DrawUtils.h"
#include "../../Scripting/ScriptManager.h"

HudModule::HudModule() : IModule(0, Category::VISUAL, "Displays ur mom") {
	registerEnumSetting("Color", &color, 0);
	color.addEntry("Rainbow", 0);
	color.addEntry("Dynamic", 1);
	color.addEntry("Horion", 2);
	color.addEntry("Fadeaway", 3);
	color.addEntry("White", 4);
	color.addEntry("Red", 5);
	color.addEntry("Orange", 6);
	color.addEntry("Yellow", 7);
	color.addEntry("Green", 8);
	color.addEntry("Blue", 9);
	color.addEntry("Purple", 11);
	color.addEntry("Pink", 12);
	registerBoolSetting("FPS", &fps, fps);
	registerBoolSetting("Coords", &coordinates, coordinates);
	registerBoolSetting("ArmorHUD", &displayArmor, displayArmor);
	//registerBoolSetting("Keystrokes", &keystrokes, keystrokes);
	registerFloatSetting("Opacity", &opacity, opacity, 0.f, 1.f);
	//registerFloatSetting("Scale", &scale, scale, 0.5f, 1.5f);
	//registerBoolSetting("BPS", &bps, bps);
	//registerBoolSetting("Always show", &alwaysShow, alwaysShow);
}

HudModule::~HudModule() {
}

const char* HudModule::getModuleName() {
	return ("HUD");
}

void HudModule::onTick(C_GameMode* gm) {
}

void HudModule::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
}

void HudModule::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	float f = 10.f * scale;
	std::string tempStr("Movement");
	float len = DrawUtils::getTextWidth(&tempStr, scale) + 7.f;
	float startY = tabgui ? 6 * f : 0.f;
	if (tabgui && scriptMgr.getNumEnabledScripts() > 0)
		startY += f;

	{  // CPS
		if (!(g_Data.getLocalPlayer() == nullptr || !cps)) {
			std::string cpsText = "CPS: " + std::to_string(g_Data.getLeftCPS());
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 15.f * scale);
			vec2_t textPos = vec2_t(rectPos.x * cpspos2, rectPos.y * cpspos);  //CPS Location
			DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), 0.f);
			DrawUtils::drawText(textPos, &cpsText, MC_Color(255, 255, 255), scale);
		}
	}
	{  // Keystrokes
		if (!(g_Data.getLocalPlayer() == nullptr || !keystrokes || !GameData::canUseMoveKeys())) {
			C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
			DrawUtils::drawKeystroke(*input->forwardKey, vec2_t(32.f, windowSize.y - 74));
			DrawUtils::drawKeystroke(*input->leftKey, vec2_t(10.f, windowSize.y - 52));
			DrawUtils::drawKeystroke(*input->backKey, vec2_t(32.f, windowSize.y - 52));
			DrawUtils::drawKeystroke(*input->rightKey, vec2_t(54.f, windowSize.y - 52));
			DrawUtils::drawKeystroke(*input->spaceBarKey, vec2_t(10.f, windowSize.y - 30));
		}
	}
}

void HudModule::onDisable() {
}