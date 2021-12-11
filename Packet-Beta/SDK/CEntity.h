#pragma once

#include "../Utils/HMath.h"
#include "CBlockLegacy.h"
#include "CEntityList.h"
#include "CInventory.h"
#include "CInventoryTransaction.h"
#include "TextHolder.h"

class C_GameMode;

class PointingStruct {
	char pad_0000[2392];  //0x0000
public:
	vec3_t position;  //0x0958
private:
	char pad_0964[12];  //0x0964
public:
	int32_t rayHitType;  //0x0970
	int32_t blockSide;   //0x0974
	vec3_ti block;         //0x0978
	vec3_t rayHitVec;   //0x0984
private:
	char pad_0990[16];  //0x0990
public:
	uint64_t entityLocalId;  //0x09A0

	bool hasEntity();       //to not go trough the entity list twice
	C_Entity *getEntity();  //returns the entity that the player is looking at
};

class C_Player;

#pragma pack(push, 4)
class C_Entity {
public:
	uint64_t ptrToEntityList;                     //0x0008
	__int64 entityLocalId;
private:
	char pad_0010[296];                           //0x0010
public:
	union {
		struct {
			float pitch;  
			float yaw;    
		};
		vec2_t viewAngles;
	};
	union {
		struct {
			float pitch2;
			float yaw2;
		};
		vec2_t viewAngles2;  
	};

private:
	char pad_0150[16];  //0x0150
public:
	vec3_t eyePos0;  //0x0160
private:
	char pad_016C[112];  //0x016C
public:
	float fallDistance;  //0x01DC
	bool onGround;     //0x01E0
private:
	char pad_01E1[95];  //0x01E1
public:
	float stepHeight;  //0x0240
	float web;         //0x0244
private:
	char pad_0248[29];  //0x0248
public:
	bool feetUnderwater;  //0x0265
private:
	char pad_0266[1];  //0x0266
public:
	bool hasEnteredWater;  //0x0267
private:
	char pad_0268[72];  //0x0268
public:
	int32_t ticksAlive;  //0x02B0
private:
	char pad_02B4[180];  //0x02B4
public:
	class C_BlockSource *region;  //0x0368
private:
	char pad_0370[8];  //0x0370
public:
	class PointingStruct *pointingStruct;  //0x0378
private:
	char pad_0380[336];  //0x0380
public:
	struct AABB aabb;    //0x04D0
	float width;        //0x04EC
	float height;       //0x04F0
	vec3_t currentPos;  //0x04F4
	vec3_t oldPos;      //0x0500
	vec3_t velocity;    //0x050C
	vec3_t velocity2;   //0x0518
private:
	char pad_0524[92];  //0x0524
public:
	int64_t entityRuntimeId;  //0x0580
private:
	char pad_0588[408];  //0x0588
public:
	float bodyYaw;                    //0x0720
	float oldBodyYaw;                 //0x0724
	float yawUnused1;                 //0x0728
	float yawUnused2;                 //0x072C
	int32_t damageTime;               //0x0730
	int32_t damageTotalTime;               //0x0734
private:
	char pad_0738[136];  //0x0738
public:
	int32_t timeSinceDeath;  //0x07C0
private:
	char pad_07C4[520];  //0x07C4
public:
	bool canFly;  //0x09CC
private:
	char pad_09CD[1779];  //0x09CD
public:
	int16_t itemData;  //0x10C0
	int16_t itemId;    //0x10C2
private:
	char pad_10C4[508];  //0x10C4
public:
	class C_InventoryTransactionManager transac;  //0x12C0
private:
	char pad_1320[2796];  //0x1320
public:
	int32_t gamemode;  //0x1E0C


