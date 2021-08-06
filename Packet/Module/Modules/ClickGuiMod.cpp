#include "ClickGuiMod.h"
#include "../../Menu/ClickGui.h"

ClickGuiMod::ClickGuiMod() : IModule(VK_INSERT, Category::VISUAL, "The clickgui - toggle everything by just clicking on it!") {
	registerEnumSetting("Theme", &theme, 1);
	theme.addEntry("Packet", 0);
	theme.addEntry("Fadeaway", 1);
	registerBoolSetting("Animations", &this->animations, this->animations);
	registerBoolSetting("Rainbow", &this->rainbowColor, this->rainbowColor);
	registerFloatSetting("Opacity", &this->opacity, this->opacity, 0.05f, 1.f);
	//registerBoolSetting("Show Tooltips", &this->showTooltips, this->showTooltips);
}

ClickGuiMod::~ClickGuiMod() {
}

const char* ClickGuiMod::getModuleName() {
	return ("ClickGui");
}

void ClickGuiMod::onEnable() {
	g_Data.getClientInstance()->releaseMouse();
	makeOnixMadCounter = 1;
}

void ClickGuiMod::onTick(C_GameMode* gm) {
	if (makeOnixMadCounter == INFINITY) {
		makeOnixMadCounter = 1;
	} else {
		makeOnixMadCounter++;
	}
	if (makeOnixMadCounter == 2) {
		openAnimation = true;
	}
	if (makeOnixMadCounter == 3) {
		openAnimation = false;
	}
	if (makeOnixMadCounter == 10) {
		headerAnimation = false;
	}
}

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
