#pragma once
//#include "Tag.h"
#include <memory>
#include "TextHolder.h"
#include "../Utils/HMath.h"

class CompoundTag;
class Tag;
class C_BlockLegacy;
class C_BlockSource;
class C_Block;
class C_Entity;
class C_Player;
class C_ItemStack;
class C_ScreenContext;
class C_ItemRenderer;
class C_ClientInstance;
class MinecraftGame;


class ItemDescriptor {
private:
	char pad_0x0[0x48];  //0x0000
public:
	ItemDescriptor() {
		memset(this, 0x0, sizeof(ItemDescriptor));
	}
	ItemDescriptor(int id, int16_t itemData);
};


class C_BaseActorRenderContext {
private:
	char pad_0x0[0x58];  //0x0000
	__int64 helloIamHereAndiFix;

public:
	C_ItemRenderer* renderer;  //0x0058
private:
	char pad_0x50[0x238];  //0x60
public:
	C_BaseActorRenderContext(C_ScreenContext* ScreenCtx, C_ClientInstance* client, MinecraftGame* game);
};

class C_ItemRenderer {
public:
	void renderGuiItemNew(C_BaseActorRenderContext* BaseActorRenderCtx, C_ItemStack* item, MinecraftGame* game, float x, float y, float opacity, float scale, bool isEnchanted);
	void renderGuiItemInChunk(C_BaseActorRenderContext* BaseActorRenderCtx, C_ItemStack* item, float x, float y, float opacity, float scale, bool isEnchanted, int mode);
};

class C_Item {
	private:
		char pad_0008[0x70];  //0x0008
	public:
		int16_t maxStackSize;  //0x0090
		int16_t itemId;        //0x0092
	private:
		char pad_0094[4];  //0x0094
	public:
		TextHolder tileName;  //0x0098
	private:
		char pad_00B8[8];  //0x00B8
	public:
		TextHolder name;  //0x00C0
	private:
		char pad_00E0[8];  //0x00E0
	public:
		TextHolder isFrom;  //0x00E8
	private:
		char pad_0108[80];  //0x0108
	public:
		int16_t duration;  //0x0158

