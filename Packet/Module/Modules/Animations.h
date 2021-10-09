#pragma once
#include "../ModuleManager.h"
#include "Module.h"
class Animations : public IModule {
private:
	bool reset = false;
	int delay = 0;

public:
	bool rightClickDown = false;
	bool doTranslate = true;
	bool aroundWorld = false;
	bool doScale = true;
	bool doRotate = false;

	float float1 = 0;
	float xMod = 1.f;
	float yMod = 1.f;
	float zMod = 1.f;

	float xTrans = 0.f;
	float yTrans = 0.f;
	float zTrans = 0.f;

	float xRotate = 0.f;
	float yRotate = 0.f;
	float zRotate = 0.f;
	Animations();
	~Animations();

	std::string name = "Animations";
	SettingEnum mode = this;


	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual const char* getRawModuleName() override;
};