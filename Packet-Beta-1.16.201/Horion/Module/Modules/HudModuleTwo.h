#pragma once
#include "Module.h"

class HudModuleTwo : public IModule {
public:
	HudModuleTwo();
	~HudModuleTwo();


	float bpsX = 100.f;
	float bpsY = 100.f;
	float speed = 0.f;
	float scale = 1.f;
	bool tabgui = false;
	bool shadowMode = true;
	bool textShadow = true;
	bool versionMode = false;
	bool rainbow = false;
	bool r2 = false;
	bool horion = false;
	bool white = false;
	bool dynamic = false;
	bool arraylist = true;
	bool arraylistOutline = true;
	bool arraylistBackground = true;
	bool clickToggle = true;
	bool bps = true;
	bool watermark = true;
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

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onDisable() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};