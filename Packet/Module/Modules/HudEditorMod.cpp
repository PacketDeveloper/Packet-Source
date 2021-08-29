#include "HudEditorMod.h"

HudEditorMod::HudEditorMod() : IModule(0, Category::VISUAL, "onix client rip off lol") {
}

const char* HudEditorMod::getModuleName() {
	return "HudEditor";
}

HudEditorMod::~HudEditorMod() {
}

// WIP

void HudEditorMod::onEnable() {
	g_Data.getClientInstance()->releaseMouse();
}

bool HudEditorMod::allowAutoStart() {
	return false;
}

void HudEditorMod::onDisable() {
	g_Data.getClientInstance()->grabMouse();
}

void HudEditorMod::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if (GameData::canUseMoveKeys())
		g_Data.getClientInstance()->releaseMouse();
}

/*void HudEditorMod::onLoadConfig(void* conf) {
	IModule::onLoadConfig(conf);
	HudEditor::onLoadConfig(conf);
}
void HudEditorMod::onSaveConfig(void* conf) {
	IModule::onSaveConfig(conf);
	HudEditor::onSaveConfig(conf);
}*/