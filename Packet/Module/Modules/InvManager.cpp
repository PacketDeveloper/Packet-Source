#include "InvManager.h"

#include "../ModuleManager.h"

InvManager::InvManager() : IModule(0, Category::PLAYER, "Automatintory YEP") {
	registerBoolSetting("Cleaner", &clean, clean);
	//registerBoolSetting("Blocks", &keepBlocks, keepBlocks);
	//registerBoolSetting("Tools", &keepTools, keepTools);
	//registerBoolSetting("Armor", &keepArmor, keepArmor);
	//registerBoolSetting("Food", &keepFood, keepFood);

	registerBoolSetting("InvOnly", &openInv, openInv);
	//registerIntSetting("Sword", &swordSlot, swordSlot, 0, 9);
	//registerIntSetting("Pickaxe", &pickSlot, pickSlot, 0, 9);
	//registerIntSetting("Axe", &axeSlot, axeSlot, 0, 9);
	//registerIntSetting("Blocks", &blockSlot, blockSlot, 0, 9);
	registerBoolSetting("AutoDisable", &autoDisable, autoDisable);
	registerIntSetting("Delay", &delay, delay, 0, 30);
}

InvManager::~InvManager() {
}

const char* InvManager::getRawModuleName() {
	return "InvManager";
}

const char* InvManager::getModuleName() {
	if (clean) {
		name = std::string("InvManager ") + std::string(GRAY) + std::string("Clean");
		return name.c_str();
	} else {
		return "InvManager";
	}
}

void InvManager::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (g_Data.getLocalPlayer()->canOpenContainerScreen() && openInv) 
		return;

	// Drop items
	if (clean) {
		std::vector<int> dropSlots = findUselessItems();
		Odelay++;
		if (Odelay > delay) {
			if (!dropSlots.empty()) {
				for (int i : dropSlots) {
					Odelay++;
					if (Odelay > delay) {
						g_Data.getLocalPlayer()->getSupplies()->inventory->dropSlot(i);
						dropSlots.push_back(i);
					}
				}
			}
			Odelay = 0;
		}
	}

	if (autoSort) {
		// Put sword in first slot -- crashes
		/*{
			C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
			C_Inventory* inv = supplies->inventory;
			float damage = 0;
			int item = 256;
			for (int n = 0; n < 36; n++) {
				C_ItemStack* stack = inv->getItemStack(n);
				if (stack->item != NULL) {
					float currentDamage = stack->getAttackingDamageWithEnchants();
					if (currentDamage > damage) {
						damage = currentDamage;
						item = n;
					}
				}
			}
			if (item != 0) inv->moveItem(item, swordSlot);
		}*/
	}
	auto inGame = g_Data.isInGame();
	if (autoDisable && !inGame) {
		auto box = g_Data.addInfoBox("InvManager", "Disabled due to world change");
		box->closeTimer = 10;
		setEnabled(false);
	}
}

std::vector<int> InvManager::findStackableItems() {
	std::vector<int> stackableSlot;

	for (int i = 0; i < 36; i++) {
		C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemStack->item != nullptr) {
			if ((*itemStack->item)->getMaxStackSize(0) > itemStack->count) {
				for (int i2 = 0; i2 < 36; i2++) {
					if (i2 == i) continue;
					C_ItemStack* itemStack2 = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i2);
					if ((*itemStack2->item)->getMaxStackSize(0) > itemStack->count) {
						if (*itemStack->item == *itemStack2->item) {
							if ((std::find(stackableSlot.begin(), stackableSlot.end(), i2) == stackableSlot.end())) stackableSlot.push_back(i2);
							if ((std::find(stackableSlot.begin(), stackableSlot.end(), i) == stackableSlot.end())) stackableSlot.push_back(i);
						}
					}
				}
			}
		}
	}

	return stackableSlot;
}