	virtual void hasComponent(class HashedString const &);
	virtual void reloadHardcoded();              //(C_Entity::InitializationMethod, VariantParameterList const&); // 1
	virtual void reloadHardcodedClient();        //(C_Entity::InitializationMethod, VariantParameterList const&); // 2
	virtual void initializeComponents();         //(C_Entity::InitializationMethod, VariantParameterList const&); // 3
	virtual void reloadComponents();             //(C_Entity::InitializationMethod, VariantParameterList const&); // 4
	virtual void _serverInitC_ItemStackIds(void);  // 5
	virtual void _doInitialMove(void);           // 6
	virtual void destructor();
	virtual void reset(void);                                          // 8
	virtual void getOnDeathExperience(void);                           // 9
	virtual void getOwnerEntityType(void);                             // 10
	virtual void remove(void);                                         // 11
	virtual void setPos(vec3_t const &);                                 // 12
	virtual void Function13(void);                                     // 13
	virtual void getPredictedMovementValues(void);                     // 14
	virtual vec3_t* getPos(void) const;                     // 15
	virtual vec3_t* getPosOld(void);                                      // 16
	virtual void getPosExtrapolated(float);                            // 17
	virtual void getAttachPos(uintptr_t C_EntityLocation, float);         // 18
	virtual void getFiringPos(void);                                   // 19
	virtual void setRot(vec2_t const &);                                 // 20
	virtual void move(struct IC_EntityMovementProxy &, vec3_t const &);     // 21
	virtual void move(vec3_t const &);                                   // 22
	virtual void getInterpolatedRidingPosition(float);                 // 23
	virtual void getInterpolatedBodyRot(float);                        // 24
	virtual void getInterpolatedHeadRot(float);                        // 25
	virtual void getInterpolatedBodyYaw(float);                        // 26
	virtual void getYawSpeedInDegreesPerSecond(void);                  // 27
	virtual void getInterpolatedWalkAnimSpeed(float);                  // 28
	virtual void getWorldPosition(void);                               // 29
	virtual void checkBlockCollisions();                               //(AABB const&, std::function<void(BlockSource&, Block const&, vec3_ti const&, C_Entity*)>); // 30
	virtual void updateEntityInside(void);                             // 31
	virtual void updateEntityInside(struct AABB const &);              // 32
	virtual void isFireImmune(void);                                   // 33
	virtual void Function34();                                         //parse<AnimationMode>(void*, CommandRegistry::ParseToken const&, CommandOrigin const&, int, TextHolder*, std::vector<TextHolder, std::allocator<TextHolder>>&); // 34
	virtual void blockedByShield(class ActorDamageSource const &, C_Entity *);  // 35
	virtual void teleportTo(vec3_t const &, bool, int, int);             // 36
	virtual void tryTeleportTo(vec3_t const &, bool, bool, int, int);    // 37
	virtual void chorusFruitTeleport(vec3_t &);                          // 38
	virtual void lerpTo(vec3_t const &, vec2_t const &, int);              // 39
	virtual void lerpMotion(vec3_t const &);                             // 40
	virtual void tryCreateAddC_EntityC_Packet(void);                        // 41
	virtual void normalTick(void);                                     // 42
	virtual void baseTick(void);                                       // 43
	virtual void rideTick(void);                                       // 44
	virtual void positionRider(C_Entity *, float);                        // 45
	virtual void getRidingHeight(void);                                // 46
	virtual void startRiding(C_Entity *);                                 // 47
	virtual void addRider(C_Entity *);                                    // 48
	virtual void flagRiderToRemove(C_Entity *);                           // 49
	virtual void getExitTip(const TextHolder *, uintptr_t InputMode);        // 50
	virtual void intersects(vec3_t const &, vec3_t const &);               // 51
	virtual void isFree(vec3_t const &);                                 // 52
	virtual void isFree(vec3_t const &, float);                          // 53
	virtual void isInWall(void);                                       // 54
	virtual bool isInvisible(void);                                    // 55
	virtual bool canShowNameTag(void);                                 // 56
	virtual void Function57(void);                                     // 57
	virtual void setNameTagVisible(bool);                              // 58
	virtual TextHolder* getNameTag(void);                                     // 59

	//WE FORGOT ONE HERE!!!
	//WE FORGOT ONE HERE!!!
	//WE FORGOT ONE HERE!!!
	//WE FORGOT ONE HERE!!!
	//WE FORGOT ONE HERE!!!!!!!!!!!
	virtual void getNameTagAsHash(void);  // 59

