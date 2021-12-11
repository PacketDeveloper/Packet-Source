#pragma once

#include "Module.h"

class Camera : public IModule {
public:
	bool nohurtMode = false;
	bool fullbrightMode = false;

	Camera();
	virtual const char* getModuleName();
};