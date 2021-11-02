#pragma once
#include "Module.h"
class Sprint : public IModule {
public:
	bool omniSprint = false;
	bool useSprint = true;

	Sprint();
	~Sprint();

	virtual void onTick(C_GameMode* gm) override;
	virtual const char* getModuleName() override;
};