	virtual void getFormattedNameTag(void);                                                                                                  // 60
	virtual void filterFormattedNameTag(class UIProfanityConTextHolder const &);                                                                   // 61
	virtual void setNameTag(const TextHolder *);                                                                                                   // 62
	virtual void Function63(void);                                                                                                           // 63
	virtual void setScoreTag(const TextHolder *);                                                                                                  // 64
	virtual void getScoreTag(void);                                                                                                          // 65
	virtual bool isInWater(void);                                                                                                                  // 66
	virtual bool v_hasEnteredWater(void);                                                                                                      // 67
	virtual bool isImmersedInWater(void);                                                                                                          // 68
	virtual bool isInLava(void);                                                                                                             // 69
	virtual bool isUnderLiquid(enum MaterialType);                                                                                                 // 70
	virtual bool isOverWater(void);                                                                                                          // 71
	virtual void setBlockMovementSlowdownMultiplier(vec3_t const &);                                                                           // 72
	virtual void resetBlockMovementSlowdownMultiplier(void);                                                                                 // 73
	virtual void getCameraOffset(void);                                                                                                      // 74
	virtual void getShadowHeightOffs(void);                                                                                                  // 75
	virtual void getShadowRadius(void);                                                                                                      // 76
	virtual void getHeadLookVector(float);                                                                                                   // 77
	virtual void Function78();                                                                                                               //parse<AnimationMode>(void*, CommandRegistry::ParseToken const&, CommandOrigin const&, int, TextHolder*, std::vector<TextHolder, std::allocator<TextHolder>>&); // 78
	virtual bool canSee(vec3_t const &);                                                                                                       // 79
	virtual bool canSee(const C_Entity *);                                                                                                      // 80
	virtual void isSkyLit(float);                                                                                                            // 81
	virtual void getBrightness(float);                                                                                                       // 82
	virtual void Function82();                                                                                                               //parse<AnimationMode>(void*, CommandRegistry::ParseToken const&, CommandOrigin const&, int, TextHolder*, std::vector<TextHolder, std::allocator<TextHolder>>&); // 83
	virtual void Function84();                                                                                                               // 84
	virtual void onAboveBubbleColumn(bool);                                                                                                  // 85
	virtual void onInsideBubbleColumn(bool);                                                                                                 // 86
	virtual bool isImmobile(void);                                                                                                           // 87
	virtual bool isSilent(void);                                                                                                             // 88
	virtual bool isPickable(void);                                                                                                           // 89
	virtual void Function90(void);                                                                                                           // 90
	virtual void isSleeping(void);                                                                                                           // 91
	virtual void Function92(void);                                                                                                           // 92
	virtual void setSneaking(bool);                                                                                                          // 93
	virtual void isBlocking(void);                                                                                                           // 94
	virtual void isDamageBlocked(class ActorDamageSource const &);                                                                                 // 95
	virtual bool isAlive(void);                                                                                                              // 96
	virtual bool isOnFire(void);                                                                                                             // 97
	virtual void isOnHotBlock(void);                                                                                                         // 98
	virtual void Function99(void);                                                                                                           // 99
	virtual void isSurfaceMob(void);                                                                                                         // 100
	virtual void Function101(void);                                                                                                          // 101
	virtual void Function102();                                                                                                              //parse<AnimationMode>(void*, CommandRegistry::ParseToken const&, CommandOrigin const&, int, TextHolder*, std::vector<TextHolder, std::allocator<TextHolder>>&); // 102
	virtual void Function103(void);                                                                                                          // 103
	virtual void isAffectedByWaterBottle(void);                                                                                              // 104
	virtual bool canAttack(C_Entity *, bool);                                                                                                   // 105
	virtual void setTarget(C_Entity *);                                                                                                         // 106
	virtual void Function107();                                                                                                              //_Func_impl_no_alloc<std::_Binder<std::_Unforced, EventResult(PlayerEventListener::*&)(PlayerSayCommandEvent const&), std::_Ph<1> const&, PlayerSayCommandEvent const&>, EventResult, gsl::not_null<PlayerEventListener*>>::_Move(void*); // 107
	virtual bool isValidTarget(C_Entity *);                                                                                                     // 108
	virtual void attack(C_Entity *);                                                                                                            // 109
	virtual void performRangedAttack(C_Entity *, float);                                                                                        // 110
	virtual void adjustDamageAmount(int &);                                                                                                  // 111
	virtual void getEquipmentCount(void);                                                                                                    // 112
	virtual void setOwner(__int64 entityUniqueId);                                                                                                    // 113
	virtual void setSitting(bool);                                                                                                           // 114
	virtual void Function115();                                                                                                              // 115
	virtual void Function116();                                                                                                              // 116
	virtual void getInventorySize(void);                                                                                                     // 117
	virtual void getEquipSlots(void);                                                                                                        // 118
	virtual void getChestSlots(void);                                                                                                        // 119
	virtual void setStanding(bool);                                                                                                          // 120
	virtual void canPowerJump(void);                                                                                                         // 121
	virtual void setCanPowerJump(bool);                                                                                                      // 122
	virtual void isJumping(void);                                                                                                            // 123
	virtual void isEnchanted(void);                                                                                                          // 124
	virtual void Function125();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 125
	virtual void Function126();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 126
	virtual void shouldRender(void);                                                                                                         // 127
	virtual void isInvulnerableTo(class ActorDamageSource const &);                                                                                // 128
	virtual void getBlockDamageCause(class Block const &);                                                                                   // 129
	virtual void actuallyHurt(int, class ActorDamageSource const &, bool);                                                                         // 130
	virtual void animateHurt(void);                                                                                                          // 131
	virtual void doFireHurt(int);                                                                                                            // 132
	virtual void onLightningHit(void);                                                                                                       // 133
	virtual void onBounceStarted(vec3_ti const &, class Block const &);                                                                     // 134
	virtual void feed(int);                                                                                                                  // 135
	virtual void handleEntityEvent(uintptr_t C_EntityEvent, int);                                                                               // 136
	virtual void getPickRadius(void);                                                                                                        // 137
	virtual void getC_EntityRendererId(void);                                                                                                   // 138
	virtual void spawnAtLocation(C_ItemStack *, float);                                                                                        // 139
	virtual void spawnAtLocation(class Block const &, int, float);                                                                           // 140
	virtual void spawnAtLocation(class Block const &, int);                                                                                  // 141
	virtual void spawnAtLocation(int, int, float);                                                                                           // 142
	virtual void spawnAtLocation(int, int);                                                                                                  // 143
	virtual void despawn(void);                                                                                                              // 144
	virtual void killed(C_Entity *);                                                                                                            // 145
	virtual void awardKillScore(C_Entity *, int);                                                                                               // 146
	virtual void setArmor(uintptr_t ArmorSlot, C_ItemStack *);                                                                                 // 147
	virtual C_ItemStack* getArmor(uintptr_t ArmorSlot);                                                                                              // 148
	virtual void getArmorMaterialTypeInSlot(uintptr_t ArmorSlot);                                                                            // 149
	virtual void getArmorMaterialTextHolderureTypeInSlot(uintptr_t ArmorSlot);                                                                     // 150
	virtual void getArmorColorInSlot(uintptr_t ArmorSlot, int);                                                                              // 151
	virtual void getEquippedSlot(uintptr_t EquipmentSlot);                                                                                   // 152
	virtual void setEquippedSlot(uintptr_t EquipmentSlot, C_ItemStack *);                                                                      // 153
	virtual C_ItemStack* v_getSelectedItem(void);                                                                                                      // 154
	virtual void setCarriedItem(C_ItemStack *);                                                                                                // 155
	virtual void setOffhandSlot(C_ItemStack *);                                                                                                // 156
	virtual C_ItemStack* getEquippedTotem(void);                                                                                                     // 157
	virtual void consumeTotem(void);                                                                                                         // 158
	virtual void save(class CompoundTag *);                                                                                                  // 159
	virtual void saveWithoutId(class CompoundTag *);                                                                                         // 160
	virtual void load(class CompoundTag const &, class DataLoadHelper &);                                                                    // 161
	virtual void loadLinks(class CompoundTag, __int64, DataLoadHelper &);  // 162
	virtual int getEntityTypeId(void);                                                                                                      // 163
	virtual void getRawName(void);                                                                                                           // 164
	virtual __int64* getSourceUniqueID(void);                                                                                                    // 165
	virtual void setOnFire(int);                                                                                                             // 166
	virtual void extinguishFire(void);                                                                                                       // 167
	virtual void thawFreezeEffect(void);                                                                                                     // 168
	virtual bool canFreeze(void);                                                                                                            // 169
	virtual void isWearingLeatherArmor(void);                                                                                                // 170
	virtual void getHandleWaterAABB(void);                                                                                                   // 171
	virtual void handleInsidePortal(vec3_ti const &);                                                                                       // 172
	virtual void getId(void);                                                                                                                // 173
	virtual void getPortalWaitTime(void);                                                                                                    // 174
	virtual int getDimensionId();                                                                                                       // 175
	virtual void canChangeDimensions(void);                                                                                                  // 176
	virtual void Function177();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 177
	virtual void changeDimension(int, bool);                                                                                                 // 178
	virtual void getControllingPlayer(void);                                                                                                 // 179
	virtual void checkFallDamage(float, bool);                                                                                               // 180
	virtual void causeFallDamage(float, float, class ActorDamageSource);                                                                           // 181
	virtual void handleFallDistanceOnServer(float, float, bool);                                                                             // 182
	virtual void playSynchronizedSound(uintptr_t LevelSoundEvent, vec3_t const &, int, bool);                                                  // 183
	virtual void playSynchronizedSound(uintptr_t LevelSoundEvent, vec3_t const &, Block const &, bool);                                        // 184
	virtual void onSynchedDataUpdate(int);                                                                                                   // 185
	virtual void canAddRider(C_Entity *);                                                                                                       // 186
	virtual void Function187(void);                                                                                                          // 187
	virtual void Function188();                                                                                                              //parse<AnimationMode>(void*, CommandRegistry::ParseToken const&, CommandOrigin const&, int, TextHolder*, std::vector<TextHolder, std::allocator<TextHolder>>&); // 188
	virtual void inCaravan(void);                                                                                                            // 189
	virtual void Function190();                                                                                                              //parse<AnimationMode>(void*, CommandRegistry::ParseToken const&, CommandOrigin const&, int, TextHolder*, std::vector<TextHolder, std::allocator<TextHolder>>&); // 190
	virtual void tickLeash(void);                                                                                                            // 191
	virtual void Function192();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 192
	virtual void Function193(void);                                                                                                          // 193
	virtual void stopRiding(bool, bool, bool);                                                                                               // 194
	virtual void startSwimming(void);                                                                                                        // 195
	virtual void stopSwimming(void);                                                                                                         // 196
	virtual void buildDebugInfo(TextHolder *);                                                                                                     // 197
	virtual void getCommandPermissionLevel(void);                                                                                            // 198
	virtual void getMutableAttribute(struct Attribute const &);                                                                                     // 199
	virtual void getAttribute(struct Attribute const &);                                                                                            // 200
	virtual void getDeathTime(void);                                                                                                         // 201
	virtual void heal(int);                                                                                                                  // 202
	virtual void isInvertedHealAndHarm(void);                                                                                                // 203
	virtual void canBeAffected(class MobEffectInstance const &);                                                                                   // 204
	virtual void canBeAffected(int);                                                                                                         // 205
	virtual void canBeAffectedByArrow(class MobEffectInstance const &);                                                                            // 206
	virtual void onEffectAdded(class MobEffectInstance &);                                                                                         // 207
	virtual void onEffectUpdated(class MobEffectInstance const &);                                                                                 // 208
	virtual void onEffectRemoved(class MobEffectInstance &);                                                                                       // 209
	virtual void getAnimationComponent(void);                                                                                                // 210
	virtual void openContainerComponent(C_Entity *);                                                                                     // 211
	virtual void swing(void);                                                                                                                // 212
	virtual void useItem(C_ItemStack *, uintptr_t ItemUseMethod, bool);                                                                        // 213
	virtual void Function214();                                                                                                              //parse<AnimationMode>(void*, CommandRegistry::ParseToken const&, CommandOrigin const&, int, TextHolder*, std::vector<TextHolder, std::allocator<TextHolder>>&); // 214
	virtual void Function215();                                                                                                              //_Func_impl_no_alloc<std::_Binder<std::_Unforced, EventResult(PlayerEventListener::*&)(PlayerSayCommandEvent const&), std::_Ph<1> const&, PlayerSayCommandEvent const&>, EventResult, gsl::not_null<PlayerEventListener*>>::_Move(void*); // 215
	virtual void getDebugTextHolder(uintptr_t vectorTextHolder);                                                                                            // 216
	virtual void getMapDecorationRotation(void);                                                                                             // 217
	virtual void getRiderYRotation(const C_Entity *);                                                                                           // 218
	virtual void getYHeadRot(void);                                                                                                          // 219
	virtual bool isWorldBuilder(void);                                                                                                       // 220
	virtual bool isCreative(void);                                                                                                           // 221
	virtual bool isAdventure(void);                                                                                                          // 222
	virtual void add(C_ItemStack *);                                                                                                           // 223
	virtual void drop(C_ItemStack *, bool);                                                                                                    // 224
	virtual void getInteraction(C_Entity *, uintptr_t C_EntityInteraction, vec3_t const &);                                                   // 225
	virtual void Function226(void);                                                                                                          // 226
	virtual void Function227();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 227
	virtual void setSize(float, float);                                                                                                      // 228
	virtual void getLifeSpan(void);                                                                                                          // 229
	virtual void onOrphan(void);                                                                                                             // 230
	virtual void wobble(void);                                                                                                               // 231
	virtual void wasHurt(void);                                                                                                              // 232
	virtual void startSpinAttack(void);                                                                                                      // 233
	virtual void stopSpinAttack(void);                                                                                                       // 234
	virtual void setDamageNearbyMobs(bool);                                                                                                  // 235
	virtual void Function236();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 236
	virtual void reloadLootTable(struct EquipmentTableDefinition const &);                                                                   // 237
	virtual void reloadLootTable(void);                                                                                                      // 238
	virtual void getLootTable(void);                                                                                                         // 239
	virtual void Function240();                                                                                                              //_Func_impl_no_alloc<std::_Binder<std::_Unforced, EventResult(PlayerEventListener::*&)(PlayerSayCommandEvent const&), std::_Ph<1> const&, PlayerSayCommandEvent const&>, EventResult, gsl::not_null<PlayerEventListener*>>::_Move(void*); // 240
	virtual void getShadowHeightOffs2(void);                                                                                                 // 241
	virtual void kill(void);                                                                                                                 // 242
	virtual void die(class ActorDamageSource const &);                                                                                             // 243
	virtual void shouldTick(void);                                                                                                           // 244
	virtual void createMovementProxy(void);                                                                                                  // 245
	virtual void getMovementProxy(void);                                                                                                     // 246
	virtual void getNextStep(float);                                                                                                         // 247
	virtual void updateEntitySpecificMolangVariables(struct RenderParams &);                                                               // 248
	virtual void useWebsocketEncryption(void);                                                                                               // 249
	virtual void Function250(void);                                                                                                          // 250
	virtual void outOfWorld(void);                                                                                                           // 251
	virtual void _hurt(class ActorDamageSource const &, int, bool, bool);                                                                          // 252
	virtual void markHurt(void);                                                                                                             // 253
	virtual void _getAnimationComponent();                                                                                                   //(std::shared_ptr<AnimationComponent>&, AnimationComponentGroup); // 254
	virtual void readAdditionalSaveData(CompoundTag const &, DataLoadHelper &);                                                              // 255
	virtual void addAdditionalSaveData(CompoundTag &);                                                                                       // 256
	virtual void _playStepSound(vec3_ti const &, Block const &);                                                                            // 257
	virtual void _playFlySound(vec3_ti const &, Block const &);                                                                             // 258
	virtual void Function259();                                                                                                              //parse<AnimationMode>(void*, CommandRegistry::ParseToken const&, CommandOrigin const&, int, TextHolder*, std::vector<TextHolder, std::allocator<TextHolder>>&); // 259
	virtual void checkInsideBlocks(float);                                                                                                   // 260
	virtual void pushOutOfBlocks(vec3_t const &);                                                                                              // 261
	virtual void updateWaterState(void);                                                                                                     // 262
	virtual void doWaterSplashEffect(void);                                                                                                  // 263
	virtual void spawnTrailBubbles(void);                                                                                                    // 264
	virtual void updateInsideBlock(void);                                                                                                    // 265
	virtual void _removeRider(__int64 entityUniqueId, bool, bool, bool);                                                                      // 266
	virtual void _onSizeUpdated(void);                                                                                                       // 267
	virtual void Function268(char const *);                                                                                                  // 268
	virtual void knockback(C_Entity *, int, float, float, float, float, float);                                                                 // 269
	virtual void resolveDeathLoot(int, class ActorDamageSource const &);                                                                           // 270
	virtual void Function271();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 271
	virtual void setSleeping(bool);                                                                                                          // 272
	virtual void setSprinting(bool);                                                                                                         // 273
	virtual void playAmbientSound(void);                                                                                                     // 274
	virtual void getAmbientSound(void);                                                                                                      // 275
	virtual void getHurtSound(void);                                                                                                         // 276
	virtual void getDeathSound(void);                                                                                                        // 277
	virtual void getAmbientSoundPostponeTicks(void);                                                                                         // 278
	virtual void getAmbientSoundPostponeTicksRange(void);                                                                                    // 279
	virtual void getItemInHandIcon(C_ItemStack *, int);                                                                                        // 280
	virtual float* getSpeed(void);                                                                                                            // 281
	virtual void setSpeed(float);                                                                                                            // 282
	virtual void getJumpPower(void);                                                                                                         // 283
	virtual void hurtEffects(class ActorDamageSource const &, int, bool, bool);                                                                    // 284
	virtual void getMeleeWeaponDamageBonus(C_Entity *);                                                                                         // 285
	virtual void getMeleeKnockbackBonus(void);                                                                                               // 286
	virtual void travel(class IMobMovementProxy &, float, float, float);                                                                     // 287
	virtual void travel(float, float, float);                                                                                                // 288
	virtual void applyFinalFriction(float, bool);                                                                                            // 289
	virtual void updateWalkAnim(void);                                                                                                       // 290
	virtual void aiStep(class IMobMovementProxy &);                                                                                          // 291
	virtual void aiStep(void);                                                                                                               // 292
	virtual void pushC_Entitys(void);                                                                                                           // 293
	virtual void lookAt(C_Entity *, float, float);                                                                                              // 294
	virtual bool isLookingAtAnEntity(void);                                                                                                  // 295
	virtual void checkSpawnRules(bool);                                                                                                      // 296
	virtual void checkSpawnObstruction(void);                                                                                                // 297
	virtual void getAttackAnim(float);                                                                                                       // 298
	virtual void getItemUseDuration(void);                                                                                                   // 299
	virtual void getItemUseStartupProgress(void);                                                                                            // 300
	virtual void getItemUseIntervalProgress(void);                                                                                           // 301
	virtual void getItemuseIntervalAxis(void);                                                                                               // 302
	virtual void getTimeAlongSwing(void);                                                                                                    // 303
	virtual void Function304();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 304
	virtual void getMaxHeadXRot(void);                                                                                                       // 305
	virtual void getLastHurtByMob(void);                                                                                                     // 306
	virtual void setLastHurtByMob(C_Entity *);                                                                                           // 307
	virtual C_Entity *getLastHurtByPlayer(void);                                                                                         // 308
	virtual void setLastHurtByPlayer(C_Entity *);                                                                                        // 309
	virtual C_Entity *getLastHurtMob(void);                                                                                              // 310
	virtual void setLastHurtMob(C_Entity *);                                                                                                    // 311
	virtual bool isAlliedTo(C_Entity *);                                                                                                 //parse<AnimationMode>(void*, CommandRegistry::ParseToken const&, CommandOrigin const&, int, TextHolder*, std::vector<TextHolder, std::allocator<TextHolder>>&); // 312
	virtual void doHurtTarget(C_Entity *);                                                                                                      // 313
	virtual void Function314();                                                                                                              //parse<AnimationMode>(void*, CommandRegistry::ParseToken const&, CommandOrigin const&, int, TextHolder*, std::vector<TextHolder, std::allocator<TextHolder>>&); // 314
	virtual void leaveCaravan(void);                                                                                                         // 315
	virtual void joinCaravan(class Mob *);                                                                                                   // 316
	virtual void hasCaravanTail(void);                                                                                                       // 317
	virtual void getCaravanHead(void);                                                                                                       // 318
	virtual void getArmorValue(void);                                                                                                        // 319
	virtual void getArmorCoverPercentage(void);                                                                                              // 320
	virtual void hurtArmor();                                                                                                                //(class ActorDamageSource const&, int, std::bitset<4> const&); // 321
	virtual void hurtArmor(class ActorDamageSource const &, int);                                                                                  // 322
	virtual void hurtArmorSlot(class ActorDamageSource const &, int, uintptr_t ArmorSlot);                                                         // 323
	virtual void setDamagedArmor(uintptr_t ArmorSlot, C_ItemStack *);                                                                          // 324
	virtual void sendArmorDamage();                                                                                                          //(std::bitset<4> const&); // 325
	virtual void sendArmor();                                                                                                                //(std::bitset<4> const&); // 326
	virtual void containerChanged(int);                                                                                                      // 327
	virtual void updateEquipment(void);                                                                                                      // 328
	virtual void clearEquipment(void);                                                                                                       // 329
	virtual void getAllArmor(void);                                                                                                          // 330
	virtual void getAllArmorID(void);                                                                                                        // 331
	virtual void getAllHand(void) const;                                                                                                     // 332
	virtual void getAllEquipment(void);                                                                                                      // 333
	virtual void getArmorTypeHash(void);                                                                                                     // 334
	virtual void dropEquipmentOnDeath(void);                                                                                                 // 335
	virtual void dropEquipmentOnDeath(class ActorDamageSource const &, int);                                                                       // 336
	virtual void clearVanishEnchantedItemsOnDeath(void);                                                                                     // 337
	virtual void sendInventory(bool);                                                                                                        // 338
	virtual void getDamageAfterMagicAbsorb(class ActorDamageSource const &, int);                                                                  // 339
	virtual void createAIGoals(void);                                                                                                        // 340
	virtual void Function342();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 341
	virtual void setItemSlot(uintptr_t EquipmentSlot, C_ItemStack *);                                                                          // 342
	virtual void setTransitioningSitting(bool);                                                                                              // 343
	virtual void attackAnimation(C_Entity *, float);                                                                                            // 344
	virtual void getTickDelay(void);                                                                                                         // 345
	virtual void getShadowHeightOffs8(void);                                                                                                 // 346
	virtual void Function347(void);                                                                                                          // 347
	virtual void Function348();                                                                                                              //parse<AnimationMode>(void*, CommandRegistry::ParseToken const&, CommandOrigin const&, int, TextHolder*, std::vector<TextHolder, std::allocator<TextHolder>>&); // 348
	virtual void ascendLadder(void);                                                                                                         // 349
	virtual void ascendScaffolding(void);                                                                                                    // 350
	virtual void ascendScaffolding(void) const;                                                                                              // 351
	virtual void descendScaffolding(void);                                                                                                   // 352
	virtual void canAscendCurrentBlockByJumping(void);                                                                                       // 353
	virtual void dropContainer(void);                                                                                                        // 354
	virtual void initBodyControl(void);                                                                                                      // 355
	virtual void jumpFromGround(void);                                                                                                       // 357
	virtual void jumpFromGround(class IMobMovementProxy &);                                                                                  // 356
	virtual void Function358();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 358
	virtual void newServerAiStep(void);                                                                                                      // 359
	virtual void Function360();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 360
	virtual void getDamageAfterEnchantReduction(class ActorDamageSource const &, int);                                                             // 361
	virtual void getDamageAfterArmorAbsorb(class ActorDamageSource const &, int);                                                                  // 362
	virtual void dropBags(void);                                                                                                             // 363
	virtual void tickDeath(void);                                                                                                            // 364
	virtual void updateGliding(void);                                                                                                        // 365
	virtual void Function336(void);                                                                                                          // 366
	virtual void prepareRegion(class ChunkSource &);                                                                                         // 367
	virtual void destroyRegion(void);                                                                                                        // 368
	virtual void suspendRegion(void);                                                                                                        // 369
	virtual void resendAllChunks(void);                                                                                                      // 370
	virtual void _fireWillChangeDimension(void);                                                                                             // 371
	virtual void _fireDimensionChanged(void);                                                                                                // 372
	virtual void changeDimensionWithCredits(int);                                                                                            // 373
	virtual void tickWorld(struct Tick const &);                                                                                             // 374
	virtual void Function375();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 375
	virtual void getTickingOffsets(void);                                                                                                    // 376
	virtual void moveView(void);                                                                                                             // 377
	virtual void setName(const TextHolder *);                                                                                                      // 378
	virtual void checkMovementStats(vec3_t const &);                                                                                           // 379
	virtual void Function380();                                                                                                              //parse<AnimationMode>(void*, CommandRegistry::ParseToken const&, CommandOrigin const&, int, TextHolder*, std::vector<TextHolder, std::allocator<TextHolder>>&); // 380
	virtual void Function381(void);                                                                                                          // 381
	virtual void respawn(void);                                                                                                              // 382
	virtual void Function383();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 383
	virtual void resetPos(bool);                                                                                                             // 384
	virtual void Function385();                                                                                                              //parse<AnimationMode>(void*, CommandRegistry::ParseToken const&, CommandOrigin const&, int, TextHolder*, std::vector<TextHolder, std::allocator<TextHolder>>&); // 385
	virtual void hasResource(int);                                                                                                           // 386
	virtual void completeUsingItem(void);                                                                                                    // 387
	virtual void setPermissions(int CommandPermissionLevel);                                                                                     // 388
	virtual void startDestroying(void);                                                                                                      // 389
	virtual void stopDestroying(void);                                                                                                       // 390
	virtual void Function391();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 391
	virtual void Function392();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 392
	virtual void openTrading(__int64 entityUniqueId, bool);                                                                                   // 393
	virtual bool canOpenContainerScreen(void);                                                                                               // 394
	virtual void Function395();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 395
	virtual void Function396();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 396
	virtual void openInventory(void);                                                                                                        // 397
	virtual void Function398();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 398
	virtual void Function399();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 399
	virtual void displayTextHolderObjectMessage(struct TextHolderObjectRoot const &);                                                                    // 400
	virtual void displayTextHolderObjectWhisperMessage(struct ResolvedTextHolderObject const &, const TextHolder *, const TextHolder *);                             // 401
	virtual void displayWhisperMessage(const TextHolder *, const TextHolder *, const TextHolder *, const TextHolder *);                                              // 402
	virtual void startSleepInBed(vec3_ti const &);                                                                                          // 403
	virtual void stopSleepInBed(bool, bool);                                                                                                 // 404
	virtual void canStartSleepInBed(void);                                                                                                   // 405
	virtual void getSleepTimer(void);                                                                                                        // 406
	virtual void getPreviousTickSleepTimer(void);                                                                                            // 407
	virtual void Function408();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 408
	virtual void Function409();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 409
	virtual bool isHostingPlayer(void);                                                                                                      // 410
	virtual bool isLoading(void);                                                                                                            // 411
	virtual bool isPlayerInitialized(void);                                                                                                  // 412
	virtual void Function413();                                                                                                              //Profile::ProfileClient::onThreadCreate(char const*); // 413
	virtual void registerTrackedBoss(__int64 entityUniqueId);                                                                                         // 414
	virtual void unRegisterTrackedBoss(__int64 entityUniqueId);                                                                                       // 415
	virtual void setPlayerGameType(int GameType);                                                                                            // 416
	virtual void initHUDContainerManager(void);                                                                                              // 417
	virtual void _crit(C_Entity *);                                                                                                             // 418
	virtual void getEventing(void);                                                                                                          // 419
	virtual void Function420();                                                                                                              //_Func_impl_no_alloc<std::_Binder<std::_Unforced, EventResult(PlayerEventListener::*&)(PlayerSayCommandEvent const&), std::_Ph<1> const&, PlayerSayCommandEvent const&>, EventResult, gsl::not_null<PlayerEventListener*>>::_Move(void*); // 420
	virtual void sendEventC_Packet(class EventC_Packet &);                                                                                       // 421
	virtual void addExperience(int);                                                                                                         // 422
	virtual void addLevels(int);                                                                                                             // 423
	virtual void setContainerData(class IContainerManager &, int, int);                                                                      // 424
	virtual void slotChanged(class IContainerManager &, class Container &, int, C_ItemStack *, C_ItemStack *, bool);                             // 425
	virtual void inventoryChanged(class Container &, int, C_ItemStack *, C_ItemStack *, bool);                                                   // 426
	virtual void refreshContainer(class IContainerManager &);                                                                                // 427
	virtual void deleteContainerManager(void);                                                                                               // 428
	virtual void setFieldOfViewModifier(float);                                                                                              // 429
	virtual void is2DPositionRelevant(int dimensionID, vec3_ti const &);                                                                    // 430
	virtual void isEntityRelevant(const C_Entity *);                                                                                            // 431
	virtual void isTeacher(void);                                                                                                            // 432
	virtual void onSuspension(void);                                                                                                         // 433
	virtual void onLinkedSlotsChanged(void);                                                                                                 // 434
	virtual void startCooldown(class Item const *);                                                                                          // 435
	virtual void getItemCooldownLeft(const TextHolder *);                                                                                          // 436
	virtual void isItemInCooldown(const TextHolder *);                                                                                             // 437
	virtual void sendInventoryTransaction(class InventoryTransaction const &);                                                               // 438
	virtual void sendComplexInventoryTransaction();                                                                                          //(std::unique_ptr<class ComplexInventoryTransaction, std::default_delete<class ComplexInventoryTransaction>>); // 439
	virtual void sendNetworkC_Packet(class C_Packet &);                                                                                                // 440
	virtual void getPlayerEventCoordinator(void);                                                                                            // 441
	virtual void getMoveInputHandler(void);                                                                                                  // 442
	virtual void getInputMode(void);                                                                                                         // 443
	virtual void getPlayMode(void);                                                                                                          // 444
	virtual void reportMovementTelemetry(uintptr_t MovementEventType);                                                                       // 445
	virtual void onMovePlayerC_PacketNormal(vec3_t const &, vec2_t const &, float);                                                                // 446

public:
	C_InventoryTransactionManager *getTransactionManager();

