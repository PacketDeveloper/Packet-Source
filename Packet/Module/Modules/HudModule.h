#pragma once
#include "Module.h"

class HudModule : public IModule {
public:
	SettingEnum color = this;

	float speed = 0.f;
	float scale = 1.f;
	bool tabgui = false;
	bool arraylist = true;
	bool arraylistOutline = true;
	bool arraylistBackground = true;
	bool clickToggle = false;
	bool coordinates = false;
	bool keybinds = true;
	bool displayArmor = false;
	bool keystrokes = false;
	bool fps = false;
	bool cps = false;
	bool alwaysShow = false;
	float fpspos = 1.f;
	float fpspos2 = 1.f;
	float cpspos = 1.f;
	float cpspos2 = 1.f;

	HudModule();
	~HudModule();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onDisable() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};