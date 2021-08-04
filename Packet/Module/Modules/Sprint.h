#pragma once
#include "Module.h"
class Sprint : public IModule {
private:
	bool alldirections = false;

public:
	Sprint();
	~Sprint();

	virtual void onTick(C_GameMode* gm) override;
	virtual const char* getModuleName() override;
};