	AABB *getAABB() {
		return &this->aabb;
	}

	__int64* getUniqueId() {
		uintptr_t *v1;  // rbx
		char v3;     // [rsp+30h] [rbp+8h]

		v1 = (uintptr_t *)((char *)this + 264);
		if (*((uintptr_t *)this + 33) == -1i64)
			*v1 = *(uintptr_t *)(*(__int64(__fastcall **)(uintptr_t, char *))(**((uintptr_t **)this + 111) + 1848i64))(
				*((uintptr_t *)this + 111),
				&v3);
		return (__int64 *)v1;
	}

	int getTicksUsingItem() {
		return *reinterpret_cast<int *>(reinterpret_cast<__int64>(this) + 0x1098); //0x10A8 for remaining ticks
	}

	bool isSneaking() {
		return false;  // TODO
	}

	bool isSprinting() {
		return false;  // TODO
	}
};

#pragma pack(pop)


class C_Player : public C_Entity {
public:
	C_PlayerInventoryProxy *getSupplies();
	

	C_ItemStack *getSelectedItem() {
		auto supplies = this->getSupplies();
		return supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
	}

	int getSelectedItemId() {
		auto item = getSelectedItem();
		if (item == nullptr || item->item == nullptr)
			return 0;
		if (item->getItem() == nullptr)
			return 0;
		return item->getItem()->itemId;
	}

};


class C_LocalPlayer : public C_Player {
public:
	void unlockAchievments();

	void swingArm() {
		this->swing();
	}

	/*void localPlayerTurn(vec2_t* viewAngles) {
		using Turn = void(__thiscall*)(void*, vec2_t*);
		static Turn TurnFunc = reinterpret_cast<Turn>(FindSignature("4C 8B DC 55 57 ?? ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 45 ?? ?? ?? ?? ?? ?? ?? 48 8B 05 D6 D4 A5 01 48 33 C4 48 89 45 ?? 49 89 5B ?? 48 8B F9 ?? ?? ?? ?? ?? ?? ?? 48 8B DA 4D 89 73 E8 41 0F 29 73 D8 41 0F 29 7B C8 48 8B 01 FF 90 ?? ?? ?? ?? F2 0F 10 03"));
		TurnFunc(this, viewAngles);
	}*/

	void applyTurnDelta(vec2_t *viewAngleDelta);
	void setGameModeType(int gma);
};