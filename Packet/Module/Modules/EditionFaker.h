#pragma once
#include "Module.h"
class EditionFaker : public IModule {
public:
	EditionFaker();
	~EditionFaker();

	int vers = 2;
	int getFakedEditon();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
