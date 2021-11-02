#pragma once

#include <bitset>

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "CBlockLegacy.h"
#include "CEntityList.h"
#include "CInventory.h"
#include "CInventoryTransaction.h"
#include "TextHolder.h"

class C_GameMode;

class PointingStruct {
	char pad_0x0000[0xA28];  //0x0000
public:
	int rayHitType;    //0x08E0
	int blockSide;     //0x08E4
	vec3_ti block;     //0x08E8
	vec3_t rayHitVec;  //0x08F4
private:
	C_Entity *entityPtr;          //0x0900
	C_Entity *entityPtr2;         //0x0908
	uint64_t GamingEntityFinder;  //0x0910

public:
	bool hasEntity();       //to not go trough the entity list twice
	C_Entity *getEntity();  //returns the entity that the player is looking at
};

class C_Player;
struct MobEffectInstance;

#pragma pack(push, 4)
class C_Entity {
public:
public:
	uint64_t ptrToEntityList;  //0x0008
private:
	char pad_0010[296];  //0x0010
public:
	union {
		struct {
			float pitch;  //0x0138
			float yaw;    //0x013C
		};
		vec2_t viewAngles;
	};
	union {
		struct {
			float pitch2;  //0x0138
			float yaw2;    //0x013C
		};
		vec2_t viewAngles2;
	};

private:
	char pad_0148[16];  //0x0148
public:
	vec3_t eyePos0;  //0x0158
private:
	char pad_0164[112];  //0x0164
public:
	float fallDistance;  //0x01D4
	bool onGround;       //0x01D8
private:
	char pad_01D9[95];  //0x01D9
public:
	float stepHeight;  //0x0238
private:
	char pad_023C[16];  //0x023C
public:
	vec3_t slowdownFactor;  //0x024C
private:
	char pad_0258[5];  //0x0258
public:
	bool didEnterWaterBool;  //0x025D
private:
	char pad_025E[1];  //0x025E
public:
	bool hasEnteredWaterBool;  //0x025F
private:
	char pad_0260[72];  //0x0260
public:
	int32_t ticksAlive;  //0x02A8
private:
	char pad_02AC[180];  //0x02AC
public:
	class C_BlockSource *region;  //0x0360
private:
	char pad_0368[8];  //0x0368
public:
	class PointingStruct *pointingStruct;  //0x0370
private:
	char pad_0378[328];  //0x0378
public:
	AABB aabb;          //0x04C0
	float width;        //0x04D8
	float height;       //0x04DC
	vec3_t currentPos;  //0x04E0
	vec3_t oldPos;      //0x04EC
	vec3_t velocity;    //0x04F8
	vec3_t velocity2;   //0x0504
private:
	char pad_0510[80];  //0x0510
public:
	int64_t entityRuntimeId;  //0x0560
private:
	char pad_0568[480];  //0x0568
public:
	float bodyYaw;                    //0x0748
	float oldBodyYaw;                 //0x074C
	float yawUnused1;                 //0x0750
	float yawUnused2;                 //0x0754
	int32_t damageTime;               //0x0758
	int32_t damageAnimationDuration;  //0x075C
private:
	char pad_0760[136];  //0x0760
public:
	int32_t timeSinceDeath;  //0x07E8
private:
	char pad_07EC[244];  //0x07EC
public:
	class TextHolder playerName;  //0x08E0
private:
	char pad_0900[140];  //0x0900
public:
	bool canFly;  //0x098C
private:
	char pad_098D[1667];  //0x098D
public:
	int32_t ticksUsingItem;  //0x1010
private:
	char pad_1014[20];  //0x1014
public:
	int16_t itemData;  //0x1028
	int16_t itemId;    //0x102A
private:
	char pad_102C[508];  //0x102C
public:
	class C_InventoryTransactionManager transac;  //0x1228
private:
	char pad_1288[2812];  //0x1288
public:
	int32_t gamemode;  //0x1D84

