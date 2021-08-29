#pragma once

#include "Module.h"

class HudEditorMod : public IModule {
public:
	HudEditorMod();
	~HudEditorMod();

	virtual const char* getModuleName();
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onDisable() override;
	virtual void onEnable() override;
	virtual bool allowAutoStart() override;
	//virtual void onLoadConfig(void* conf) override;
	//virtual void onSaveConfig(void* conf) override;
};