		virtual void destructor();
		virtual __int64 initServer(__int64 jsonValue);                                                                   // 1
		virtual __int64 tearDown(void);                                                                                  // 2
		virtual __int64 getMaxUseDuration(__int64 ItemInstance);                                                         // 3
		virtual __int64 getMaxUseDuration(C_ItemStack const *);                                                          // 4
		virtual bool isMusicDisk(void);                                                                                  // 5
		virtual __int64 executeEvent(C_ItemStack &, const TextHolder &, class RenderParams &);                          // 6
		virtual bool isComponentBased(void);                                                                             // 7
		virtual bool isArmor(void);                                                                                      // 8
		virtual bool isBlockPlanterItem(void);                                                                           // 9
		virtual bool isBucket(void);                                                                                     // 10
		virtual bool isCamera(void);                                                                                     // 11
		virtual bool isCandle(void);                                                                                     // 12
		virtual bool isDamageable(void);                                                                                 // 13
		virtual bool isDyeable(void);                                                                                    // 14
		virtual bool isDye(void);                                                                                        // 15
		virtual __int64 getItemColor(void);                                                                              // 16
		virtual bool isFertilizer(void);                                                                                 // 17
		virtual bool isFood(void);                                                                                       // 18
		virtual bool isThrowable(void);                                                                                  // 19
		virtual bool isUseable(void);                                                                                    // 20
		virtual __int64 getCamera(void);                                                                                 // 21
		virtual __int64 getFood(void);                                                                                   // 22
		virtual __int64 getFuel(void);                                                                                   // 23
		virtual __int64 setMaxStackSize(unsigned char);                                                                  // 24
		virtual __int64 setStackedByData(bool);                                                                          // 25
		virtual __int64 setMaxDamage(int);                                                                               // 26
		virtual __int64 setHandEquipped(void);                                                                           // 27
		virtual __int64 setUseAnimation(enum UseAnimation);                                                              // 28
		virtual __int64 setMaxUseDuration(int);                                                                          // 29
		virtual __int64 setRequiresWorldBuilder(bool);                                                                   // 30
		virtual __int64 setExplodable(bool);                                                                             // 31
		virtual bool setFireResistant(bool);                                                                             // 32
		virtual __int64 setIsGlint(bool);                                                                                // 33
		virtual __int64 setShouldDespawn(bool);                                                                          // 34
		virtual __int64 getBlockShape(void);                                                                             // 35
		virtual __int64 canBeDepleted(void);                                                                             // 36
		virtual __int64 canDestroySpecial(C_Block const &);                                                              // 37
		virtual __int64 getLevelDataForAuxValue(int);                                                                    // 38
		virtual bool isStackedByData(void);                                                                              // 39
		virtual __int64 getMaxDamage(void);                                                                              // 40
		virtual __int64 getAttackDamage(void);                                                                           // 41
		virtual bool isHandEquipped(void);                                                                               // 42
		virtual bool isGlint(C_ItemStack const &);                                                                       // 43
		virtual bool isPattern(void);                                                                                    // 44
		virtual __int64 getPatternIndex(void);                                                                           // 45
		virtual __int64 showsDurabilityInCreative(void);                                                                 // 46
		virtual bool isWearableThroughLootTable(CompoundTag const *);                                                    // 47
		virtual __int64 canDestroyInCreative(void);                                                                      // 48
		virtual bool isDestructive(int);                                                                                 // 49
		virtual bool isLiquidClipItem(int);                                                                              // 50
		virtual __int64 shouldInteractionWithBlockBypassLiquid(C_Block const &);                                         // 51
		virtual __int64 requiresInteract(void);                                                                          // 52
		virtual __int64 appendFormattedHovertext(C_ItemStack const &, class Level &, TextHolder &, bool);                // 53
		virtual bool isValidRepairItem(C_ItemStack const &, C_ItemStack const &, struct BaseGameVersion const &);        // 54
		virtual __int64 getEnchantSlot(void);                                                                            // 55
		virtual __int64 getEnchantValue(void);                                                                           // 56
		virtual __int64 getArmorValue(void);                                                                             // 57
		virtual bool isComplex(void);                                                                                    // 58
		virtual bool isValidAuxValue(int);                                                                               // 59
		virtual __int64 getDamageChance(int);                                                                            // 60
		virtual __int64 getViewDamping(void);                                                                            // 61
		virtual __int64 uniqueAuxValues(void);                                                                           // 62
		virtual bool isActorPlacerItem(void);                                                                            // 63
		virtual bool isMultiColorTinted(C_ItemStack const &);                                                            // 64
		virtual __int64 getColor(CompoundTag const *, ItemDescriptor const &);                                           // 65
		virtual __int64 hasCustomColor(C_ItemStack const &);                                                             // 66
		virtual __int64 hasCustomColor(CompoundTag const *);                                                             // 67
		virtual __int64 clearColor(C_ItemStack &);                                                                       // 68
		virtual __int64 clearColor(CompoundTag *);                                                                       // 69
		virtual __int64 setColor(C_ItemStack &, struct MC_Color const &);                                                // 70
		virtual __int64 getBaseColor(C_ItemStack const &);                                                               // 71
		virtual __int64 getSecondaryColor(C_ItemStack const &);                                                          // 72
		virtual __int64 getActorIdentifier(C_ItemStack const &);                                                         // 73
		virtual __int64 saveAdditionalData(C_ItemStack const &, CompoundTag &);                                          // 74
		virtual __int64 readAdditionalData(C_ItemStack &, CompoundTag const &);                                          // 75
		virtual __int64 buildIdAux(short, CompoundTag const *);                                                          // 76
		virtual __int64 use(C_ItemStack &, C_Player &);                                                                  // 77
		virtual bool dispense(C_BlockSource &, class Container &, int, vec3_t const &, unsigned char);                   // 78
		virtual __int64 useTimeDepleted(C_ItemStack &, Level *, CompoundTag *);                                          // 79
		virtual __int64 releaseUsing(C_ItemStack &, CompoundTag *, int);                                                 // 80
		virtual __int64 getDestroySpeed(C_ItemStack const &, C_Block const &);                                           // 81
		virtual __int64 hurtActor(C_ItemStack &, C_Entity &, C_Entity &);                                                // 82
		virtual __int64 hitActor(C_ItemStack &, C_Entity &, C_Entity &);                                                 // 83
		virtual __int64 hitBlock(C_ItemStack &, C_Block const &, vec3_ti const &, C_Entity &);                           // 84
		virtual __int64 mineBlock(__int64 ItemInstance, C_Block const &, int, int, int, C_Entity *);                     // 85
		virtual __int64 mineBlock(C_ItemStack &, C_Block const &, int, int, int, C_Entity *);                            // 86
		virtual __int64 buildDescriptionName(C_ItemStack const &);                                                       // 87
		virtual __int64 buildDescriptionId(ItemDescriptor const &, CompoundTag const *);                                 // 88
		virtual __int64 buildEffectDescriptionName(C_ItemStack const &);                                                 // 89
		virtual __int64 buildCategoryDescriptionName(void);                                                              // 90
		virtual __int64 readUserData(C_ItemStack &, class IDataInput &, class ReadOnlyBinaryStream &);                   // 91
		virtual __int64 writeUserData(C_ItemStack const &, class IDataOutput &);                                         // 92
		virtual __int64 getMaxStackSize(ItemDescriptor *);                                                         // 93
		virtual __int64 inventoryTick(C_ItemStack &, Level &, C_Entity &, int, bool);                                    // 94
		virtual __int64 refreshedInContainer(C_ItemStack const &, Level &);                                              // 95
		virtual __int64 getCooldownType(void);                                                                           // 96
		virtual __int64 getCooldownTime(void);                                                                           // 97
		virtual __int64 fixupCommon(C_ItemStack &);                                                                      // 98
		virtual __int64 fixupCommon(C_ItemStack &, Level &);                                                             // 99
		virtual __int64 getDamageValue(CompoundTag const *);                                                             // 100
		virtual __int64 setDamageValue(C_ItemStack &, short);                                                            // 101
		virtual __int64 getInHandUpdateType(C_Player const &, __int64 itemInstance, __int64 itemInstancee, bool, bool);  // 102
		virtual __int64 getInHandUpdateType(C_Player const &, C_ItemStack const &, C_ItemStack const &, bool, bool);     // 103
		virtual bool validFishInteraction(int);                                                                          // 104
		virtual bool isSameItem(C_ItemStack const &, C_ItemStack const &);                                               // 105
		virtual __int64 initClient(__int64 jsonValue);                                                                   // 106
		virtual __int64 getInteractText(C_Player const &);                                                               // 107
		virtual __int64 getAnimationFrameFor(C_Entity *, bool, C_ItemStack const *, bool);                               // 108
		virtual bool isEmissive(int);                                                                                    // 109
		virtual bool getLightEmission(int);                                                                              // 110
		virtual __int64 getIcon(C_ItemStack const &, int, bool);                                                         // 111
		virtual __int64 getIconYOffset(void);                                                                            // 112
		virtual __int64 setIcon(const TextHolder &, int);                                                                // 113
		virtual __int64 setIcon(struct TextureUVCoordinateSet const &);                                                  // 114
		virtual __int64 setIconAtlas(const TextHolder &, int);                                                           // 115
		virtual __int64 canBeCharged(void);                                                                              // 116
		virtual __int64 playSoundIncrementally(__int64 itemInstance, C_Entity &);                                        // 117
		virtual __int64 playSoundIncrementally(C_ItemStack const &, C_Entity &);                                         // 118
		virtual __int64 getFurnaceBurnIntervalMultipler(C_ItemStack const &);                                            // 119
		virtual __int64 getFurnaceXPmultiplier(C_ItemStack const &);                                                     // 120
		virtual __int64 getAuxValuesDescription(void);                                                                   // 121
		virtual bool _checkUseOnPermissions(C_Entity &, C_ItemStack &, unsigned char const &, vec3_ti const &);          // 122
		virtual __int64 _calculatePlacePos(C_ItemStack &, C_Entity &, unsigned char &, vec3_ti &);                       // 123
		virtual __int64 _useOn(C_ItemStack &, C_Entity &, vec3_ti, unsigned char, float, float, float);                  // 124
		

public:
	bool isTool(void) {
		if (getAttackDamage() > 0) return true;           // Does Attack Damage
		if (itemId == 300 || itemId == 301) return true;  // Bow
		if (itemId == 299) return true;                   // Flint and Steel
		return false;
	}
	/*bool isFood(void) {
		if (itemId == 322 || itemId == 466) return true;                                                                      // Golden Apple
		if (itemId == 260) return true;                                                                                       // Apple
		if (itemId == 282) return true;                                                                                       // Mushroom Stew
		if (itemId == 297) return true;                                                                                       // Bread
		if (itemId == 319 || itemId == 320) return true;                                                                      // Porkchop
		if (itemId == 349 || itemId == 350 || itemId == 460 || itemId == 461 || itemId == 462 || itemId == 463) return true;  // Fish
		if (itemId == 357) return true;                                                                                       // Cookie
		if (itemId == 360) return true;                                                                                       // Melon
		if (itemId == 363 || itemId == 364) return true;                                                                      // Beef
		if (itemId == 365 || itemId == 366) return true;                                                                      // Chicken
		if (itemId == 391) return true;                                                                                       // Carrot
		if (itemId == 392 || itemId == 393) return true;                                                                      // Potato
		if (itemId == 400) return true;                                                                                       // Pumpkin Pie
		if (itemId == 411 || itemId == 412 || itemId == 413) return true;                                                     // Rabbit thing
		if (itemId == 423 || itemId == 424) return true;                                                                      // Mutton
		if (itemId == 450) return true;
		return false;
	}*/
	bool isBlock(void) {
		auto val = *reinterpret_cast<__int64 ***>(reinterpret_cast<__int64>(this) + 0x1A0);
		return val != nullptr && *val != nullptr;
	}
};



