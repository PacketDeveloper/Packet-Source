#pragma once
#include "Module.h"
class NoSlow : public IModule {
private:
	uint8_t* opcode = 0;
	uint8_t* opcode1 = 0;
	bool noweb = false;

public:
	NoSlow();
	~NoSlow();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
