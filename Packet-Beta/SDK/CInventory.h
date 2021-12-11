#pragma once
#include "CItem.h"

class C_ItemStack;
class C_Inventory;
class C_Player;

class C_Inventory {
private:
	virtual ~C_Inventory();

public:
	bool isFull();
	void dropSlot(int slot);

	// DROPS WHOLE INVENTORY doesnt work tho
	void dropAll(int slot);

	virtual __int64 init();
	virtual bool serverInitItemStackIds(signed int a2, int a3, __int64 a4);
	virtual __int64 addContentChangeListener(__int64 a2);
	virtual __int64 removeContentChangeListener(__int64 a2);
	virtual C_ItemStack* getItemStack(int slot);
	virtual bool hasRoomForItem(C_ItemStack*);
	virtual __int64 addItem(C_ItemStack*);
	virtual __int64 addItemToFirstEmptySlot(C_ItemStack*);
	virtual __int64 setItem(int slot, const C_ItemStack&);
	virtual __int64 setItemWithForceBalance(int, const C_ItemStack&, bool);
	virtual __int64 removeItem(int, int);
	virtual __int64 removeAllItems();
	virtual __int64 dropContents(C_BlockSource&, const vec3_t&, bool);
	virtual __int64 getContainerSize();
	virtual __int64 getMaxStackSize();
	virtual __int64 startOpen(C_Player&);
	virtual __int64 stopOpen(C_Player&);
	virtual __int64 getSlotCopies();
	virtual __int64 getSlots();
	virtual __int64 getItemCount(const C_ItemStack&);
	virtual __int64 findFirstSlotForItem(const C_ItemStack&);
	virtual __int64 canPushInItem(); // BlockSource &,int,int,ItemInstance const&
	virtual __int64 canPullOutItem();  // BlockSource &,int,int,ItemInstance const&
	virtual __int64 setContainerChanged(int); 
	virtual __int64 setContainerMoved();
	virtual __int64 setCustomName(const TextHolder&); 
	virtual __int64 hasCustomName();
	virtual __int64 readAdditionalSaveData(); //  CompoundTag const&
	virtual __int64 addAdditionalSaveData();   //  CompoundTag &
	virtual __int64 createTransactionContext();  // std::function<void(C_Inventory&, int, const C_ItemStack&, const C_ItemStack&)>, std::function<void(void)>
	virtual __int64 initializeContainerContents(C_BlockSource&);
	virtual char isEmpty();
	virtual __int64 add(C_ItemStack&);
	virtual __int64 canAdd(const C_ItemStack&);
	//virtual void unknown();
	virtual __int64 clearSlot(int);
	virtual __int64 clearInventory(int);
	virtual __int64 load(); // ListTag const&,SemVersion const&,Level &
	//virtual void unknown2();
	virtual __int64 getEmptySlotsCount();
	virtual int getFirstEmptySlot();
	virtual __int64 setContainerSize();

	void moveItem(int from, int to);
	void swapSlots(int from, int to);
};

class C_PlayerInventoryProxy {
private:
	char pad_0x0000[0x10];  //0x0000
public:
	int selectedHotbarSlot;  //0x0010
private:
	char pad_0x0014[0x9C];  //0x0014
public:
	C_Inventory* inventory;  //0x00B0
};

class C_ContainerScreenController {
public:
	void handleAutoPlace(uintptr_t a1, std::string name, int slot);

private:
	virtual __int64 destructor();
	virtual __int64 tick(void);//1
	virtual __int64 handleEvent(__int64&); //2
	virtual __int64 sub_140321330(void) const;  //3
	virtual void setScreenState(__int64);//4
	virtual __int64 onOpen(void);               //5
	virtual __int64 onTerminate(void);          //6
	virtual __int64 onInit(void);               //7
	virtual __int64 onDelete(void);             //8

public:
	virtual bool canExit(void);  //9
	virtual __int64 tryExit(void);  //10

private:
	virtual __int64 areControllerTabsEnabled(void);  //11
	virtual __int64 onCreation(void);                //13
	virtual __int64 logCreationTime(std::string const&, double, double, unsigned char);  //13

public:
	virtual __int64 onLeave(void);  //14
	virtual __int64 leaveScreen(void);//15

private:
	virtual __int64 handleGameEventNotification(__int64); //16
	virtual __int64 bind(std::string const&, unsigned int, int, std::string const&, unsigned int, std::string const&, __int64&); //17
	virtual __int64 bind(std::string const&, unsigned int, std::string const&, __int64&); //18
	virtual __int64 handleLicenseChanged(void); //19
	virtual __int64 onDictationEvent(std::string const&); //20

public:
	virtual void setAssociatedvec3_ti(vec3_ti const&); //21
	virtual void setAssociatedEntityUniqueID(__int64); //22
	virtual void setSuspendInput(bool); //23

private:
	virtual __int64 getCallbackInterval(void) const; //24

private:
	virtual __int64 onRender(void); //25
	virtual __int64 addStaticScreenVars(__int64&); //26
	virtual __int64 getAdditionalScreenInfo(void) const; //27
	virtual __int64 getTelemetryOverride(void) const; //28
	virtual __int64 addEventProperties(__int64) const; //29
	virtual __int64 getSceneType(void) const; //30
	virtual __int64 getScreenVersion(void) const; //31
	virtual __int64 screenHandlesGamepadMenuButton(void) const; //32
	virtual __int64 getProxy(void); //33
	virtual __int64 onEntered(void); //34
	virtual __int64 getNameId(std::string const&) const; //35
	virtual __int64 _isStillValid(void) const; //36
	virtual __int64 _getGamepadHelperVisible(void) const; //37
	virtual __int64 _getMixedHelperVisible(void) const; //38
	virtual __int64 _getKeyboardHelperVisible(void) const; //39
	virtual __int64 _getButtonADescription(void); //40
	virtual __int64 _getButtonBDescription(void); //41
	virtual __int64 _getButtonXDescription(void); //42
	virtual __int64 _getButtonYDescription(void); //43
	virtual __int64 _getButtonKeyboardDescription(void); //44

public:
	virtual void _handlePlaceAll(const TextHolder*, int);   
	virtual void _handlePlaceOne(const TextHolder*, int);    
	virtual void _handleSelectSlot(const TextHolder*, int);  
	virtual void _getSelectedSlotInfo(void);          
	virtual C_ItemStack* _getItemStack(const TextHolder*, int); 
	virtual void _getVisualItemStack(const TextHolder*, int);

private:
	virtual void _getTakeableItemStackBase(const TextHolder*, int);                           
	virtual void _onContainerSlotHovered(const TextHolder*, int);                             
	virtual void _onContainerSlotSelected(const TextHolder*, int);                             
	virtual void _onContainerSlotPressed(const TextHolder*, int);                              
	virtual void _shouldSwap(const TextHolder*, int, const TextHolder*, int);                       
	virtual void _getCollectionName(class UIPropertyBag*);                                     
	virtual void _canSplit(const TextHolder*, int);                                            
	virtual void _sendFlyingItem(const C_ItemStack*, const TextHolder*, int, const TextHolder*, int);  
	virtual void _registerCoalesceOrder(void);                                           
	virtual void _registerAutoPlaceOrder(void);                                         
};

class C_CraftingScreenController : public C_ContainerScreenController {
};

class C_ChestScreenController : public C_ContainerScreenController {
};