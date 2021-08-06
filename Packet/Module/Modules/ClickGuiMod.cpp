#include "ClickGuiMod.h"
#include "../../Menu/ClickGui.h"

ClickGuiMod::ClickGuiMod() : IModule(VK_INSERT, Category::VISUAL, "The clickgui - toggle everything by just clicking on it!") {
	registerBoolSetting("Animations", &animations, animations);
	registerBoolSetting("Rainbow", &rainbowColor, rainbowColor);
	registerFloatSetting("Opacity", &opacity, opacity, 0.05f, 1.f);
	//registerBoolSetting("Show Tooltips", &showTooltips, showTooltips);
}

ClickGuiMod::~ClickGuiMod() {
}

const char* ClickGuiMod::getModuleName() {
	return ("ClickGui");
}

void ClickGuiMod::onEnable() {
	g_Data.getClientInstance()->releaseMouse();
	auto clickGUI = moduleMgr->getModule<ClickGui>();
	//clickGUI->ourWindow->animation = 0.2f;
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