	virtual __int64 getLastHurtByMob(void);                                                                                // 0
	virtual __int64 setLastHurtByMob(C_Entity *);                                                                          // 1
	virtual __int64 getLastHurtByPlayer(void);                                                                             // 2
	virtual __int64 setLastHurtByPlayer(C_Player *);                                                                       // 3
	virtual __int64 getLastHurtMob(void);                                                                                  // 4
	virtual __int64 setLastHurtMob(C_Entity *);                                                                            // 5
	virtual __int64 outOfWorld(void);                                                                                      // 6
	virtual __int64 reloadHardcoded(__int64, __int64);                                                                     // 7
	virtual __int64 reloadHardcodedClient(__int64, __int64);                                                               // 8
	virtual __int64 initializeComponents(__int64, __int64);                                                                // 9
	virtual __int64 reloadComponents(__int64, __int64);                                                                    // 10
	virtual __int64 _serverInitItemStackIds(void);                                                                         // 11
	virtual __int64 _doInitialMove(void);                                                                                  // 12
	virtual __int64 hasComponent(class HashedString const &);                                                              // 13
	virtual void destructor();                                                                                             // 14
	virtual __int64 reset(void);                                                                                           // 15
	virtual __int64 getOnDeathExperience(void);                                                                            // 16
	virtual __int64 getOwnerEntityType(void);                                                                              // 17
	virtual __int64 remove(void);                                                                                          // 18
	virtual __int64 setPos(vec3_t const &);                                                                                // 19
	virtual __int64 isRuntimePredictedMovementEnabled(void);                                                               // 20
	virtual __int64 getPredictedMovementValues(void);                                                                      // 21
	virtual vec3_t *getPos(void);                                                                                          // 22
	virtual vec3_t *getPosOld(void);                                                                                       // 23
	virtual vec3_t *getPosExtrapolated(float);                                                                             // 24
	virtual vec3_t *getAttachPos(struct ActorLocation, float);                                                             // 25
	virtual vec3_t *getFiringPos(void);                                                                                    // 26
	virtual __int64 setRot(vec2_t const &);                                                                                // 27
	virtual __int64 move(vec3_t const &);                                                                                  // 28
	virtual __int64 move(__int64 IActorMovementProxy, vec3_t const &);                                                     // 29
	virtual __int64 getInterpolatedRidingPosition(float);                                                                  // 30
	virtual __int64 getInterpolatedBodyRot(float);                                                                         // 31
	virtual __int64 getInterpolatedHeadRot(float);                                                                         // 32
	virtual __int64 getInterpolatedBodyYaw(float);                                                                         // 33
	virtual __int64 getYawSpeedInDegreesPerSecond(void);                                                                   // 34
	virtual __int64 getInterpolatedWalkAnimSpeed(float);                                                                   // 35
	virtual __int64 getInterpolatedRidingOffset(float);                                                                    // 36
	virtual __int64 checkBlockCollisions(AABB const &, __int64);                                                           // 37
	virtual __int64 updateEntityInside(AABB const &);                                                                      // 38
	virtual __int64 updateEntityInside(void);                                                                              // 39
	virtual __int64 isFireImmune(void);                                                                                    // 40
	virtual __int64 breaksFallingBlocks(void);                                                                             // 41
	virtual __int64 blockedByShield(__int64, C_Entity *);                                                                  // 42
	virtual __int64 teleportTo(vec3_t const &, bool, int, int);                                                            // 43
	virtual __int64 tryTeleportTo(vec3_t const &, bool, bool, int, int);                                                   // 44
	virtual __int64 chorusFruitTeleport(vec3_t const &);                                                                   // 45
	virtual __int64 lerpMotion(vec3_t const &);                                                                            // 46
	virtual __int64 tryCreateAddActorPacket(void);                                                                         // 47
	virtual __int64 normalTick(void);                                                                                      // 48
	virtual __int64 baseTick(void);                                                                                        // 49
	virtual __int64 vehicleTick(void);                                                                                     // 50
	virtual __int64 positionPassenger(C_Entity *, float);                                                                  // 51
	virtual __int64 getRidingHeight(void);                                                                                 // 52
	virtual __int64 startRiding(C_Entity &);                                                                               // 53
	virtual __int64 addPassenger(C_Entity &);                                                                              // 54
	virtual __int64 flagPassengerToRemove(C_Entity &);                                                                     // 55
	virtual __int64 getExitTip(const TextHolder &, int InputMode);                                                         // 56
	virtual __int64 intersects(vec3_t const &, vec3_t const &);                                                            // 57
	virtual __int64 isInWall(void);                                                                                        // 58
	virtual __int64 isInvisible(void);                                                                                     // 59
	virtual __int64 canShowNameTag(void);                                                                                  // 60
	virtual __int64 canExistInPeaceful(void);                                                                              // 61
	virtual __int64 setNameTagVisible(bool);                                                                               // 62
	virtual TextHolder *getNameTag(void);                                                                                  // 63
	virtual __int64 getNameTagAsHash(void);                                                                                // 64
	virtual TextHolder *getFormattedNameTag(void);                                                                         // 65
	virtual __int64 filterFormattedNameTag(class UIProfanityContext const &);                                              // 66
	virtual __int64 setNameTag(const TextHolder &);                                                                        // 67
	virtual __int64 getAlwaysShowNameTag(void);                                                                            // 68
	virtual __int64 setScoreTag(const TextHolder &);                                                                       // 69
	virtual __int64 getScoreTag(void);                                                                                     // 70
	virtual bool isInWater(void);                                                                                       // 71
	virtual bool hasEnteredWater(void);                                                                                 // 72
	virtual bool isInLava(void);                                                                                        // 73
	virtual bool isUnderLiquid(int materialType);                                                                       // 74
	virtual bool isOverWater(void);                                                                                     // 75
	virtual __int64 setBlockMovementSlowdownMultiplier(vec3_t const &);                                                    // 76
	virtual __int64 resetBlockMovementSlowdownMultiplier(void);                                                            // 77
	virtual __int64 getCameraOffset(void);                                                                                 // 78
	virtual __int64 getShadowHeightOffs(void);                                                                             // 79
	virtual __int64 getShadowRadius(void);                                                                                 // 80
	virtual __int64 getHeadLookVector(float);                                                                              // 81
	virtual __int64 canSeeInvisible(void);                                                                                 // 82
	virtual __int64 canSee(C_Entity const &);                                                                              // 83
	virtual __int64 canSee(vec3_t const &);                                                                                // 84
	virtual __int64 isSkyLit(float);                                                                                       // 85
	virtual __int64 getBrightness(float);                                                                                  // 86
	virtual __int64 interactPreventDefault(void);                                                                          // 87
	virtual __int64 playerTouch(C_Player &);                                                                               // 88
	virtual __int64 onAboveBubbleColumn(bool);                                                                             // 89
	virtual __int64 onInsideBubbleColumn(bool);                                                                            // 90
	virtual __int64 isImmobile(void);                                                                                      // 91
	virtual __int64 isSilent(void);                                                                                        // 92
	virtual __int64 isPickable(void);                                                                                      // 93
	virtual __int64 isFishable(void);                                                                                      // 94
	virtual __int64 isSleeping(void);                                                                                      // 95
	virtual __int64 setSleeping(bool);                                                                                     // 96
	virtual __int64 isShootable(void);                                                                                     // 97
	virtual __int64 setSneaking(bool);                                                                                     // 98
	virtual __int64 isBlocking(void);                                                                                      // 99
	virtual __int64 isDamageBlocked(__int64);                                                                              // 100
	virtual __int64 isAlive(void);                                                                                         // 101
	virtual __int64 isOnFire(void);                                                                                        // 102
	virtual __int64 isOnHotBlock(void);                                                                                    // 103
	virtual __int64 isCreativeModeAllowed(void);                                                                           // 104
	virtual __int64 isSurfaceMob(void);                                                                                    // 105
	virtual __int64 isTargetable(void);                                                                                    // 106
	virtual __int64 isLocalPlayer(void);                                                                                   // 107
	virtual __int64 isPlayer(void);                                                                                        // 108
	virtual __int64 isAffectedByWaterBottle(void);                                                                         // 109
	virtual __int64 canAttack(C_Entity *, bool);                                                                           // 110
	virtual __int64 setTarget(C_Entity *);                                                                                 // 111
	virtual __int64 findAttackTarget(void);                                                                                // 112
	virtual __int64 isValidTarget(C_Entity *);                                                                             // 113
	virtual __int64 attack(C_Entity &, __int64 ActorDamageCause);                                                          // 114
	virtual __int64 performRangedAttack(C_Entity &, float);                                                                // 115
	virtual __int64 adjustDamageAmount(int &);                                                                             // 116
	virtual __int64 getEquipmentCount(void);                                                                               // 117
	virtual __int64 setOwner(__int64);                                                                                     // 118
	virtual __int64 setSitting(bool);                                                                                      // 119
	virtual __int64 onTame(void);                                                                                          // 120
	virtual __int64 onFailedTame(void);                                                                                    // 121
	virtual __int64 getInventorySize(void);                                                                                // 122
	virtual __int64 getEquipSlots(void);                                                                                   // 123
	virtual __int64 getChestSlots(void);                                                                                   // 124
	virtual __int64 setStanding(bool);                                                                                     // 125
	virtual __int64 canPowerJump(void);                                                                                    // 126
	virtual __int64 setCanPowerJump(bool);                                                                                 // 127
	virtual __int64 isJumping(void);                                                                                       // 128
	virtual __int64 isEnchanted(void);                                                                                     // 129
	virtual __int64 vehicleJumped(void);                                                                                   // 130
	virtual __int64 vehicleLanded(vec3_t const &, vec3_t const &);                                                         // 131
	virtual __int64 shouldRender(void);                                                                                    // 132
	virtual __int64 playAmbientSound(void);                                                                                // 133
	virtual __int64 getAmbientSound(void);                                                                                 // 134
	virtual __int64 isInvulnerableTo(__int64);                                                                             // 135
	virtual __int64 getBlockDamageCause(C_Block const &);                                                                  // 136
	virtual __int64 actuallyHurt(int, __int64, bool);                                                                      // 137
	virtual __int64 animateHurt(void);                                                                                     // 138
	virtual __int64 doFireHurt(int);                                                                                       // 139
	virtual __int64 onLightningHit(void);                                                                                  // 140
	virtual __int64 onBounceStarted(vec3_ti const &, C_Block const &);                                                     // 141
	virtual __int64 feed(int);                                                                                             // 142
	virtual __int64 handleEntityEvent(struct ActorEvent, int);                                                             // 143
	virtual __int64 getPickRadius(void);                                                                                   // 144
	virtual __int64 getActorRendererId(void);                                                                              // 145
	virtual __int64 spawnAtLocation(int, int);                                                                             // 146
	virtual __int64 spawnAtLocation(int, int, float);                                                                      // 147
	virtual __int64 spawnAtLocation(C_Block const &, int);                                                                 // 148
	virtual __int64 spawnAtLocation(C_Block const &, int, float);                                                          // 149
	virtual __int64 spawnAtLocation(C_ItemStack const &, float);                                                           // 150
	virtual __int64 despawn(void);                                                                                         // 151
	virtual __int64 killed(C_Entity &);                                                                                    // 152
	virtual __int64 awardKillScore(C_Entity &, int);                                                                       // 153
	virtual __int64 setArmor(int ArmorSlot, C_ItemStack const &);                                                          // 154
	virtual C_ItemStack *getArmor(int ArmorSlot);                                                                          // 155
	virtual __int64 getArmorMaterialTypeInSlot(int ArmorSlot);                                                             // 156
	virtual __int64 getArmorMaterialTextureTypeInSlot(int ArmorSlot);                                                      // 157
	virtual C_ItemStack *getArmorColorInSlot(int ArmorSlot, int);                                                          // 158
	virtual C_ItemStack *getEquippedSlot(int EquipmentSlot);                                                               // 159
	virtual __int64 setEquippedSlot(int EquipmentSlot, C_ItemStack const *);                                               // 160
	virtual C_ItemStack *getCarriedItem(void);                                                                             // 161
	virtual __int64 setCarriedItem(C_ItemStack const *);                                                                   // 162
	virtual __int64 setOffhandSlot(C_ItemStack const *);                                                                   // 163
	virtual C_ItemStack *getEquippedTotem(void);                                                                           // 164
	virtual __int64 consumeTotem(void);                                                                                    // 165
	virtual __int64 save(CompoundTag *);                                                                                   // 166
	virtual __int64 saveWithoutId(CompoundTag &);                                                                          // 167
	virtual __int64 load(CompoundTag const &, class DataLoadHelper &);                                                     // 168
	virtual __int64 loadLinks(CompoundTag const &, __int64, __int64);                                                      // 169
	virtual __int64 getEntityTypeId(void);                                                                                 // 170
	virtual __int64 queryEntityRenderer(void);                                                                             // 171
	virtual __int64 getSourceUniqueID(void);                                                                               // 172
	virtual __int64 setOnFire(int);                                                                                        // 173
	virtual __int64 extinguishFire(void);                                                                                  // 174
	virtual __int64 thawFreezeEffect(void);                                                                                // 175
	virtual __int64 canFreeze(void);                                                                                       // 176
	virtual __int64 isWearingLeatherArmor(void);                                                                           // 177
	virtual __int64 getLiquidAABB(int MaterialType);                                                                       // 178
	virtual __int64 handleInsidePortal(vec3_ti const &);                                                                   // 179
	virtual __int64 getPortalCooldown(void);                                                                               // 180
	virtual __int64 getPortalWaitTime(void);                                                                               // 181
	virtual __int64 getDimensionId(int *out);                                                                              // 182
	virtual __int64 canChangeDimensions(void);                                                                             // 183
	virtual __int64 changeDimension(int, bool);                                                                            // 184
	virtual __int64 changeDimension(class ChangeDimensionPacket const &);                                                  // 185
	virtual __int64 getControllingPlayer(void);                                                                            // 186
	virtual __int64 checkFallDamage(float, bool);                                                                          // 187
	virtual __int64 causeFallDamage(float, float, __int64 ActorDamageSource);                                              // 188
	virtual __int64 handleFallDistanceOnServer(float, float, bool);                                                        // 189
	virtual __int64 playSynchronizedSound(struct LevelSoundEvent, vec3_t const &, C_Block const &, bool);                  // 190
	virtual __int64 playSynchronizedSound(struct LevelSoundEvent, vec3_t const &, int, bool);                              // 191
	virtual __int64 onSynchedDataUpdate(int);                                                                              // 192
	virtual __int64 canAddPassenger(C_Entity &);                                                                           // 193
	virtual __int64 canPickupItem(C_ItemStack const &);                                                                    // 194
	virtual __int64 canBePulledIntoVehicle(void);                                                                          // 195
	virtual __int64 inCaravan(void);                                                                                       // 196
	virtual __int64 isLeashableType(void);                                                                                 // 197
	virtual __int64 tickLeash(void);                                                                                       // 198
	virtual __int64 sendMotionPacketIfNeeded(void);                                                                        // 199
	virtual __int64 canSynchronizeNewEntity(void);                                                                         // 200
	virtual __int64 stopRiding(bool, bool, bool);                                                                          // 201
	virtual __int64 startSwimming(void);                                                                                   // 202
	virtual __int64 stopSwimming(void);                                                                                    // 203
	virtual __int64 buildDebugInfo(TextHolder &);                                                                          // 204
	virtual __int64 getCommandPermissionLevel(void);                                                                       // 205
	virtual __int64 isClientSide(void);                                                                                    // 206
	virtual __int64 getMutableAttribute(struct Attribute const &);                                                         // 207
	virtual __int64 getAttribute(struct Attribute const &);                                                                // 208
	virtual __int64 getDeathTime(void);                                                                                    // 209
	virtual __int64 heal(int);                                                                                             // 210
	virtual __int64 isInvertedHealAndHarm(void);                                                                           // 211
	virtual __int64 canBeAffected(int);                                                                                    // 212
	virtual __int64 canBeAffected(struct MobEffectInstance const &);                                                       // 213
	virtual __int64 canBeAffectedByArrow(struct MobEffectInstance const &);                                                // 214
	virtual __int64 onEffectAdded(MobEffectInstance &);                                                                    // 215
	virtual __int64 onEffectUpdated(MobEffectInstance &);                                                                  // 216
	virtual __int64 onEffectRemoved(MobEffectInstance &);                                                                  // 217
	virtual __int64 getAnimationComponent(void);                                                                           // 218
	virtual __int64 openContainerComponent(C_Player &);                                                                    // 219
	virtual __int64 swing(void);                                                                                           // 220
	virtual __int64 useItem(C_ItemStack &, int ItemUseMethod, bool);                                                       // 221
	virtual __int64 hasOutputSignal(unsigned char);                                                                        // 222
	virtual __int64 getOutputSignal(void);                                                                                 // 223
	virtual __int64 getDebugText(std::vector<TextHolder>);                                                                 // 224
	virtual __int64 getMapDecorationRotation(void);                                                                        // 225
	virtual __int64 getPassengerYRotation(C_Entity const &);                                                               // 226
	virtual __int64 getYHeadRot(void);                                                                                     // 227
	virtual __int64 isWorldBuilder(void);                                                                                  // 228
	virtual __int64 isCreative(void);                                                                                      // 229
	virtual __int64 isAdventure(void);                                                                                     // 230
	virtual __int64 add(C_ItemStack &);                                                                                    // 231
	virtual __int64 drop(C_ItemStack const &, bool);                                                                       // 232
	virtual __int64 getInteraction(C_Player &, __int64 ActorInteraction, vec3_t const &);                                  // 233
	virtual __int64 canDestroyBlock(C_Block const &);                                                                      // 234
	virtual __int64 setAuxValue(int);                                                                                      // 235
	virtual __int64 setSize(float, float);                                                                                 // 236
	virtual __int64 onOrphan(void);                                                                                        // 237
	virtual __int64 wobble(void);                                                                                          // 238
	virtual __int64 wasHurt(void);                                                                                         // 239
	virtual __int64 startSpinAttack(void);                                                                                 // 240
	virtual __int64 stopSpinAttack(void);                                                                                  // 241
	virtual __int64 setDamageNearbyMobs(bool);                                                                             // 242
	virtual __int64 renderDebugServerState(struct Options const &);                                                        // 243
	virtual __int64 reloadLootTable(void);                                                                                 // 244
	virtual __int64 reloadLootTable(struct EquipmentTableDefinition const &);                                              // 245
	virtual __int64 getDeletionDelayTimeSeconds(void);                                                                     // 246
	virtual __int64 kill(void);                                                                                            // 247
	virtual __int64 die(__int64);                                                                                          // 248
	virtual __int64 shouldDropDeathLoot(void);                                                                             // 249
	virtual __int64 shouldTick(void);                                                                                      // 250
	virtual __int64 extractSnapshot(struct PlayerSnapshotComponent &);                                                     // 251
	virtual __int64 applySnapshot(struct PlayerSnapshotComponent const &, struct PlayerSnapshotComponent const &);         // 252
	virtual __int64 getNextStep(float);                                                                                    // 253
	virtual __int64 getLootTable(void);                                                                                    // 254
	virtual __int64 interpolatorTick(void);                                                                                // 255
	virtual __int64 updateEntitySpecificMolangVariables(struct RenderParams &);                                            // 256
	virtual __int64 shouldTryMakeStepSound(void);                                                                          // 257
	virtual __int64 canMakeStepSound(void);                                                                                // 258
	virtual __int64 _hurt(__int64, int, bool, bool);                                                                       // 259
	virtual __int64 markHurt(void);                                                                                        // 260
	virtual __int64 _getAnimationComponent(std::shared_ptr<struct AnimationComponent> &, struct AnimationComponentGroup);  // 261
	virtual __int64 readAdditionalSaveData(CompoundTag const &, DataLoadHelper &);                                         // 262
	virtual __int64 addAdditionalSaveData(CompoundTag &);                                                                  // 263
	virtual __int64 _playStepSound(vec3_ti const &, C_Block const &);                                                      // 264
	virtual __int64 _playFlySound(vec3_ti const &, C_Block const &);                                                       // 265
	virtual __int64 _makeFlySound(void);                                                                                   // 266
	virtual __int64 checkInsideBlocks(float);                                                                              // 267
	virtual __int64 pushOutOfBlocks(vec3_t const &);                                                                       // 268
	virtual __int64 updateWaterState(void);                                                                                // 269
	virtual __int64 doWaterSplashEffect(void);                                                                             // 270
	virtual __int64 spawnTrailBubbles(void);                                                                               // 271
	virtual __int64 updateInsideBlock(void);                                                                               // 272
	virtual __int64 _removePassenger(__int64 uniqueid, bool, bool, bool);                                                  // 273
	virtual __int64 _onSizeUpdated(void);                                                                                  // 274
	virtual __int64 _doAutoAttackOnTouch(C_Entity &);                                                                      // 275
	virtual __int64 knockback(C_Entity *, int, float, float, float, float, float);                                         // 276
	virtual __int64 spawnAnim(void);                                                                                       // 277
	virtual __int64 setSprinting(bool);                                                                                    // 278
	virtual __int64 getHurtSound(void);                                                                                    // 279
	virtual __int64 getDeathSound(void);                                                                                   // 280
	virtual __int64 getItemInHandIcon(C_ItemStack const &, int);                                                           // 281
	virtual __int64 getSpeed(void);                                                                                        // 282
	virtual __int64 setSpeed(float);                                                                                       // 283
	virtual __int64 getJumpPower(void);                                                                                    // 284
	virtual __int64 hurtEffects(__int64, int, bool, bool);                                                                 // 285
	virtual __int64 getMeleeWeaponDamageBonus(C_Entity *);                                                                 // 286
	virtual __int64 getMeleeKnockbackBonus(void);                                                                          // 287
	virtual __int64 travel(float, float, float);                                                                           // 288
	virtual __int64 travel(__int64 IMobMovementProxy, float, float, float);                                                // 289
	virtual __int64 applyFinalFriction(float, bool);                                                                       // 290
	virtual __int64 updateWalkAnim(void);                                                                                  // 291
	virtual __int64 aiStep(void);                                                                                          // 292
	virtual __int64 aiStep(__int64 IMobMovementProxy);                                                                     // 293
	virtual __int64 pushActors(void);                                                                                      // 294
	virtual __int64 lookAt(C_Entity *, float, float);                                                                      // 295
	virtual __int64 isLookingAtAnEntity(void);                                                                             // 296
	virtual __int64 checkSpawnRules(bool);                                                                                 // 297
	virtual __int64 checkSpawnObstruction(void);                                                                           // 298
	virtual __int64 getAttackAnim(float);                                                                                  // 299
	virtual __int64 getItemUseDuration(void);                                                                              // 300
	virtual __int64 getItemUseStartupProgress(void);                                                                       // 301
	virtual __int64 getItemUseIntervalProgress(void);                                                                      // 302
	virtual __int64 getItemUseIntervalAxis(void);                                                                          // 303
	virtual __int64 getTimeAlongSwing(void);                                                                               // 304
	virtual __int64 ate(void);                                                                                             // 305
	virtual __int64 getMaxHeadXRot(void);                                                                                  // 306
	virtual __int64 isAlliedTo(C_Entity *);                                                                                // 307
	virtual __int64 doHurtTarget(C_Entity *, __int64 ActorDamageCause);                                                    // 308
	virtual __int64 canBeControlledByPassenger(void);                                                                      // 309
	virtual __int64 leaveCaravan(void);                                                                                    // 310
	virtual __int64 joinCaravan(C_Entity *);                                                                               // 311
	virtual __int64 hasCaravanTail(void);                                                                                  // 312
	virtual __int64 getCaravanHead(void);                                                                                  // 313
	virtual __int64 getArmorValue(void);                                                                                   // 314
	virtual __int64 getArmorCoverPercentage(void);                                                                         // 315
	virtual __int64 hurtArmorSlots(__int64, int, std::bitset<4ul> const &);                                                // 316
	virtual __int64 setDamagedArmor(int ArmorSlot, C_ItemStack const &);                                                   // 317
	virtual __int64 sendArmorDamage(std::bitset<4ul> const &);                                                             // 318
	virtual __int64 sendArmor(std::bitset<4ul> const &);                                                                   // 319
	virtual __int64 containerChanged(int);                                                                                 // 320
	virtual __int64 updateEquipment(void);                                                                                 // 321
	virtual __int64 clearEquipment(void);                                                                                  // 322
	virtual __int64 getAllArmor(void);                                                                                     // 323
	virtual __int64 getAllArmorID(void);                                                                                   // 324
	virtual __int64 getAllHand(void);                                                                                      // 325
	virtual __int64 getAllEquipment(void);                                                                                 // 326
	virtual __int64 getArmorTypeHash(void);                                                                                // 327
	virtual __int64 dropEquipmentOnDeath(__int64, int);                                                                    // 328
	virtual __int64 dropEquipmentOnDeath(void);                                                                            // 329
	virtual __int64 clearVanishEnchantedItemsOnDeath(void);                                                                // 330
	virtual __int64 sendInventory(bool);                                                                                   // 331
	virtual __int64 getDamageAfterMagicAbsorb(__int64, int);                                                               // 332
	virtual __int64 createAIGoals(void);                                                                                   // 333
	virtual __int64 onBorn(C_Entity &, C_Entity &);                                                                        // 334
	virtual __int64 setItemSlot(int EquipmentSlot, C_ItemStack const &);                                                   // 335
	virtual __int64 setTransitioningSitting(bool);                                                                         // 336
	virtual __int64 attackAnimation(C_Entity *, float);                                                                    // 337
	virtual __int64 getAttackTime(void);                                                                                   // 338
	virtual __int64 _getWalkTargetValue(vec3_ti const &);                                                                  // 339
	virtual __int64 canExistWhenDisallowMob(void);                                                                         // 340
	virtual __int64 useNewAi(void);                                                                                        // 341
	virtual __int64 ascendLadder(void);                                                                                    // 342
	virtual __int64 ascendBlockByJumping(void);                                                                            // 343
	virtual __int64 descendBlockByCrouching(void);                                                                         // 344
	virtual __int64 dropContainer(void);                                                                                   // 345
	virtual __int64 initBodyControl(void);                                                                                 // 346
	virtual __int64 jumpFromGround(void);                                                                                  // 347
	virtual __int64 jumpFromGround(__int64 IMobMovementProxy);                                                             // 348
	virtual __int64 updateAi(void);                                                                                        // 349
	virtual __int64 newServerAiStep(void);                                                                                 // 350
	virtual __int64 _serverAiMobStep(void);                                                                                // 351
	virtual __int64 getDamageAfterEnchantReduction(__int64, int);                                                          // 352
	virtual __int64 getDamageAfterArmorAbsorb(__int64, int);                                                               // 353
	virtual __int64 dropBags(void);                                                                                        // 354
	virtual __int64 tickDeath(void);                                                                                       // 355
	virtual __int64 updateGliding(void);                                                                                   // 356
	virtual __int64 _allowAscendingScaffolding(void);                                                                      // 357

public:
	C_InventoryTransactionManager *getTransactionManager();