class C_ItemStack {
private:
	uintptr_t** vTable;  //0x0000
public:
	C_Item** item;  //0x08
	CompoundTag* tag;  //0x10
private:
	char pad_0x18[0xA]; //0x18
public:
	char count;  //0x22
private:
	char pad_0x1B[0x6D];  //0x23
public:
	C_ItemStack() {
		memset(this, 0x0, sizeof(C_ItemStack));
	}

	C_ItemStack(C_BlockLegacy& legacy, int count) {
		memset(this, 0x0, sizeof(C_ItemStack));
		reinit(legacy, count);
	}

	C_ItemStack(C_Item& item, int count, int itemData) {
		memset(this, 0x0, sizeof(C_ItemStack));
		reinit(item, count, itemData);
	}

	C_ItemStack(C_ItemStack const& src);

	C_ItemStack(Tag const& tag);

	void fromTag(Tag const& tag);

	void save(CompoundTag** tag);

	void setUserData(std::unique_ptr<Tag> tag);

	bool isValid() {
		return this->item != nullptr && *this->item != nullptr;
	}

	inline C_Item* getItem() {
		return *this->item;
	}

	void reinit(C_BlockLegacy& legacy, int count);

	void reinit(C_Item& item, int count, int itemData);

	int getEnchantValue(int enchantId);