std::vector<int> InvManager::findUselessItems() {
	// Filter by options

	std::vector<int> uselessItems;
	std::vector<C_ItemStack*> items;

	{
		for (int i = 0; i < 36; i++) { // 36
			C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
			if (itemStack->item != nullptr) {
				if (!stackIsUseful(itemStack)) {
					if (std::find(items.begin(), items.end(), itemStack) == items.end())
						uselessItems.push_back(i);
					else
						items.push_back(itemStack);
				} else if (std::find(items.begin(), items.end(), itemStack) == items.end()) {
					if ((*itemStack->item)->itemId == 261 && !isLastItem(*itemStack->item))
						uselessItems.push_back(i);
					else
						items.push_back(itemStack);	
				}
			}
		}

		for (int i = 0; i < 4; i++) {
			if (g_Data.getLocalPlayer()->getArmor(i)->item != nullptr)
				items.push_back(g_Data.getLocalPlayer()->getArmor(i));
		}
	}
	// Filter weapons
	if (items.size() > 0) {
		// Filter by attack damage
		std::sort(items.begin(), items.end(), [](const C_ItemStack* lhs, const C_ItemStack* rhs) {
			C_ItemStack* current = const_cast<C_ItemStack*>(lhs);
			C_ItemStack* other = const_cast<C_ItemStack*>(rhs);
			return current->getAttackingDamageWithEnchants() > other->getAttackingDamageWithEnchants();
		});

		bool hadTheBestItem = false;
		C_ItemStack* bestItem = items.at(0);

		for (int i = 0; i < 36; i++) { // 36
			if (std::find(uselessItems.begin(), uselessItems.end(), i) != uselessItems.end())
				continue;
			C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
			if (itemStack->item != nullptr && itemStack->getAttackingDamageWithEnchants() > 1) {
				if (itemStack->getAttackingDamageWithEnchants() < bestItem->getAttackingDamageWithEnchants()) {
					uselessItems.push_back(i);
				} else {
					// Damage same as bestItem
					if (hadTheBestItem)
						uselessItems.push_back(i);
					else
						hadTheBestItem = true;
				}
			}
		}
	}
	// Filter armor
	{
		std::vector<C_ItemStack*> helmets;
		std::vector<C_ItemStack*> chestplates;
		std::vector<C_ItemStack*> leggings;
		std::vector<C_ItemStack*> boots;

		// Filter by armor value
		std::sort(items.begin(), items.end(), [](const C_ItemStack* lhs, const C_ItemStack* rhs) {
			C_ItemStack* current = const_cast<C_ItemStack*>(lhs);
			C_ItemStack* other = const_cast<C_ItemStack*>(rhs);
			return current->getArmorValueWithEnchants() > other->getArmorValueWithEnchants();
		});

		// Put armor items in their respective vectors
		for (C_ItemStack* itemsteck : items) {
			C_Item* item = itemsteck->getItem();
			if (item->isArmor()) {
				C_ArmorItem* armorItem = reinterpret_cast<C_ArmorItem*>(item);
				if (armorItem->isHelmet())
					helmets.push_back(itemsteck);
				else if (armorItem->isChestplate())
					chestplates.push_back(itemsteck);
				else if (armorItem->isLeggings())
					leggings.push_back(itemsteck);
				else if (armorItem->isBoots())
					boots.push_back(itemsteck);
			}
		}
		bool hadBest[4] = {0, 0, 0, 0};
		for (int i = 0; i < 4; i++) {
			C_ItemStack* itemsteck = g_Data.getLocalPlayer()->getArmor(i);
			C_Item** item = itemsteck->item;
			if (item != nullptr) {
				
				C_ArmorItem* armor = reinterpret_cast<C_ArmorItem*>(*item);
				
				float testArmorValue = 0;
				switch (armor->getArmorSlot()) {
				case 0:
					if (helmets.size() > 0)
						testArmorValue = helmets.at(0)->getArmorValueWithEnchants();
					break;
				case 1:
					if (chestplates.size() > 0)
						testArmorValue = chestplates.at(0)->getArmorValueWithEnchants();
					break;
				case 2:
					if (leggings.size() > 0)
						testArmorValue = leggings.at(0)->getArmorValueWithEnchants();
					break;
				case 3:
					if (boots.size() > 0)
						testArmorValue = boots.at(0)->getArmorValueWithEnchants();
					break;
				}
				if (itemsteck->getArmorValueWithEnchants() >= testArmorValue)
					hadBest[armor->getArmorSlot()] = true;
			}
		}

		for (int i = 0; i < 36; i++) { // 36
			if (std::find(uselessItems.begin(), uselessItems.end(), i) != uselessItems.end())
				continue;  // item already useless
			C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
			if (itemStack->item != nullptr && (*itemStack->item)->isArmor()) {
				C_ArmorItem* armor = reinterpret_cast<C_ArmorItem*>(*itemStack->item);
				if (armor->isHelmet()) {
					if (hadBest[0] || itemStack->getArmorValueWithEnchants() < helmets.at(0)->getArmorValueWithEnchants()) {
						uselessItems.push_back(i);
					} else
						hadBest[0] = true;
				} else if (armor->isChestplate()) {
					if (hadBest[1] || itemStack->getArmorValueWithEnchants() < chestplates.at(0)->getArmorValueWithEnchants())
						uselessItems.push_back(i);
					else
						hadBest[1] = true;
				} else if (armor->isLeggings()) {
					if (hadBest[2] || itemStack->getArmorValueWithEnchants() < leggings.at(0)->getArmorValueWithEnchants())
						uselessItems.push_back(i);
					else
						hadBest[2] = true;
				} else if (armor->isBoots()) {
					if (hadBest[3] || itemStack->getArmorValueWithEnchants() < boots.at(0)->getArmorValueWithEnchants())
						uselessItems.push_back(i);
					else
						hadBest[3] = true;
				}
			}
		}
	}

	return uselessItems;
}

bool InvManager::stackIsUseful(C_ItemStack* itemStack) {
	if (itemStack->item == nullptr) return true;
	if (keep) {
		if ((*itemStack->item)->isArmor()) return true;      // Armor
		if ((*itemStack->item)->isTool()) return true;       // Tools
		if ((*itemStack->item)->isWeapon()) return true;     // Weapon
		if ((*itemStack->item)->isFood()) return true;       // Food
		if ((*itemStack->item)->isBlock()) return true;      // Block
	}
	return false;
}

bool InvManager::isLastItem(C_Item* item) {
	std::vector<C_Item*> items;
	for (int i = 0; i < 36; i++) {
		C_ItemStack* stack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (stack->item != nullptr) items.push_back((*stack->item));
	}
	int count = 0;
	for (C_Item* a : items) {
		if (a == item) count++;
	}
	if (count > 1) return false;
	return true;
}
