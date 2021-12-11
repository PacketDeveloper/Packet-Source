#pragma once
#include "Module.h"

class HudModuleTwo : public IModule {
public:
	HudModuleTwo();
	~HudModuleTwo();

	bool tabgui = false;
	bool arraylist = true;
	bool arraylistOutline = true;
	bool arraylistBackground = true;
	bool rainbow = false;
	bool clickToggle = true;
	bool watermark = true;
	bool coordinates = false;
	bool keybinds = true;
	bool displayArmor = false;
	bool keystrokes = false;
	bool fps = false;
	bool cps = false;
	bool alwaysShow = false;

	float scale = 1.f;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};