	bool isEnchanted() {
		int enchantValue = 0;
		for (int i = 0; i < 40; i++) {
			enchantValue = this->getEnchantValue(i);
		}
		if (enchantValue != 0)
			return true;
		else
			return false;
	}

	float getArmorValueWithEnchants() {
		if (!this->item || !(*this->item)->isArmor())
			return 0;

		return (float)(((*this->item)->getArmorValue() + ((this->getEnchantValue(0) * 1.5f		// Protection
														   + this->getEnchantValue(5) * 0.4f	// Thorns
														   + this->getEnchantValue(3) * 0.4f	// Blast Protection
														   + this->getEnchantValue(1) * 0.4f	// Fire Protection
														   + this->getEnchantValue(4) * 0.4f	// Projectile Protection
			))));
	}

	float getAttackingDamageWithEnchants() {
		if (!this->item)
			return 0;
		int sharpnessValue = this->getEnchantValue(9);
		return (*this->item)->getAttackDamage() + 1.25f * sharpnessValue;
	}

	void setShowPickUp(bool b) {
		*(bool*)(reinterpret_cast<__int64>(this)+ 0x30) = b;
	}

private:
	inline void setVtable(void);
};

class C_ArmorItem : public C_Item {
public:
	virtual bool getArmorKnockbackResistance(void);

	int getArmorSlot() { return *(int *)((__int64)this + 0x228); }

	bool isHelmet() {
		return getArmorSlot() == 0;
	}
	bool isChestplate() {
		return getArmorSlot() == 1;
	}
	bool isLeggings() {
		return getArmorSlot() == 2;
	}
	bool isBoots() {
		return getArmorSlot() == 3;
	}
};


class ItemRegistry {
public:
	static C_Item*** getItemFromId(void* ptr, int itemId);
	static C_Item*** lookUpByName(void*, void*, TextHolder&);
};