	AABB *getAABB() {
		return &this->aabb;
	}

	__int64 *getUniqueId() {
		__int64 *v1;  // rbx
		char v3;      // [rsp+30h] [rbp+8h]

		v1 = (__int64 *)((char *)this + 256);
		if (*((__int64 *)this + 32) == -1i64)
			*v1 = *(__int64 *)(*(__int64(__fastcall **)(__int64, char *))(**((__int64 **)this + 110) + 1960i64))(
				*((__int64 *)this + 110),
				&v3);
		return (__int64 *)v1;
	}

	int getTicksUsingItem() {
		return this->ticksUsingItem;
	}

	bool isSneaking() {
		return false;  // TODO
	}

	bool isSprinting() {
		return false;  // TODO
	}
};

#pragma pack(pop)

class C_ServerPlayer;

class C_Player : public C_Entity {
public:
	C_PlayerInventoryProxy *getSupplies();
	;

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

	virtual __int64 prepareRegion(class ChunkSource &);            // 358
	virtual __int64 destroyRegion(void);                           // 359
	virtual __int64 suspendRegion(void);                           // 360
	virtual __int64 resendAllChunks(void);                         // 361
	virtual __int64 _fireWillChangeDimension(void);                // 362
	virtual __int64 _fireDimensionChanged(void);                   // 363
	virtual __int64 changeDimensionWithCredits(int);               // 364
	virtual __int64 tickWorld(int *);                              // 365
	virtual __int64 frameUpdate(struct FrameUpdateContextBase &);  // 366
	virtual __int64 getTickingOffsets(void);                       // 367
	virtual __int64 moveView(void);                                // 368
	virtual __int64 setName(const TextHolder &);                   // 369
	virtual __int64 checkMovementStats(vec3_t const &);            // 370
	virtual __int64 getCurrentStructureFeature(void);              // 371
	virtual __int64 isAutoJumpEnabled(void);                       // 372
	virtual __int64 respawn(void);                                 // 373
	virtual __int64 resetRot(void);                                // 374
	virtual __int64 resetPos(bool);                                // 375
	virtual __int64 isInTrialMode(void);                           // 376
	virtual __int64 hasResource(int);                              // 377
	virtual __int64 completeUsingItem(void);                       // 378
	virtual __int64 setPermissions();                              // 379
	virtual __int64 startDestroying(void);                         // 380
	virtual __int64 stopDestroying(void);                          // 381
	virtual __int64 openPortfolio(void);                           // 382
	virtual __int64 openBook(int, bool, int, C_BlockActor *);      // 383
	virtual __int64 openTrading(__int64 ActorUniqueID, bool);      // 384
	virtual bool canOpenContainerScreen(void);  // 385
	virtual __int64 openChalkboard(__int64, bool);                                                                                   // 386
	virtual __int64 openNpcInteractScreen(__int64);                                                                                  // 387
	virtual __int64 openInventory(void);                                                                                             // 388
	virtual __int64 displayChatMessage(TextHolder &, TextHolder &);                                                                  // 389
	virtual __int64 displayClientMessage(TextHolder &);                                                                              // 390
	virtual __int64 displayTextObjectMessage(__int64, TextHolder &, TextHolder &);                                                   // 391
	virtual __int64 displayTextObjectWhisperMessage(__int64, TextHolder &, TextHolder &);                                            // 392
	virtual __int64 displayWhisperMessage(TextHolder &, TextHolder &, TextHolder &, TextHolder &);                                   // 393
	virtual __int64 startSleepInBed(vec3_ti const &);                                                                                // 394
	virtual __int64 stopSleepInBed(bool, bool);                                                                                      // 395
	virtual __int64 canStartSleepInBed(void);                                                                                        // 396
	virtual __int64 getSleepTimer(void);                                                                                             // 397
	virtual __int64 getPreviousTickSleepTimer(void);                                                                                 // 398
	virtual __int64 openSign(vec3_ti const &);                                                                                       // 399
	virtual __int64 playEmote(TextHolder &);                                                                                         // 400
	virtual __int64 isHostingPlayer(void);                                                                                           // 401
	virtual __int64 isLoading(void);                                                                                                 // 402
	virtual __int64 isPlayerInitialized(void);                                                                                       // 403
	virtual __int64 stopLoading(void);                                                                                               // 404
	virtual __int64 registerTrackedBoss(__int64 actorniqueid);                                                                       // 405
	virtual __int64 unRegisterTrackedBoss(__int64 ActorUniqueID);                                                                    // 406
	virtual __int64 setPlayerGameType(int);                                                                                          // 407
	virtual __int64 initHUDContainerManager(void);                                                                                   // 408
	virtual __int64 _crit(C_Entity *);                                                                                               // 409
	virtual __int64 getEventing(void);                                                                                               // 410
	virtual __int64 getUserId(void);                                                                                                 // 411
	virtual __int64 sendEventPacket(class EventPacket &);                                                                            // 412
	virtual __int64 addExperience(int);                                                                                              // 413
	virtual __int64 addLevels(int);                                                                                                  // 414
	virtual __int64 setContainerData(class IContainerManager &, int, int);                                                           // 415
	virtual __int64 slotChanged(class IContainerManager &, class Container &, int, C_ItemStack const &, C_ItemStack const &, bool);  // 416
	virtual __int64 inventoryChanged(class Container &, int, C_ItemStack const &, C_ItemStack const &, bool);                        // 417
	virtual __int64 refreshContainer(class IContainerManager &);                                                                     // 418
	virtual __int64 deleteContainerManager(void);                                                                                    // 419
	virtual __int64 setFieldOfViewModifier(float);                                                                                   // 420
	virtual __int64 is2DPositionRelevant(int, vec3_ti const &);                                                                      // 421
	virtual __int64 isActorRelevant(C_Entity const &);                                                                               // 422
	virtual __int64 isTeacher(void);                                                                                                 // 423
	virtual __int64 onSuspension(void);                                                                                              // 424
	virtual __int64 onLinkedSlotsChanged(void);                                                                                      // 425
	virtual __int64 startCooldown(C_Item const *);                                                                                   // 426
	virtual __int64 getItemCooldownLeft(HashedString const &);                                                                       // 427
	virtual __int64 getItemCooldownLeft(unsigned long);                                                                              // 428
	virtual __int64 getMaxItemCooldownLeft(void);                                                                                    // 429
	virtual __int64 isItemInCooldown(HashedString const &);                                                                          // 430
	virtual __int64 sendInventoryTransaction(__int64 InventoryTransaction);                                                          // 431
	virtual __int64 sendComplexInventoryTransaction(std::unique_ptr<__int64, std::default_delete<__int64>>);                         // 432
	virtual __int64 sendNetworkPacket(class Packet *);                                                                               // 433
	virtual __int64 getPlayerEventCoordinator(void);                                                                                 // 434
	virtual __int64 getMoveInputHandler(void);                                                                                       // 435
	virtual __int64 getInputMode(void);                                                                                              // 436
	virtual __int64 getPlayMode(void);                                                                                               // 437
	virtual __int64 reportMovementTelemetry(int MovementEventType);                                                                  // 438
	virtual __int64 isSimulated(void);                                                                                               // 439
	virtual __int64 getXuid(void);                                                                                                   // 440
	virtual __int64 getMovementSettings(void);                                                                                       // 441
	virtual __int64 onMovePlayerPacketNormal(vec3_t const &, vec2_t const &, float);                                                 // 442
	virtual __int64 _createChunkSource(ChunkSource &);                                                                               // 443
	virtual __int64 _getSpawnChunkLimit(void);                                                                                       // 444
	virtual __int64 _updateChunkPublisherView(vec3_t const &, float);                                                                // 445


};

