#pragma once
#include "Module.h"
class InventoryCleaner : public IModule {
private:
	bool sorted = false;

	std::vector<int> findStackableItems();
	std::vector<int> findUselessItems();
	bool isLastItem(C_Item* item);
	
	bool clean = true;
	bool keepTools = true;
	bool keepArmor = true;
	bool keepBlocks = true;
	bool keepFood = true;

	bool openInv = true;
	int test = 0;
	bool autoSort = true;

	// Slots
	int swordSlot = 0;
	int pickSlot = 1;
	int axeSlot = 2;
	int blockSlot = 9;

public:
	bool stackIsUseful(C_ItemStack* itemStack);
	InventoryCleaner();
	~InventoryCleaner();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
