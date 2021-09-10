#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class HackerDetector : public IModule {
public:
	bool speedCheck = false;
	bool flyCheck = false;

	HackerDetector();
	~HackerDetector();

	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onTick(C_GameMode* gm) override;
	//virtual void onMove(C_MoveInputHandler* input) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual const char* getModuleName();
};