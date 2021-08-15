#include "ClickGuiMod.h"

#include "../../Menu/ClickGui.h"

ClickGuiMod::ClickGuiMod() : IModule(VK_INSERT, Category::VISUAL, "The clickgui - toggle everything by just clicking on it!") {
	registerEnumSetting("Theme", &theme, 1);
	theme.addEntry("Packet", 0);
	theme.addEntry("Fadeaway", 1);
	//registerBoolSetting("Rainbow", &this->rainbowColor, this->rainbowColor);
	registerFloatSetting("Opacity", &this->opacity, this->opacity, 0.05f, 1.f);
}

ClickGuiMod::~ClickGuiMod() {
}

const char* ClickGuiMod::getModuleName() {
	return ("ClickGui");
}

void ClickGuiMod::onEnable() {
	g_Data.getClientInstance()->releaseMouse();
	bgOpacity = 0.1f;
	bgCounter = 1;
}

void ClickGuiMod::onTick(C_GameMode* gm) {
	// worst code but idc
	if (bgCounter == INFINITY) {
		bgCounter = 1;
	} else {
		bgCounter++;
	}
	if (bgCounter == 2) {
		bgOpacity = 0.1f;
	} else if (bgCounter == 3) {
		bgOpacity = 0.115f;
	} else if (bgCounter == 4) {
		bgOpacity = 0.12f;
	} else if (bgCounter == 5) {
		bgOpacity = 0.13f;
	} else if (bgCounter == 6) {
		bgOpacity = 0.14f;
	} else if (bgCounter == 7) {
		bgOpacity = 0.15f;
	} else if (bgCounter == 8) {
		bgOpacity = 0.16f;
	} else if (bgCounter == 9) {
		bgOpacity = 0.17f;
	}


	// alot of color code yes
	if (theme.getSelectedValue() == 0) {  // packet
		backroundR = 0;
		backroundG = 0;
		backroundB = 0;

		textR = 1;
		textG = 1;
		textB = 1;

		dtextR = 0.40;
		dtextG = 0.40;
		dtextB = 0.40;

		sliderR = 0.32;
		sliderG = 0.32;
		sliderB = 0.32;
	}

	if (theme.getSelectedValue() == 1) {  // fadeaway
		opacity = 1;
		backroundR = 0;
		backroundG = 0;
		backroundB = 0;

		textR = 1;
		textG = 1;
		textB = 1;

		dtextR = 1;
		dtextG = 1;
		dtextB = 1;

		sliderR = 0.32;
		sliderG = 0.32;
		sliderB = 0.32;
	}
}

float ClickGuiMod::textR = 0.f;
float ClickGuiMod::textG = 0.f;
float ClickGuiMod::textB = 0.f;

float ClickGuiMod::dtextR = 0.f;
float ClickGuiMod::dtextG = 0.f;
float ClickGuiMod::dtextB = 0.f;

float ClickGuiMod::sliderR = 0.f;
float ClickGuiMod::sliderG = 0.f;
float ClickGuiMod::sliderB = 0.f;

float ClickGuiMod::backroundR = 0.f;
float ClickGuiMod::backroundG = 0.f;
float ClickGuiMod::backroundB = 0.f;

bool ClickGuiMod::allowAutoStart() {
	return false;
}

void ClickGuiMod::onDisable() {
	g_Data.getClientInstance()->grabMouse();
	openAnimation = false;
}

void ClickGuiMod::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if (GameData::canUseMoveKeys())
		g_Data.getClientInstance()->releaseMouse();
}
void ClickGuiMod::onLoadConfig(void* conf) {
	IModule::onLoadConfig(conf);
	ClickGui::onLoadConfig(conf);
}
void ClickGuiMod::onSaveConfig(void* conf) {
	IModule::onSaveConfig(conf);
	ClickGui::onSaveConfig(conf);
}