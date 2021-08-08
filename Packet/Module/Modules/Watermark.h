#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class Watermark : public IModule {
private:
public:
	bool firstLetter = false;
	float opacity = 0.2f;

	Watermark();
	~Watermark();

		SettingEnum mode = this;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};