class C_ServerPlayer : public C_Player {
};

class C_LocalPlayer : public C_Player {
public:
	void unlockAchievments();

	void swingArm() {
		//using SwingArm = void(__thiscall*)(void*);
		//static SwingArm swingFunc = reinterpret_cast<SwingArm>(FindSignature("40 57 48 83 EC ?? 48 C7 44 24 ?? FE FF FF FF 48 89 5C 24 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 48 8B D9 80 B9"));
		//swingFunc(this);
		this->swing();
	}
	void localPlayerTurn(vec2_t *viewAngles) {
		using Turn = void(__thiscall *)(void *, vec2_t *);
		static Turn TurnFunc = reinterpret_cast<Turn>(FindSignature("48 8B 58 68 48 ?? ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 89 ?? ?? ?? ?? ?? ?? ?? 48 8B 03 48 8B CB FF 50 08 90 48 85 DB ?? 09 48 8B ?? 48 8B CB ?? ?? ?? ?? ?? ?? ?? 48 8B D3 48 8B CE E8 0D BB 1D FF 90 48 85 DB 74 09 48 8B 03 48 8B ?? ?? ?? ?? 48 83 C7 10"));
		TurnFunc(this, viewAngles);
	}
	void applyTurnDelta(vec2_t *viewAngleDelta);
	void setGameModeType(int gma);
};