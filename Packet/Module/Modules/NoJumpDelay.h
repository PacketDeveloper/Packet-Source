#pragma once
#include "Module.h"

class NoJumpDelay : public IModule {
public:
	NoJumpDelay();
	~NoJumpDelay();

	virtual void onTick(C_GameMode* gm) override;
	const char* getModuleName() override;
};
