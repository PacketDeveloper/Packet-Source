#include "Hooks.h"

#include <algorithm>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <glm/ext/matrix_transform.hpp>  // perspective, translate, rotate
#include <glm/gtc/constants.hpp>
#include <glm/mat4x4.hpp>         // mat4
#include <glm/trigonometric.hpp>  //radians

#include "../SDK/Tag.h"

Hooks g_Hooks;
bool isTicked = false;
bool overrideStyledReturn = false;
TextHolder styledReturnText;
//#define TEST_DEBUG

void Hooks::Init() {
	logF("Setting up Hooks...");
	// clang-format off
	// Vtables
	{
		// GameMode::vtable
		{
			uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 01 48 89 51 ?? 48 C7 41 10 FF FF FF FF");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** gameModeVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			if (gameModeVtable == 0x0 || sigOffset == 0x0)
				logF("C_GameMode signature not working!!!");
			else {
				g_Hooks.GameMode_startDestroyBlockHook = std::make_unique<FuncHook>(gameModeVtable[1], Hooks::GameMode_startDestroyBlock);

				g_Hooks.GameMode_getPickRangeHook = std::make_unique<FuncHook>(gameModeVtable[10], Hooks::GameMode_getPickRange);
				
				g_Hooks.GameMode_attackHook = std::make_unique<FuncHook>(gameModeVtable[14], Hooks::GameMode_attack);
			}
		}

		// BlockLegacy::vtable
		{
			intptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? 48 89 01 4C 8B 72 ? 48 B9");  // BlockLegacy constructor
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** blockLegacyVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + 7);
			if (blockLegacyVtable == 0x0 || sigOffset == 0x0)
				logF("C_BlockLegacy signature not working!!!");
			else {
				g_Hooks.BlockLegacy_getRenderLayerHook = std::make_unique<FuncHook>(blockLegacyVtable[180], Hooks::BlockLegacy_getRenderLayer);
			}
		}

		// LocalPlayer::vtable
		{ 
			uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 07 48 8D 8F ?? ?? ?? ?? 48 8B 87");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** localPlayerVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + /*length of instruction*/ 7);
			if (localPlayerVtable == 0x0 || sigOffset == 0x0)
				logF("C_LocalPlayer signature not working!!!");
			else {
				//g_Hooks.Actor_startSwimmingHook = std::make_unique<FuncHook>(localPlayerVtable[182], Hooks::Actor_startSwimming);

				g_Hooks.Actor_lerpMotionHook = std::make_unique<FuncHook>(localPlayerVtable[40], Hooks::Actor_lerpMotion);

				g_Hooks.Mob__isImmobileHook = std::make_unique<FuncHook>(localPlayerVtable[88], Hooks::Mob__isImmobile);
			}
		}

		// MoveInputHandler::vtable
		{
			uintptr_t sigOffset = FindSignature("48 8D 0D ? ? ? ? 49 89 48 ? 49 89 80 ? ? ? ? 49 89 80 ? ? ? ? 48 39 87 ? ? ? ? 74 20 48 8B 8F");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** moveInputVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + 7);
			if (moveInputVtable == 0x0 || sigOffset == 0x0)
				logF("C_GameMode signature not working!!!");
			else 
				g_Hooks.MoveInputHandler_tickHook = std::make_unique<FuncHook>(moveInputVtable[1], Hooks::MoveInputHandler_tick);
			
		}

		// PackAccessStrategy vtables for isTrusted
		{
			uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 49 89 06 49 8D 76 50");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** directoryPackVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset +  7);
			
			{
				g_Hooks.DirectoryPackAccessStrategy__isTrustedHook = std::make_unique<FuncHook>(directoryPackVtable[6], Hooks::DirectoryPackAccessStrategy__isTrusted);
			}

			uintptr_t sigOffset2 = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 03 49 8D 57");
			int offset2 = *reinterpret_cast<int*>(sigOffset2 + 3);
			uintptr_t** directoryPackVtable2 = reinterpret_cast<uintptr_t**>(sigOffset2 + offset2 +  7);
			
			{
				g_Hooks.ZipPackAccessStrategy__isTrustedHook = std::make_unique<FuncHook>(directoryPackVtable2[6], Hooks::ReturnTrue);
			}
			g_Hooks.SkinRepository___checkSignatureFileInPack = std::make_unique<FuncHook>(FindSignature("48 89 5C 24 ? 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B 39 48 8B 59 ? 48 85 DB"), Hooks::ReturnTrue);			
		}
	}

	

	// Signatures
	{
		void* base_tick = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ?? 48 89 6C 24 ?? 56 57 41 54 41 56 41 57 48 83 EC 50 48 8B F9 BE"));
		g_Hooks.Actor__baseTick = std::make_unique<FuncHook>(base_tick, Hooks::Actor_baseTick);

		void* _renderText = reinterpret_cast<void*>(FindSignature("48 8B C4 48 89 58 ?? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 0F 29 70 ?? 0F 29 78 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 4C 8B FA 48 89 54 24"));
		g_Hooks.RenderTextHook = std::make_unique<FuncHook>(_renderText, Hooks::RenderText);
		g_Hooks.RenderTextHook->enableHook();

		void* setupRender = reinterpret_cast<void*>(FindSignature("48 89 5C 24 10 57 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 48 8B DA 48 8B F9 33 D2 41 B8"));
		g_Hooks.UIScene_setupAndRenderHook = std::make_unique<FuncHook>(setupRender, Hooks::UIScene_setupAndRender);

		void* render = reinterpret_cast<void*>(FindSignature("48 89 5C 24 18 56 57 41 56 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 48 8B FA 48 8B D9 41"));
		g_Hooks.UIScene_renderHook = std::make_unique<FuncHook>(render, Hooks::UIScene_render);

		void* fogColorFunc = reinterpret_cast<void*>(FindSignature("41 0F 10 08 48 8B C2 0F"));
		g_Hooks.Dimension_getFogColorHook = std::make_unique<FuncHook>(fogColorFunc, Hooks::Dimension_getFogColor);

		void* timeOfDay = reinterpret_cast<void*>(FindSignature("44 8B C2 B8 F1 19 76 05 F7 EA C1 FA 09 8B C2 C1 E8 1F 03 D0"));
		g_Hooks.Dimension_getTimeOfDayHook = std::make_unique<FuncHook>(timeOfDay, Hooks::Dimension_getTimeOfDay);

		void* chestTick = reinterpret_cast<void*>(FindSignature("40 53 57 48 83 EC ?? 48 83 79 ?? ?? 48"));
		g_Hooks.ChestBlockActor_tickHook = std::make_unique<FuncHook>(chestTick, Hooks::ChestBlockActor_tick);

		//bad
		//void* autoComplete = reinterpret_cast<void*>(FindSignature("48 8B C4 55 57 41 56 48 8D 68 ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 89 58 ?? 48 89 70 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 41 8B F9"));
		//g_Hooks.PleaseAutoCompleteHook = std::make_unique<FuncHook>(autoComplete, Hooks::PleaseAutoComplete);

		uintptr_t** packetSenderVtable = reinterpret_cast<uintptr_t**>(*(uintptr_t*)g_Data.getClientInstance()->loopbackPacketSender);
		g_Hooks.LoopbackPacketSender_sendToServerHook = std::make_unique<FuncHook>(packetSenderVtable[2], Hooks::LoopbackPacketSender_sendToServer);

		void* getFov = reinterpret_cast<void*>(FindSignature("40 53 48 83 EC ?? 0F 29 7C 24 ?? 44"));
		g_Hooks.LevelRendererPlayer_getFovHook = std::make_unique<FuncHook>(getFov, Hooks::LevelRendererPlayer_getFov);

		void* tick_entityList = reinterpret_cast<void*>(FindSignature("48 89 ?? ?? ?? 57 48 83 EC ?? 48 8B ?? E8 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 8B D8 ?? ?? ?? ?? ?? ?? 48 99"));
		g_Hooks.MultiLevelPlayer_tickHook = std::make_unique<FuncHook>(tick_entityList, Hooks::MultiLevelPlayer_tick);

		void* keyMouseFunc = reinterpret_cast<void*>(FindSignature("48 8B C4 48 89 58 10 55 56 57 41 54 41 55 41 56 41 57 48 8D 68 A1 48 81 EC 90 00 00 00"));
		g_Hooks.HIDController_keyMouseHook = std::make_unique<FuncHook>(keyMouseFunc, Hooks::HIDController_keyMouse);

		void* renderLevel = reinterpret_cast<void*>(FindSignature("48 89 5C 24 10 48 89 74 24 20 57 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 49 8B F8 48 8B DA"));
		g_Hooks.LevelRenderer_renderLevelHook = std::make_unique<FuncHook>(renderLevel, Hooks::LevelRenderer_renderLevel);

		void* clickHook = reinterpret_cast<void*>(FindSignature("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24 ? ? ? ? 48 8B D9"));
		g_Hooks.ClickFuncHook = std::make_unique<FuncHook>(clickHook, Hooks::ClickFunc);

		void* chestScreenControllerTick = reinterpret_cast<void*>(FindSignature("48 89 5C 24 08 57 48 83 EC 20 48 8B F9 E8 ?? ?? ?? ?? 48 8B 17 48 8B CF 8B D8 FF 92 ?? ?? ?? ?? 84 C0 74 31"));
        g_Hooks.ChestScreenController_tickHook = std::make_unique<FuncHook>(chestScreenControllerTick, Hooks::ChestScreenController_tick);

		void* fullbright = reinterpret_cast<void*>(FindSignature("48 83 EC ?? 80 B9 ?? ?? ?? ?? ?? 48 8D 54 24 ?? 48 8B 01 74 35 41 B8 0D 01 00 00"));
		g_Hooks.GetGammaHook = std::make_unique<FuncHook>(fullbright, Hooks::GetGamma);
		
		//bad
		//void* onAppSuspended = reinterpret_cast<void*>(FindSignature("48 8B C4 55 48 8B EC 48 83 EC ? 48 C7 45 ? ? ? ? ? 48 89 58 ? 48 89 70 ? 48 89 78 ? 48 8B F1 E8 ? ? ? ? 48 8B D8 48 8B C8"));
		//g_Hooks.MinecraftGame_onAppSuspendedHook = std::make_unique<FuncHook>(onAppSuspended, Hooks::MinecraftGame_onAppSuspended);

		void* RakNetInstance__tick = reinterpret_cast<void*>(FindSignature("48 89 5C 24 10 48 89 74 24 18 55 57 41 54 41 56 41 57 48 8D ?? 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 48 8B F9 45 33 E4 4C"));
		g_Hooks.RakNetInstance_tickHook = std::make_unique<FuncHook>(RakNetInstance__tick, Hooks::RakNetInstance_tick);

		void* getRotation = reinterpret_cast<void*>(FindSignature("48 89 5C 24 10 57 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 80"));
        g_Hooks.Actor_getRotationHook = std::make_unique<FuncHook>(getRotation, Hooks::Actor_getRotation);

		void* getRot = reinterpret_cast<void*>(FindSignature("48 89 5C 24 10 57 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 80"));
        g_Hooks.RotHook = std::make_unique<FuncHook>(getRot, Hooks::Actor_getRotation);
		
		//bad
		//void* ConnectionRequest__create = reinterpret_cast<void*>(FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 49 8B D9 4D 8B F8"));
		//g_Hooks.ConnectionRequest_createHook = std::make_unique<FuncHook>(ConnectionRequest__create, Hooks::ConnectionRequest_create);

		void* ConnectionRequest__create = reinterpret_cast<void*>(FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 4D 8B E1 4D 8B F8 48 89 55"));
		g_Hooks.ConnectionRequest_createHook = std::make_unique<FuncHook>(ConnectionRequest__create, Hooks::ConnectionRequest_create);
		
		//bad
	//	void* _getSkinPack = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B E2 48 8B F1"));
		//g_Hooks.SkinRepository___loadSkinPackHook = std::make_unique<FuncHook>(_getSkinPack, Hooks::SkinRepository___loadSkinPack);

		
		//bad
		//void* _toStyledString = reinterpret_cast<void*>(FindSignature("40 55 56 57 48 81 EC ?? ?? ?? ?? 48 C7 44 24 ?? FE FF FF FF 48 89 9C 24 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 48 8B FA 48 8B D9 48 89 54 24 ?? 33 D2"));
		//g_Hooks.toStyledStringHook = std::make_unique<FuncHook>(_toStyledString, Hooks::toStyledString);
		 
#ifdef TEST_DEBUG
		void* addAction = reinterpret_cast<void*>(FindSignature("55 56 57 41 56 41 57 48 83 EC ?? 45 0F B6 F8 4C 8B F2 48 8B F1 48 8B 01 48 8B 88") - 5);
		g_Hooks.InventoryTransactionManager__addActionHook = std::make_unique<FuncHook>(addAction, Hooks::InventoryTransactionManager__addAction);
#endif

		void* localPlayerUpdateFromCam = reinterpret_cast<void*>(FindSignature(" 48 89 5C 24 10 57 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 80 ?? ?? ?? ?? ?? 00 48 8B FA 48 8B D9"));
		g_Hooks.LocalPlayer__updateFromCameraHook = std::make_unique<FuncHook>(localPlayerUpdateFromCam, Hooks::LocalPlayer__updateFromCamera);
		
		//bad
		//void* renderNameTags = reinterpret_cast<void*>(FindSignature("48 8B C4 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 C7 45 ? ? ? ? ? 48 89 58 ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 48 8B 05 ? ? ? ? 48 33 C4"));
		//g_Hooks.LevelRendererPlayer__renderNameTagsHook = std::make_unique<FuncHook>(renderNameTags, Hooks::LevelRendererPlayer__renderNameTags);
	
		static constexpr auto counterStart = __COUNTER__ + 1;
		#define lambda_counter (__COUNTER__ - counterStart)
		
		void* levelRendererBobView = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ?? 57 48 81 EC ?? ?? ?? ?? 48 8B D9 0F 29 B4 24 ?? ?? ?? ?? 48 8B 89"));

		static auto bobViewHookF = [](__int64 _this, glm::mat4& matrix, float lerpT) {
			static auto origFunc = g_Hooks.lambdaHooks.at(lambda_counter)->GetFastcall<void, __int64, glm::mat4&, float>();
						static auto animations = moduleMgr->getModule<Animations>();
			static auto killaura = moduleMgr->getModule<Killaura>();
			//static auto test = moduleMgr->getModule<TestModule>();
            auto p = g_Data.getLocalPlayer();
            float degrees = fmodf(p->getPosOld()->lerp(p->getPos(), lerpT).x, 5) - 2.5f;
            degrees *= 180 / 2.5f;

            auto pos = g_Data.getClientInstance()->levelRenderer->origin;

            glm::mat4 View = matrix;

            matrix = View;

			// AutoBlock
			if (animations->isEnabled() && killaura->isEnabled() && killaura->holdingWeapon && g_Data.isInGame()) {
				if (animations->mode.getSelectedValue() == 1 && !killaura->targetListEmpty) {
					matrix = glm::translate<float>(matrix, glm::vec3(5.54, 0.85, -2.00));
					matrix = glm::scale<float>(matrix, glm::vec3(2, 2, 2));
				}
				if (animations->mode.getSelectedValue() == 1 && !killaura->targetListEmpty && killaura->holdingWeapon && g_Data.isInGame()) {
					float degrees = 13;
					degrees *= 180 / 4;

					auto pos = g_Data.getClientInstance()->levelRenderer->origin;
			
					glm::mat4 View = matrix;
			
					matrix = View;
					matrix = glm::rotate<float>(matrix, glm::radians<float>(degrees), glm::vec3(2.58, -4.40, -3.50));
					matrix = glm::translate<float>(matrix, glm::vec3(1.08, -0.02, -0.02));
					matrix = glm::scale<float>(matrix, glm::vec3(4, 4, 4));
					matrix = glm::translate<float>(matrix, glm::vec3(0.5, 0.4, 0.4));
				}
			}

			// Custom settings
            if (animations->isEnabled() && animations->mode.getSelectedValue() == 0) {
                if (animations->doTranslate)
                    matrix = glm::translate<float>(matrix, glm::vec3(animations->xTrans, animations->yTrans, animations->zTrans)); // X Y Z

                if (animations->doScale)
                    matrix = glm::scale<float>(matrix, glm::vec3(animations->xMod, animations->yMod, animations->zMod)); // SCALE

                if (animations->doRotate)
                    matrix = glm::rotate<float>(matrix, degrees, glm::vec3(animations->xRotate, animations->yRotate, animations->zRotate)); // idk
            }

			// Spin
			if (animations->isEnabled() && animations->aroundWorld && animations->mode.getSelectedValue() == 0) {
				auto p = g_Data.getLocalPlayer();
				float degrees = fmodf(p->getPosOld()->lerp(p->getPos(), lerpT).x, 5) - 2.5f;
				degrees *= 180 / 2.5f;

				auto pos = g_Data.getClientInstance()->levelRenderer->origin;
			
				glm::mat4 View = matrix;
			
				matrix = View;
				matrix = glm::rotate<float>(matrix, glm::radians<float>(degrees), glm::vec3(0, 0, 1));
			}
			return origFunc(_this, matrix, lerpT);
		};
		
		std::shared_ptr<FuncHook> bobViewHook = std::make_shared<FuncHook>(levelRendererBobView, (decltype(&bobViewHookF.operator()))bobViewHookF);

		g_Hooks.lambdaHooks.push_back(bobViewHook);

		#undef lambda_counter

		logF("Hooks initialized");
	}

// clang-format on
}

void Hooks::Restore() {
	MH_DisableHook(MH_ALL_HOOKS);
	Sleep(10);
}

void Hooks::Enable() {
	logF("Hooks enabled");
	MH_EnableHook(MH_ALL_HOOKS);
}

void Hooks::Actor_baseTick(C_Entity* _this) {
	static auto oTick = g_Hooks.Actor__baseTick->GetFastcall<void, C_Entity*>();
	oTick(_this);
	if (_this != (C_Entity*)g_Data.getLocalPlayer())
		return;

	C_GameMode* gm = *(C_GameMode**)((__int64)_this + 0x12E8);

	GameData::updateGameData(gm);
	if (gm->player == (C_Entity*)g_Data.getLocalPlayer() && gm->player != nullptr)
		moduleMgr->onTick(gm);
}

void Hooks::Actor_getRotation(C_Entity* _this, vec4_t& newAngle) {
	static auto oFunc = g_Hooks.Actor_getRotationHook->GetFastcall<void, C_Entity*, vec4_t&>();
	static auto killaura = moduleMgr->getModule<Killaura>();
	static auto tpaura = moduleMgr->getModule<TPAura>();
	static auto scaffold = moduleMgr->getModule<Scaffold>();
	if (killaura->isEnabled() && !killaura->targetListEmpty && scaffold->useRot) {
		if (g_Data.getLocalPlayer() != nullptr && killaura->mode.getSelectedValue() == 0 || killaura->mode.getSelectedValue() == 1)
			return oFunc(_this, killaura->testRot);
	}
	if (scaffold->isEnabled() && scaffold->tower) {
		if (g_Data.getLocalPlayer() != nullptr && scaffold->isOnHive && scaffold->isHoldingSpace)
			return oFunc(_this, scaffold->scaffoldRot);
	}
	if (tpaura->isEnabled() && !tpaura->targetListEmpty) {
		if (g_Data.getLocalPlayer() != nullptr && tpaura->rotations && tpaura->mode.getSelectedValue() == 0 || tpaura->mode.getSelectedValue() == 1)
			return oFunc(_this, tpaura->tpAuraRot);
	}
	oFunc(_this, newAngle);
}

__int64 Hooks::UIScene_setupAndRender(C_UIScene* uiscene, __int64 screencontext) {
	static auto oSetup = g_Hooks.UIScene_setupAndRenderHook->GetFastcall<__int64, C_UIScene*, __int64>();

	g_Hooks.shouldRender = false;
	//g_Hooks.shouldRender = uiscene->isPlayScreen();

	return oSetup(uiscene, screencontext);
}

__int64 Hooks::UIScene_render(C_UIScene* uiscene, __int64 screencontext) {
	static auto oRender = g_Hooks.UIScene_renderHook->GetFastcall<__int64, C_UIScene*, __int64>();

	//g_Hooks.shouldRender = uiscene->isPlayScreen();
	g_Hooks.shouldRender = false;

	bool alwaysRender = moduleMgr->isInitialized() && moduleMgr->getModule<ArrayList>()->alwaysShow;

	TextHolder alloc = {};
	uiscene->getScreenName(&alloc);

	if (alloc.getTextLength() < 100) {
		strcpy_s(g_Hooks.currentScreenName, alloc.getText());
	}

	if (!g_Hooks.shouldRender) {
		g_Hooks.shouldRender = (strcmp(alloc.getText(), "start_screen") == 0 || strcmp(alloc.getText(), "hud_screen") == 0 || strcmp(alloc.getText(), "inventory_screen") == 0);
	}
	static auto chestStealer = moduleMgr->getModule<ChestStealer>();
	static auto invManager = moduleMgr->getModule<InvManager>();
	static auto clickGUI = moduleMgr->getModule<ClickGuiMod>();
	static auto scaffold = moduleMgr->getModule<Scaffold>();
	std::string screenName(g_Hooks.currentScreenName);

	if (clickGUI->isEnabled() && strcmp(alloc.getText(), "pause_screen") == 0) {
		clickGUI->setEnabled(false);
	}

	if (invManager->autoDisable && strcmp(screenName.c_str(), "start_screen") == 0) {
		//auto box = g_Data.addInfoBox("InvManager: Disabled");
		//box->closeTimer = 14;
		invManager->setEnabled(false);
	}
	if (chestStealer->autoDisable && strcmp(screenName.c_str(), "start_screen") == 0) {
		//auto box = g_Data.addInfoBox("ChestStealer: Disabled");
		//box->closeTimer = 14;
		chestStealer->setEnabled(false);
	}
	if (scaffold->isEnabled() && strcmp(screenName.c_str(), "start_screen") == 0) {
		scaffold->setEnabled(false);
	}
	alloc.resetWithoutDelete();

	return oRender(uiscene, screencontext);
}

__int64 Hooks::RenderText(__int64 a1, C_MinecraftUIRenderContext* renderCtx) {
	static auto oText = g_Hooks.RenderTextHook->GetFastcall<__int64, __int64, C_MinecraftUIRenderContext*>();
	C_GuiData* dat = g_Data.getClientInstance()->getGuiData();

	DrawUtils::setCtx(renderCtx, dat);

	if (GameData::shouldHide() || !g_Hooks.shouldRender || !moduleMgr->isInitialized())
		return oText(a1, renderCtx);

	static auto hudMod = moduleMgr->getModule<HudModule>();
#ifdef _DEBUG
	static auto hudEditorMod = moduleMgr->getModule<HudEditorMod>();
#endif
	static auto arraylistMod = moduleMgr->getModule<ArrayList>();
	static auto watermark = moduleMgr->getModule<Watermark>();
	static auto blinkMod = moduleMgr->getModule<Blink>();
	static auto disabler = moduleMgr->getModule<Disabler>();
	static auto clickGuiModule = moduleMgr->getModule<ClickGuiMod>();

	HImGui.startFrame();

	g_Data.frameCount++;

	auto wid = g_Data.getClientInstance()->getGuiData()->windowSize;

	// Call PreRender() functions
	moduleMgr->onPreRender(renderCtx);
	DrawUtils::flush();

	__int64 retval = oText(a1, renderCtx);

	bool shouldPostRender = true;
	bool shouldRenderArrayList = true;
	bool shouldRenderTabGui = true;
	bool shouldRenderWatermark = true;

	static float rcolors[4];          // Rainbow color array RGBA
	static float disabledRcolors[4];  // Rainbow Colors, but for disabled modules
	static float currColor[4];        // ArrayList colors

	static constexpr float colorIncrease = 0.05f;
	static float dynamicColor = 0.f;
	static bool isIncreasingColor = true;
	static bool isIncreasingcurrColor = true;
	static float dynamic = 0.f;

	dynamic = dynamicColor;
	isIncreasingcurrColor = !isIncreasingColor;

	// Rainbow color updates
	{
		Utils::ApplyRainbow(rcolors, 0.0005f);  // Increase Hue of rainbow color array
		disabledRcolors[0] = std::min(10.f, rcolors[0] * 0.20f + -22.f);
		disabledRcolors[1] = std::min(10.f, rcolors[2] * 0.21f + -24.f);
		disabledRcolors[2] = std::min(10.f, rcolors[3] * 0.22f + -26.f);
		disabledRcolors[3] = 1;
	}
	// Dynamic color updates
	{
		dynamicColor += (isIncreasingColor ? 0.007f : -0.007f) * DrawUtils::getLerpTime();
		if (dynamicColor < 0.2f) {
			dynamicColor = 0.2;
			isIncreasingColor = true;
		}
		if (dynamicColor > 1.f) {
			dynamicColor = 1.f;
			isIncreasingColor = false;
		}
	}

	// Draw Watermark
	{
		if (shouldRenderWatermark) {
			static auto clickGUI = moduleMgr->getModule<ClickGuiMod>();
			static auto watermark = moduleMgr->getModule<Watermark>();
			static auto hudMod = moduleMgr->getModule<HudModule>();
			if (watermark->isEnabled() && !clickGUI->isEnabled()) {
				// Draw Watermark
				vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
				if (watermark->mode.getSelectedValue() == 0) {
					if (watermark->firstLetter) {  // Packet FirstLetter
						constexpr float nameTextSize = 1.3;
						std::string textShadow2 = "Packet Client";
						std::string color2 = "P";
						std::string white2 = "acket Client";
						float startY = hudMod->tabgui ? 6 * 10 : 0.f;
						float l = DrawUtils::getTextWidth(&textShadow2, 1.4);
						vec4_t rectPos = vec4_t(2.5f, startY + 4.f * 1, l, startY + 20.f * 1);
						vec2_t textPos = vec2_t(rectPos.x + 13, rectPos.y + 4.f);
						vec2_t pPos = vec2_t(rectPos.x + 5, rectPos.y + 4.f);

						DrawUtils::drawText(vec2_t(textPos), &white2, MC_Color(255, 255, 255), nameTextSize, 1, true);

						if (hudMod->color.getSelectedValue() != 1)  // Rainbow
							DrawUtils::drawText(vec2_t(pPos), &color2, MC_Color(currColor), nameTextSize, 1, true);
						if (hudMod->color.getSelectedValue() == 1) {  // Dynamic
							DrawUtils::drawText(vec2_t(pPos), &color2, MC_Color(dynamic, dynamic, dynamic), nameTextSize, 1, true);
						} else if (hudMod->color.getSelectedValue() == 4) {  // White
							DrawUtils::drawText(vec2_t(pPos), &color2, MC_Color(255, 255, 255), nameTextSize, 1, true);
						} else if (hudMod->color.getSelectedValue() == 5) {  // Red
							DrawUtils::drawText(vec2_t(pPos), &color2, MC_Color(255, 0, 0), nameTextSize, 1, true);
						} else if (hudMod->color.getSelectedValue() == 6) {  // Orange
							DrawUtils::drawText(vec2_t(pPos), &color2, MC_Color(255, 127, 0), nameTextSize, 1, true);
						} else if (hudMod->color.getSelectedValue() == 7) {  // Yellow
							DrawUtils::drawText(vec2_t(pPos), &color2, MC_Color(255, 255, 0), nameTextSize, 1, true);
						} else if (hudMod->color.getSelectedValue() == 8) {  // Green
							DrawUtils::drawText(vec2_t(pPos), &color2, MC_Color(0, 255, 0), nameTextSize, 1, true);
						} else if (hudMod->color.getSelectedValue() == 9) {  // Blue
							DrawUtils::drawText(vec2_t(pPos), &color2, MC_Color(0, 170, 255), nameTextSize, 1, true);
						} else if (hudMod->color.getSelectedValue() == 11) {  // Purple
							DrawUtils::drawText(vec2_t(pPos), &color2, MC_Color(148, 0, 211), nameTextSize, 1, true);
						} else if (hudMod->color.getSelectedValue() == 12) {  // Pink
							DrawUtils::drawText(vec2_t(pPos), &color2, MC_Color(255, 192, 203), nameTextSize, 1, true);
						}
					} else {  // Packet
						constexpr float nameTextSize = 1.3;
						std::string tempStr = watermark->message;
						float startY = hudMod->tabgui ? 6 * 10 : 0.f;
						float l = DrawUtils::getTextWidth(&tempStr, 1.064) + 6.f;
						vec4_t rectPos = vec4_t(2.5f, startY + 4.f * 1, l, startY + 20.f * 1);
						vec2_t pPos = vec2_t(rectPos.x + 5, rectPos.y + 4.f);

						if (hudMod->color.getSelectedValue() != 1)  // Rainbow
							DrawUtils::drawText(vec2_t(pPos), &tempStr, MC_Color(currColor), nameTextSize, 1, true);
						if (hudMod->color.getSelectedValue() == 1) {  // Dynamic
							DrawUtils::drawText(vec2_t(pPos), &tempStr, MC_Color(dynamic, dynamic, dynamic), nameTextSize, 1, true);
						} else if (hudMod->color.getSelectedValue() == 4) {  // White
							DrawUtils::drawText(pPos, &tempStr, MC_Color(255, 255, 255), nameTextSize, 1, true);
						} else if (hudMod->color.getSelectedValue() == 5) {  // Red
							DrawUtils::drawText(pPos, &tempStr, MC_Color(255, 0, 0), nameTextSize, 1, true);
						} else if (hudMod->color.getSelectedValue() == 6) {  // Orange
							DrawUtils::drawText(pPos, &tempStr, MC_Color(255, 127, 0), nameTextSize, 1, true);
						} else if (hudMod->color.getSelectedValue() == 7) {  // Yellow
							DrawUtils::drawText(pPos, &tempStr, MC_Color(255, 255, 0), nameTextSize, 1, true);
						} else if (hudMod->color.getSelectedValue() == 8) {  // Green
							DrawUtils::drawText(pPos, &tempStr, MC_Color(0, 255, 0), nameTextSize, 1, true);
						} else if (hudMod->color.getSelectedValue() == 9) {  // Blue
							DrawUtils::drawText(pPos, &tempStr, MC_Color(0, 170, 255), nameTextSize, 1, true);
						} else if (hudMod->color.getSelectedValue() == 11) {  // Purple
							DrawUtils::drawText(pPos, &tempStr, MC_Color(148, 0, 211), nameTextSize, 1, true);
						} else if (hudMod->color.getSelectedValue() == 12) {  // Pink
							DrawUtils::drawText(pPos, &tempStr, MC_Color(255, 192, 203), nameTextSize, 1, true);
						}
					}
				}

				// New
				if (g_Data.isInGame()) {
					if (watermark->mode.getSelectedValue() == 1) {
						if (watermark->firstLetter) {
							auto player = g_Data.getLocalPlayer();
							std::string name = player->getNameTag()->getText();
							name = Utils::sanitize(name);
							std::string textShadow2 = "Packet : " + name;
							std::string color2 = "P";
							std::string white2 = "acket : " + name;
							float startY = hudMod->tabgui ? 6 * 10 : 0.f;
							float l = DrawUtils::getTextWidth(&textShadow2, 1.064) + 10.f;
							float l2 = DrawUtils::getTextWidth(&textShadow2, 1.064) + 9.5f;
							vec4_t linePos = vec4_t(3.f, startY + 2.f * 2, l2, startY + 3.f * 1);
							vec4_t rectPos = vec4_t(2.5f, startY + 4.f * 1, l, startY + 20.f * 1);
							vec2_t textPos = vec2_t(rectPos.x + 12, rectPos.y + 4.f);
							vec2_t pPos = vec2_t(rectPos.x + 6, rectPos.y + 4.f);

							DrawUtils::drawText(vec2_t(textPos), &white2, MC_Color(255, 255, 255), 1, 1, true);

							if (hudMod->color.getSelectedValue() != 1)  // Rainbow
								DrawUtils::drawText(vec2_t(pPos), &color2, MC_Color(currColor), 1, 1, true);
							if (hudMod->color.getSelectedValue() == 1) {  // Dynamic
								DrawUtils::drawText(pPos, &color2, MC_Color(dynamic, dynamic, dynamic), 1, 1, true);
							} else if (hudMod->color.getSelectedValue() == 4) {  // White
								DrawUtils::drawText(pPos, &color2, MC_Color(255, 255, 255), 1, 1, true);
							} else if (hudMod->color.getSelectedValue() == 5) {  // Red
								DrawUtils::drawText(pPos, &color2, MC_Color(255, 0, 0), 1, 1, true);
							} else if (hudMod->color.getSelectedValue() == 6) {  // Orange
								DrawUtils::drawText(pPos, &color2, MC_Color(255, 127, 0), 1, 1, true);
							} else if (hudMod->color.getSelectedValue() == 7) {  // Yellow
								DrawUtils::drawText(pPos, &color2, MC_Color(255, 255, 0), 1, 1, true);
							} else if (hudMod->color.getSelectedValue() == 8) {  // Green
								DrawUtils::drawText(pPos, &color2, MC_Color(0, 255, 0), 1, 1, true);
							} else if (hudMod->color.getSelectedValue() == 9) {  // Blue
								DrawUtils::drawText(pPos, &color2, MC_Color(0, 170, 255), 1, 1, true);
							} else if (hudMod->color.getSelectedValue() == 11) {  // Purple
								DrawUtils::drawText(pPos, &color2, MC_Color(148, 0, 211), 1, 1, true);
							} else if (hudMod->color.getSelectedValue() == 12) {  // Pink
								DrawUtils::drawText(pPos, &color2, MC_Color(255, 192, 203), 1, 1, true);
							}

							DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), watermark->opacity);
							if (hudMod->color.getSelectedValue() != 1)  // Rainbow
								DrawUtils::drawRectangle(linePos, MC_Color(currColor), 1.f);
							if (hudMod->color.getSelectedValue() == 1) {  // Dynamic
								DrawUtils::drawRectangle(linePos, MC_Color(dynamic, dynamic, dynamic), 1.f);
							} else if (hudMod->color.getSelectedValue() == 4) {  // White
								DrawUtils::drawRectangle(linePos, MC_Color(255, 255, 255), 1.f);
							} else if (hudMod->color.getSelectedValue() == 5) {  // Red
								DrawUtils::drawRectangle(linePos, MC_Color(255, 0, 0), 1.f);
							} else if (hudMod->color.getSelectedValue() == 6) {  // Orange
								DrawUtils::drawRectangle(linePos, MC_Color(255, 127, 0), 1.f);
							} else if (hudMod->color.getSelectedValue() == 7) {  // Yellow
								DrawUtils::drawRectangle(linePos, MC_Color(255, 255, 0), 1.f);
							} else if (hudMod->color.getSelectedValue() == 8) {  // Green
								DrawUtils::drawRectangle(linePos, MC_Color(0, 255, 0), 1.f);
							} else if (hudMod->color.getSelectedValue() == 9) {  // Blue
								DrawUtils::drawRectangle(linePos, MC_Color(0, 170, 255), 1.f);
							} else if (hudMod->color.getSelectedValue() == 11) {  // Purple
								DrawUtils::drawRectangle(linePos, MC_Color(148, 0, 211), 1.f);
							} else if (hudMod->color.getSelectedValue() == 12) {  // Pink
								DrawUtils::drawRectangle(linePos, MC_Color(255, 192, 203), 1.f);
							}
						} else {
							auto player = g_Data.getLocalPlayer();
							std::string name = player->getNameTag()->getText();
							name = Utils::sanitize(name);
							std::string textStr = watermark->message + " : " + name;
							std::string textShadow2 = watermark->message + " : " + name;
							float startY = hudMod->tabgui ? 6 * 10 : 0.f;
							float l = DrawUtils::getTextWidth(&textStr, 1.1) + 6.f;
							vec4_t linePos = vec4_t(3.f, startY + 2.f * 2, l, startY + 3.f * 1);
							vec4_t rectPos = vec4_t(2.5f, startY + 4.f * 1, l, startY + 20.f * 1);
							vec2_t textPos = vec2_t(rectPos.x + 6, rectPos.y + 4.f);

							DrawUtils::drawText(vec2_t(textPos), &textStr, MC_Color(255, 255, 255), 1, 1, true);

							DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), watermark->opacity);
							if (hudMod->color.getSelectedValue() != 1)  // Rainbow
								DrawUtils::drawRectangle(linePos, MC_Color(currColor), 1.f);
							if (hudMod->color.getSelectedValue() == 1) {  // Dynamic
								DrawUtils::drawRectangle(linePos, MC_Color(dynamic, dynamic, dynamic), 1.f);
							} else if (hudMod->color.getSelectedValue() == 4) {  // White
								DrawUtils::drawRectangle(linePos, MC_Color(255, 255, 255), 1.f);
							} else if (hudMod->color.getSelectedValue() == 5) {  // Red
								DrawUtils::drawRectangle(linePos, MC_Color(255, 0, 0), 1.f);
							} else if (hudMod->color.getSelectedValue() == 6) {  // Orange
								DrawUtils::drawRectangle(linePos, MC_Color(255, 127, 0), 1.f);
							} else if (hudMod->color.getSelectedValue() == 7) {  // Yellow
								DrawUtils::drawRectangle(linePos, MC_Color(255, 255, 0), 1.f);
							} else if (hudMod->color.getSelectedValue() == 8) {  // Green
								DrawUtils::drawRectangle(linePos, MC_Color(0, 255, 0), 1.f);
							} else if (hudMod->color.getSelectedValue() == 9) {  // Blue
								DrawUtils::drawRectangle(linePos, MC_Color(0, 170, 255), 1.f);
							} else if (hudMod->color.getSelectedValue() == 11) {  // Purple
								DrawUtils::drawRectangle(linePos, MC_Color(148, 0, 211), 1.f);
							} else if (hudMod->color.getSelectedValue() == 12) {  // Pink
								DrawUtils::drawRectangle(linePos, MC_Color(255, 192, 203), 1.f);
							}
						}
					}

					// Fadeaway first letter
					if (shouldRenderWatermark) {
						if (watermark->mode.getSelectedValue() == 2) {
							if (watermark->firstLetter) {
								constexpr float nameTextSize = 1.35f;
								std::string color2 = "F";
								std::string white2 = "adeaway";
								std::string length = "Fadeaway";
								float startY = hudMod->tabgui ? 6 * 10 : 0;
								float l = DrawUtils::getTextWidth(&length, 1.4) + 16;
								vec4_t rectPos = vec4_t(2.5f, startY + 4.f * 1, l, startY + 23);
								vec2_t textPos = vec2_t(rectPos.x + 14, rectPos.y + 4.f);
								vec2_t pPos = vec2_t(rectPos.x + 6, rectPos.y + 4.f);

								DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), watermark->opacity);
								DrawUtils::drawText(vec2_t(textPos), &white2, MC_Color(255, 255, 255), 1.5, 1, true);

								if (hudMod->color.getSelectedValue() != 1)  // Rainbow
									DrawUtils::drawRectangle(rectPos, MC_Color(currColor), 1);
								if (hudMod->color.getSelectedValue() == 1) {  // Dynamic
									DrawUtils::drawRectangle(rectPos, MC_Color(dynamic, dynamic, dynamic), 1);
								} else if (hudMod->color.getSelectedValue() == 4) {  // White
									DrawUtils::drawRectangle(rectPos, MC_Color(255, 255, 255), 1);
								} else if (hudMod->color.getSelectedValue() == 5) {  // Red
									DrawUtils::drawRectangle(rectPos, MC_Color(255, 0, 0), 2);
								} else if (hudMod->color.getSelectedValue() == 6) {  // Orange
									DrawUtils::drawRectangle(rectPos, MC_Color(255, 127, 0), 1);
								} else if (hudMod->color.getSelectedValue() == 7) {  // Yellow
									DrawUtils::drawRectangle(rectPos, MC_Color(255, 255, 0), 1);
								} else if (hudMod->color.getSelectedValue() == 8) {  // Green
									DrawUtils::drawRectangle(rectPos, MC_Color(0, 255, 0), 1);
								} else if (hudMod->color.getSelectedValue() == 9) {  // Blue
									DrawUtils::drawRectangle(rectPos, MC_Color(0, 170, 255), 1);
								} else if (hudMod->color.getSelectedValue() == 11) {  // Purple
									DrawUtils::drawRectangle(rectPos, MC_Color(148, 0, 211), 1);
								} else if (hudMod->color.getSelectedValue() == 12) {  // Pink
									DrawUtils::drawRectangle(rectPos, MC_Color(255, 192, 203), 1);
								}
								if (hudMod->color.getSelectedValue() != 1)  // Rainbow
									DrawUtils::drawText(vec2_t(pPos), &color2, MC_Color(currColor), 1.5, 1, true);
								if (hudMod->color.getSelectedValue() == 1) {  // Dynamic
									DrawUtils::drawText(pPos, &color2, MC_Color(dynamic, dynamic, dynamic), 1.5, 1, true);
								} else if (hudMod->color.getSelectedValue() == 4) {  // White
									DrawUtils::drawText(pPos, &color2, MC_Color(255, 255, 255), 1.5, 1, true);
								} else if (hudMod->color.getSelectedValue() == 5) {  // Red
									DrawUtils::drawText(pPos, &color2, MC_Color(255, 0, 0), 1.5, 1, true);
								} else if (hudMod->color.getSelectedValue() == 6) {  // Orange
									DrawUtils::drawText(pPos, &color2, MC_Color(255, 127, 0), 1.5, 1, true);
								} else if (hudMod->color.getSelectedValue() == 7) {  // Yellow
									DrawUtils::drawText(pPos, &color2, MC_Color(255, 255, 0), 1.5, 1, true);
								} else if (hudMod->color.getSelectedValue() == 8) {  // Green
									DrawUtils::drawText(pPos, &color2, MC_Color(0, 255, 0), 1.5, 1, true);
								} else if (hudMod->color.getSelectedValue() == 9) {  // Blue
									DrawUtils::drawText(pPos, &color2, MC_Color(0, 170, 255), 1.5, 1, true);
								} else if (hudMod->color.getSelectedValue() == 11) {  // Purple
									DrawUtils::drawText(pPos, &color2, MC_Color(148, 0, 211), 1.5, 1, true);
								} else if (hudMod->color.getSelectedValue() == 12) {  // Pink
									DrawUtils::drawText(pPos, &color2, MC_Color(255, 192, 203), 1.5, 1, true);
								}
							} else {  // fadeaway
								constexpr float nameTextSize = 1.35f;
								std::string tempStr = watermark->message;
								float startY = hudMod->tabgui ? 6 * 10 : 0.f;
								float l = DrawUtils::getTextWidth(&tempStr, 1.4) + 10;
								vec4_t rectPos = vec4_t(2.5, startY + 4, l, startY + 20);
								vec2_t pPos = vec2_t(rectPos.x + 6, rectPos.y + 4.f);

								DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), watermark->opacity);

								if (hudMod->color.getSelectedValue() != 1)  // Rainbow
									DrawUtils::drawRectangle(rectPos, MC_Color(currColor), 1);
								if (hudMod->color.getSelectedValue() == 1) {  // Dynamic
									DrawUtils::drawRectangle(rectPos, MC_Color(dynamic, dynamic, dynamic), 1);
								} else if (hudMod->color.getSelectedValue() == 4) {  // White
									DrawUtils::drawRectangle(rectPos, MC_Color(255, 255, 255), 2);
								} else if (hudMod->color.getSelectedValue() == 5) {  // Red
									DrawUtils::drawRectangle(rectPos, MC_Color(255, 0, 0), 2);
								} else if (hudMod->color.getSelectedValue() == 6) {  // Orange
									DrawUtils::drawRectangle(rectPos, MC_Color(255, 127, 0), 2);
								} else if (hudMod->color.getSelectedValue() == 7) {  // Yellow
									DrawUtils::drawRectangle(rectPos, MC_Color(255, 255, 0), 2);
								} else if (hudMod->color.getSelectedValue() == 8) {  // Green
									DrawUtils::drawRectangle(rectPos, MC_Color(0, 255, 0), 2);
								} else if (hudMod->color.getSelectedValue() == 9) {  // Blue
									DrawUtils::drawRectangle(rectPos, MC_Color(0, 170, 255), 2);
								} else if (hudMod->color.getSelectedValue() == 11) {  // Purple
									DrawUtils::drawRectangle(rectPos, MC_Color(148, 0, 211), 2);
								} else if (hudMod->color.getSelectedValue() == 12) {  // Pink
									DrawUtils::drawRectangle(rectPos, MC_Color(255, 192, 203), 1);
								}
								if (hudMod->color.getSelectedValue() != 1)  // Rainbow
									DrawUtils::drawText(vec2_t(pPos), &tempStr, MC_Color(currColor), 1.5, 1, true);
								if (hudMod->color.getSelectedValue() == 1) {  // Dynamic
									DrawUtils::drawText(pPos, &tempStr, MC_Color(dynamic, dynamic, dynamic), 1.5, 1, true);
								} else if (hudMod->color.getSelectedValue() == 4) {  // White
									DrawUtils::drawText(pPos, &tempStr, MC_Color(255, 255, 255), 1.5, 1, true);
								} else if (hudMod->color.getSelectedValue() == 5) {  // Red
									DrawUtils::drawText(pPos, &tempStr, MC_Color(255, 0, 0), 1.5, 1, true);
								} else if (hudMod->color.getSelectedValue() == 6) {  // Orange
									DrawUtils::drawText(pPos, &tempStr, MC_Color(255, 127, 0), 1.5, 1, true);
								} else if (hudMod->color.getSelectedValue() == 7) {  // Yellow
									DrawUtils::drawText(pPos, &tempStr, MC_Color(255, 255, 0), 1.5, 1, true);
								} else if (hudMod->color.getSelectedValue() == 8) {  // Green
									DrawUtils::drawText(pPos, &tempStr, MC_Color(0, 255, 0), 1.5, 1, true);
								} else if (hudMod->color.getSelectedValue() == 9) {  // Blue
									DrawUtils::drawText(pPos, &tempStr, MC_Color(0, 170, 255), 1.5, 1, true);
								} else if (hudMod->color.getSelectedValue() == 11) {  // Purple
									DrawUtils::drawText(pPos, &tempStr, MC_Color(148, 0, 211), 1.5, 1, true);
								} else if (hudMod->color.getSelectedValue() == 12) {  // Pink
									DrawUtils::drawText(pPos, &tempStr, MC_Color(255, 192, 203), 1.5, 1, true);
								}
							}
						}
					}
				}
			}
		}
	}

	{
		// Main Menu
		std::string screenName(g_Hooks.currentScreenName);
		if (strcmp(screenName.c_str(), "start_screen") == 0) {
			// Draw Custom Geo Button
			if (g_Data.allowWIPFeatures() && g_Data.isInjectorConnectionActive()) {
				if (HImGui.Button("Load Script Folder", vec2_t(wid.x * (0.765f - 0.5f), wid.y * 0.92f), true)) {
					HorionDataPacket packet;
					packet.cmd = CMD_FOLDERCHOOSER;
					auto tmp = std::shared_ptr<unsigned char[]>(new unsigned char[300]);
					packet.data.swap(tmp);
					memset(packet.data.get(), 0, 300);
					strcpy_s((char*)packet.data.get(), 200, "{\"title\": \"Select a Script Folder\", \"filter\":\".js\"}");
					packet.dataArraySize = (int)strlen((char*)packet.data.get());
					packet.params[0] = g_Data.addInjectorResponseCallback([](std::shared_ptr<HorionDataPacket> pk) {
						if (pk->params[0] != 1) {  // Dialog Canceled, reset geo
							auto box = g_Data.addInfoBox("Scripting: Invalid Folder");
							box->closeTimer = 1;
							return;
						}

						wchar_t* jsonData = reinterpret_cast<wchar_t*>(pk->data.get());
						std::wstring jsonDataStr(jsonData);

						json parsed = json::parse(jsonDataStr);
						if (parsed["path"].is_string()) {
							auto box = g_Data.addInfoBox("Importing Script, Please wait...");
							std::thread gamer([parsed, box]() {
								auto result = scriptMgr.importScriptFolder(parsed["path"].get<std::string>());
								if (result)
									box->fadeTarget = 0;
								else {
									box->message = "Script import error, \ncheck the console";
									box->closeTimer = 2;
								}
							});
							gamer.detach();
						}
					});

					g_Data.sendPacketToInjector(packet);
				}
				if (HImGui.Button("Custom Geometry", vec2_t(wid.x * 0.765f, wid.y * 0.92f), true)) {
					HorionDataPacket packet;
					packet.cmd = CMD_FILECHOOSER;
					auto tmp = std::shared_ptr<unsigned char[]>(new unsigned char[300]);
					packet.data.swap(tmp);
					memset(packet.data.get(), 0, 300);
					strcpy_s((char*)packet.data.get(), 200, "{\"title\": \"Select a 3d object\", \"filter\":\"Object Files (*.obj)|*.obj\"}");
					packet.dataArraySize = (int)strlen((char*)packet.data.get());
					packet.params[0] = g_Data.addInjectorResponseCallback([](std::shared_ptr<HorionDataPacket> pk) {
						if (pk->params[0] != 1 && std::get<0>(g_Data.getCustomGeoOverride())) {  // Dialog Canceled, reset geo
							auto box = g_Data.addInfoBox("Geometry override removed");
							box->closeTimer = 1;
							return;
						}

						wchar_t* jsonData = reinterpret_cast<wchar_t*>(pk->data.get());
						std::wstring jsonDataStr(jsonData);

						json parsed = json::parse(jsonDataStr);
						if (parsed["path"].is_string()) {
							auto box = g_Data.addInfoBox("Importing Skin, Please wait...");
							std::thread gamer([parsed, box]() {
								SkinUtil::importGeo(Utils::stringToWstring(parsed["path"].get<std::string>()));
								box->fadeTarget = 0;
							});
							gamer.detach();
						}
					});

					g_Data.sendPacketToInjector(packet);
				}
				if (HImGui.Button("Custom Texture", vec2_t(wid.x * 0.5f, wid.y * 0.92f), true)) {
					HorionDataPacket packet;
					packet.cmd = CMD_FILECHOOSER;
					auto tmp = std::shared_ptr<unsigned char[]>(new unsigned char[500]);
					packet.data.swap(tmp);
					memset(packet.data.get(), 0, 500);
					strcpy_s((char*)packet.data.get(), 400, "{\"title\": \"Select a raw image file\", \"filter\":\"Raw image files (*.data, *.raw)|*.data;*.raw\"}");
					packet.dataArraySize = (int)strlen((char*)packet.data.get());
					packet.params[0] = g_Data.addInjectorResponseCallback([](std::shared_ptr<HorionDataPacket> pk) {
						if (pk->params[0] != 1 && std::get<0>(g_Data.getCustomTextureOverride())) {  // Dialog Canceled, reset texture
							auto box = g_Data.addInfoBox("Texture override removed");
							box->closeTimer = 1;
							return;
						}

						wchar_t* jsonData = reinterpret_cast<wchar_t*>(pk->data.get());
						std::wstring jsonDataStr(jsonData);

						json parsed = json::parse(jsonDataStr);
						if (parsed["path"].is_string()) {
							auto box = g_Data.addInfoBox("Importing texture...");
							std::thread gamer([parsed, box]() {
								auto contents = Utils::readFileContents(Utils::stringToWstring(parsed["path"].get<std::string>()));
								if (contents.size() > 0) {
									auto texturePtr = std::shared_ptr<unsigned char[]>(new unsigned char[contents.size() + 1]);
									memcpy(texturePtr.get(), contents.c_str(), contents.size());
									texturePtr.get()[contents.size()] = 0;
									g_Data.setCustomTextureOverride(true, std::make_shared<std::tuple<std::shared_ptr<unsigned char[]>, size_t>>(texturePtr, contents.size()));
									box->message = "Success";
									box->closeTimer = 0.3f;
								} else {
									box->message = "Error!";
									box->message = "Could not read texture file (empty?)";
									box->closeTimer = 2.f;
								}
							});
							gamer.detach();
						}
					});

					g_Data.sendPacketToInjector(packet);
				}
			}

		} else {
			shouldRenderTabGui = hudMod->tabgui && hudMod->isEnabled();
			shouldRenderArrayList = arraylistMod->arraylist && arraylistMod->isEnabled();
			//shouldRenderWatermark = hudMod->watermark && hudMod->isEnabled();

			//CLICKGUI::RENDER clickgui render
			if (clickGuiModule->isEnabled()) {
				ClickGui::render();
				shouldPostRender = true;
				shouldRenderArrayList = false;
				shouldRenderTabGui = false;
				shouldRenderWatermark = false;
			}

			if (shouldRenderTabGui) TabGui::render();

			{
				// Display ArrayList on the Right?
				static constexpr bool isOnSomeFuckingSide = true;

				float yOffset = 0;  // Offset of next Text
				vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
				vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;

				vec2_t mousePos = *g_Data.getClientInstance()->getMousePos();
				mousePos.div(windowSizeReal);
				mousePos.mul(windowSize);

				// Draw ArrayList
				if (moduleMgr->isInitialized() && shouldRenderArrayList) {
					//Parameters
					float textSize = hudMod->scale;
					float textPadding = 0.6f * textSize;
					float textHeight = 10.0f * textSize;
					float smoothness = 2;

					struct IModuleContainer {
						// Struct used to Sort IModules in a std::set
						std::shared_ptr<IModule> backingModule;
						std::string moduleName;
						bool enabled;
						int keybind;
						float textWidth;
						vec2_t* pos;
						bool shouldRender = true;

						IModuleContainer(std::shared_ptr<IModule> mod) {
							auto arrayList = moduleMgr->getModule<ArrayList>();
							if (arrayList->modes) {
								const char* moduleNameChr = mod->getModuleName();
								moduleName = moduleNameChr;
							} else {
								const char* moduleNameChr = mod->getRawModuleName();
								moduleName = moduleNameChr;
							}

							this->enabled = mod->isEnabled();
							this->keybind = mod->getKeybind();
							this->backingModule = mod;
							this->pos = mod->getPos();

							if (!this->enabled && *this->pos == vec2_t(0.f, 0.f))
								this->shouldRender = false;
							this->textWidth = DrawUtils::getTextWidth(&moduleName, arraylistMod->scale);
						}

						bool operator<(const IModuleContainer& other) const {
							/*if (enabled) {
						if (!other.enabled)  // We are enabled
							return true;
					} else if (other.enabled)  // They are enabled
						return false;*/

							if (this->textWidth == other.textWidth)
								return moduleName < other.moduleName;
							return this->textWidth > other.textWidth;
						}
					};

					// Mouse click detector
					static bool wasLeftMouseDown = GameData::isLeftClickDown();  // Last isDown value
					bool leftMouseDown = GameData::isLeftClickDown();            // current isDown value

					bool executeClick = leftMouseDown && leftMouseDown != wasLeftMouseDown;  // isDown == true AND (current state IS NOT last state)
					wasLeftMouseDown = leftMouseDown;                                        // Set last isDown value

					std::set<IModuleContainer> modContainerList;
					// Fill modContainerList with Modules
					{
						auto lock = moduleMgr->lockModuleList();
						std::vector<std::shared_ptr<IModule>>* moduleList = moduleMgr->getModuleList();
						for (auto it : *moduleList) {
							if (it.get() != arraylistMod)
								if (it.get() != hudMod)
#ifdef _DEBUG
									if (it.get() != hudEditorMod)
#endif
										if (it.get() != watermark)
											if (it.get() != clickGuiModule)
												modContainerList.emplace(IModuleContainer(it));
						}
					}

					int a = 0;
					int b = 0;
					int c = 0;

					// Loop through mods to display Labels
					float lastTextLenght = 0.f;
					vec4_t underline;
					for (std::set<IModuleContainer>::iterator it = modContainerList.begin(); it != modContainerList.end(); ++it) {
						if (!it->shouldRender)
							continue;

						std::string textStr = it->moduleName;
						float textWidth = it->textWidth;

						float xOffsetOri = windowSize.x - textWidth - (textPadding * 2);

						float xOffset = windowSize.x - it->pos->x;
						float barOffset = windowSize.x;
						float barSize = windowSize.x;

						it->pos->x += smoothness;

						if (xOffset < xOffsetOri) {
							xOffset = xOffsetOri;
						}
						if (!it->enabled) {
							xOffset += it->pos->y;
							it->pos->y +=
								smoothness;
						}
						if (xOffset >= windowSize.x && !it->enabled) {
							it->pos->x = 0.f;
							it->pos->y = 0.f;
						}
						vec2_t textPos = vec2_t(
							xOffset + textPadding,
							yOffset + textPadding);
						vec4_t rectPos = vec4_t(
							xOffset - 2,
							yOffset,
							isOnSomeFuckingSide ? windowSize.x : textWidth + (textPadding * 2),
							yOffset + textPadding * 2 + textHeight);
						vec4_t leftRect = vec4_t(
							xOffset - 2,
							yOffset,
							xOffset - 1,
							yOffset + textPadding * 2 + textHeight);
						vec2_t textShadow = vec2_t(
							xOffset + textPadding + 1,
							yOffset + textPadding + 1);
						vec4_t barBG = vec4_t(
							xOffset - 4,
							yOffset,
							isOnSomeFuckingSide ? windowSize.x : textWidth + (textPadding * 2),
							yOffset + textPadding * 2 + textHeight);
						vec2_t barTextShadow = vec2_t(
							xOffset + textPadding - 1,
							yOffset + textPadding + 1);
						vec2_t barTextPos = vec2_t(
							xOffset + textPadding - 2,
							yOffset + textPadding);
						vec4_t bar = vec4_t(
							barSize - 1,
							yOffset,
							barOffset,
							yOffset + textPadding * 2 + textHeight);

						underline = vec4_t(windowSize.x - (lastTextLenght + 2.f + (textPadding * 2.f)), leftRect.y, leftRect.x, leftRect.y + 1.f);

						lastTextLenght = textWidth;
						c++;
						b++;
						if (b < 20)
							a = moduleMgr->getEnabledModuleCount() * 2;
						else
							b = 0;
						currColor[3] = rcolors[3];
						if (hudMod->color.getSelectedValue() == 0) {  // Rainbow
							Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[2], rcolors[1], currColor[0], currColor[1], currColor[2]);
							currColor[0] += 1 / rectPos.x * rectPos.y;
							Utils::ColorConvertHSVtoRGB(currColor[0], currColor[2], currColor[3], currColor[0], currColor[1], currColor[2]);
						}
						if (hudMod->color.getSelectedValue() == 2) {  // Horion
							Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], currColor[0], currColor[1], currColor[2]);
							currColor[0] += 1.f / a * c;
							Utils::ColorConvertHSVtoRGB(currColor[0], currColor[1], currColor[2], currColor[0], currColor[1], currColor[2]);
						}
						if (hudMod->color.getSelectedValue() == 3) {  // Fadeaway
							Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], currColor[0], currColor[1], currColor[2]);
							currColor[0] += 1.f / a * c;
							Utils::ColorConvertHSVtoRGB(currColor[0], currColor[1], currColor[2], currColor[0], currColor[3], currColor[3]);
						}
						//if (hudModuleTwo->color.getSelectedValue()) { // Weather (Unused)
						//Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], currColor[0], currColor[2], currColor[2]);
						//currColor[0] += 1.1f / a * b;
						//Utils::ColorConvertHSVtoRGB(currColor[0], currColor[2], currColor[3], currColor[0], currColor[0], currColor[1]);
						//}

						if (hudMod->color.getSelectedValue() == 1) {  // Dynamic
							dynamic += (isIncreasingcurrColor ? colorIncrease : -colorIncrease);
							if (dynamic < 0.2f) {
								dynamic = 0.2f;
								isIncreasingcurrColor = true;
							}
							if (dynamic > 1.f) {
								dynamic = 1.f;
								isIncreasingcurrColor = false;
							}
						}

						auto arrayList = moduleMgr->getModule<ArrayList>();
						if (arraylistMod->mode.getSelectedValue() == 2) {                         // Bar
							DrawUtils::fillRectangle(barBG, MC_Color(0, 0, 0), arrayList->bgOp);  //ArrayList background
						} else {
							DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), arrayList->bgOp);  //ArrayList background
						}

						// ArrayList Modes
						// Outline
						if (arraylistMod->mode.getSelectedValue() == 0) {
							if (hudMod->color.getSelectedValue() != 1)  // Rainbow
								DrawUtils::fillRectangle(leftRect, MC_Color(currColor), 1.f);
							DrawUtils::fillRectangle(underline, MC_Color(currColor), 1.f);
							if (hudMod->color.getSelectedValue() == 1) {  // Dynamic
								DrawUtils::fillRectangle(leftRect, MC_Color(dynamic, dynamic, dynamic), 1.f);
								DrawUtils::fillRectangle(underline, MC_Color(dynamic, dynamic, dynamic), 1.f);
							} else if (hudMod->color.getSelectedValue() == 4) {  // White
								DrawUtils::fillRectangle(leftRect, MC_Color(255, 255, 255), 1.f);
								DrawUtils::fillRectangle(underline, MC_Color(255, 255, 255), 1.f);
							} else if (hudMod->color.getSelectedValue() == 5) {  // Red
								DrawUtils::fillRectangle(leftRect, MC_Color(255, 0, 0), 1.f);
								DrawUtils::fillRectangle(underline, MC_Color(255, 0, 0), 1.f);
							} else if (hudMod->color.getSelectedValue() == 6) {  // Orange
								DrawUtils::fillRectangle(leftRect, MC_Color(255, 127, 0), 1.f);
								DrawUtils::fillRectangle(underline, MC_Color(255, 127, 0), 1.f);
							} else if (hudMod->color.getSelectedValue() == 7) {  // Yellow
								DrawUtils::fillRectangle(leftRect, MC_Color(255, 255, 0), 1.f);
								DrawUtils::fillRectangle(underline, MC_Color(255, 255, 0), 1.f);
							} else if (hudMod->color.getSelectedValue() == 8) {  // Green
								DrawUtils::fillRectangle(leftRect, MC_Color(0, 255, 0), 1.f);
								DrawUtils::fillRectangle(underline, MC_Color(0, 255, 0), 1.f);
							} else if (hudMod->color.getSelectedValue() == 9) {  // Blue
								DrawUtils::fillRectangle(leftRect, MC_Color(0, 170, 255), 1.f);
								DrawUtils::fillRectangle(underline, MC_Color(0, 170, 255), 1.f);
							} else if (hudMod->color.getSelectedValue() == 11) {  // Purple
								DrawUtils::fillRectangle(leftRect, MC_Color(148, 0, 211), 1.f);
								DrawUtils::fillRectangle(underline, MC_Color(148, 0, 211), 1.f);
							} else if (hudMod->color.getSelectedValue() == 12) {  // Pink
								DrawUtils::fillRectangle(leftRect, MC_Color(255, 192, 203), 1.f);
								DrawUtils::fillRectangle(underline, MC_Color(255, 192, 203), 1.f);
							}
						}

						// Split
						if (arraylistMod->mode.getSelectedValue() == 1) {
							if (hudMod->color.getSelectedValue() != 1)  // Rainbow
								DrawUtils::fillRectangle(leftRect, MC_Color(currColor), 1.f);
							if (hudMod->color.getSelectedValue() == 1) {  // Dynamic
								DrawUtils::fillRectangle(leftRect, MC_Color(dynamic, dynamic, dynamic), 1.f);
							} else if (hudMod->color.getSelectedValue() == 4) {  // White
								DrawUtils::fillRectangle(leftRect, MC_Color(255, 255, 255), 1.f);
							} else if (hudMod->color.getSelectedValue() == 5) {  // Red
								DrawUtils::fillRectangle(leftRect, MC_Color(255, 0, 0), 1.f);
							} else if (hudMod->color.getSelectedValue() == 6) {  // Orange
								DrawUtils::fillRectangle(leftRect, MC_Color(255, 127, 0), 1.f);
							} else if (hudMod->color.getSelectedValue() == 7) {  // Yellow
								DrawUtils::fillRectangle(leftRect, MC_Color(255, 255, 0), 1.f);
							} else if (hudMod->color.getSelectedValue() == 8) {  // Green
								DrawUtils::fillRectangle(leftRect, MC_Color(0, 255, 0), 1.f);
							} else if (hudMod->color.getSelectedValue() == 9) {  // Blue
								DrawUtils::fillRectangle(leftRect, MC_Color(0, 170, 255), 1.f);
							} else if (hudMod->color.getSelectedValue() == 11) {  // Purple
								DrawUtils::fillRectangle(leftRect, MC_Color(148, 0, 211), 1.f);
							} else if (hudMod->color.getSelectedValue() == 12) {  // Pink
								DrawUtils::fillRectangle(leftRect, MC_Color(255, 192, 203), 1.f);
							}
						}

						// Bar
						if (arraylistMod->mode.getSelectedValue() == 2) {
							if (hudMod->color.getSelectedValue() != 1)  // Rainbow
								DrawUtils::fillRectangle(bar, MC_Color(currColor), 1.f);
							if (hudMod->color.getSelectedValue() == 1) {  // Dynamic
								DrawUtils::fillRectangle(bar, MC_Color(dynamic, dynamic, dynamic), 1.f);
							} else if (hudMod->color.getSelectedValue() == 4) {  // White
								DrawUtils::fillRectangle(bar, MC_Color(255, 255, 255), 1.f);
							}
							if (hudMod->color.getSelectedValue() == 5) {  // Red
								DrawUtils::fillRectangle(bar, MC_Color(255, 0, 0), 1.f);
							} else if (hudMod->color.getSelectedValue() == 6) {  // Orange
								DrawUtils::fillRectangle(bar, MC_Color(255, 127, 0), 1.f);
							} else if (hudMod->color.getSelectedValue() == 7) {  // Yellow
								DrawUtils::fillRectangle(bar, MC_Color(255, 255, 0), 1.f);
							} else if (hudMod->color.getSelectedValue() == 8) {  // Green
								DrawUtils::fillRectangle(bar, MC_Color(0, 255, 0), 1.f);
							} else if (hudMod->color.getSelectedValue() == 9) {  // Blue
								DrawUtils::fillRectangle(bar, MC_Color(0, 170, 255), 1.f);
							} else if (hudMod->color.getSelectedValue() == 11) {  // Purple
								DrawUtils::fillRectangle(bar, MC_Color(148, 0, 211), 1.f);
							} else if (hudMod->color.getSelectedValue() == 12) {  // Pink
								DrawUtils::fillRectangle(bar, MC_Color(255, 192, 203), 1.f);
							}
						}

						underline = vec4_t(windowSize.x - (textWidth + 2.f + (textPadding * 2.f)), leftRect.w, windowSize.x, leftRect.w + 1.f);

						if (!GameData::canUseMoveKeys() && rectPos.contains(&mousePos) && arraylistMod->clickToggle) {
							vec4_t selectedRect = rectPos;
							selectedRect.x = leftRect.z;
							if (leftMouseDown) {
								DrawUtils::fillRectangle(selectedRect, MC_Color(0.8f, 0.8f, 0.8f), 0.8f);
								if (executeClick)
									it->backingModule->toggle();
							} else
								DrawUtils::fillRectangle(selectedRect, MC_Color(0.8f, 0.8f, 0.8f, 0.8f), 0.3f);
						}

						if (arraylistMod->mode.getSelectedValue() == 2) {  // Bar
							if (hudMod->color.getSelectedValue() != 1)     // currColor
								DrawUtils::drawText(vec2_t(barTextPos), &textStr, MC_Color(currColor), textSize, 1, true);
							if (hudMod->color.getSelectedValue() == 1) {  // Dynamic
								DrawUtils::drawText(barTextPos, &textStr, MC_Color(dynamic, dynamic, dynamic), textSize, 1, true);
							} else if (hudMod->color.getSelectedValue() == 4) {  // White
								DrawUtils::drawText(barTextPos, &textStr, MC_Color(255, 255, 255), textSize, 1, true);
							} else if (hudMod->color.getSelectedValue() == 5) {  // Red
								DrawUtils::drawText(barTextPos, &textStr, MC_Color(255, 0, 0), textSize, 1, true);
							} else if (hudMod->color.getSelectedValue() == 6) {  // Orange
								DrawUtils::drawText(barTextPos, &textStr, MC_Color(255, 127, 0), textSize, 1, true);
							} else if (hudMod->color.getSelectedValue() == 7) {  // Yellow
								DrawUtils::drawText(barTextPos, &textStr, MC_Color(255, 255, 0), textSize, 1, true);
							} else if (hudMod->color.getSelectedValue() == 8) {  // Green
								DrawUtils::drawText(barTextPos, &textStr, MC_Color(0, 255, 0), textSize, 1, true);
							} else if (hudMod->color.getSelectedValue() == 9) {  // Blue
								DrawUtils::drawText(barTextPos, &textStr, MC_Color(0, 170, 255), textSize, 1, true);
							} else if (hudMod->color.getSelectedValue() == 11) {  // Purple
								DrawUtils::drawText(barTextPos, &textStr, MC_Color(148, 0, 211), textSize, 1, true);
							} else if (hudMod->color.getSelectedValue() == 12) {  // Pink
								DrawUtils::drawText(barTextPos, &textStr, MC_Color(255, 192, 203), textSize, 1, true);
							}
						} else {
							if (hudMod->color.getSelectedValue() != 1)  // Rainbow
								DrawUtils::drawText(vec2_t(textPos), &textStr, MC_Color(currColor), textSize, 1, true);
							if (hudMod->color.getSelectedValue() == 1) {  // Dynamic
								DrawUtils::drawText(textPos, &textStr, MC_Color(dynamic, dynamic, dynamic), textSize, 1, true);
							} else if (hudMod->color.getSelectedValue() == 4) {  // White
								DrawUtils::drawText(textPos, &textStr, MC_Color(255, 255, 255), textSize, 1, true);
							} else if (hudMod->color.getSelectedValue() == 5) {  // Red
								DrawUtils::drawText(textPos, &textStr, MC_Color(255, 0, 0), textSize, 1, true);
							} else if (hudMod->color.getSelectedValue() == 6) {  // Orange
								DrawUtils::drawText(textPos, &textStr, MC_Color(255, 127, 0), textSize, 1, true);
							} else if (hudMod->color.getSelectedValue() == 7) {  // Yellow
								DrawUtils::drawText(textPos, &textStr, MC_Color(255, 255, 0), textSize, 1, true);
							} else if (hudMod->color.getSelectedValue() == 8) {  // Green
								DrawUtils::drawText(textPos, &textStr, MC_Color(0, 255, 0), textSize, 1, true);
							} else if (hudMod->color.getSelectedValue() == 9) {  // Blue
								DrawUtils::drawText(textPos, &textStr, MC_Color(0, 170, 255), textSize, 1, true);
							} else if (hudMod->color.getSelectedValue() == 11) {  // Purple
								DrawUtils::drawText(textPos, &textStr, MC_Color(148, 0, 211), textSize, 1, true);
							} else if (hudMod->color.getSelectedValue() == 12) {  // Pink
								DrawUtils::drawText(textPos, &textStr, MC_Color(255, 192, 203), textSize, 1, true);
							}
						}

						yOffset += textHeight + (textPadding * 2);
					}

					//Bottom Line
					if (arraylistMod->mode.getSelectedValue() == 0)
						if (hudMod->color.getSelectedValue() != 1)  // Rainbow
							DrawUtils::fillRectangle(underline, MC_Color(currColor), 1.f);
					if (hudMod->color.getSelectedValue() == 1 && arraylistMod->mode.getSelectedValue() == 0) {  // Dynamic
						DrawUtils::fillRectangle(underline, MC_Color(dynamic, dynamic, dynamic), 1.f);
					} else if (hudMod->color.getSelectedValue() == 4 && arraylistMod->mode.getSelectedValue() == 0) {  // White
						DrawUtils::fillRectangle(underline, MC_Color(255, 255, 255), 1.f);
					} else if (hudMod->color.getSelectedValue() == 5 && arraylistMod->mode.getSelectedValue() == 0) {  // Red
						DrawUtils::fillRectangle(underline, MC_Color(255, 0, 0), 1.f);
					} else if (hudMod->color.getSelectedValue() == 6 && arraylistMod->mode.getSelectedValue() == 0) {  // Orange
						DrawUtils::fillRectangle(underline, MC_Color(255, 127, 0), 1.f);
					} else if (hudMod->color.getSelectedValue() == 7 && arraylistMod->mode.getSelectedValue() == 0) {  // Yellow
						DrawUtils::fillRectangle(underline, MC_Color(255, 255, 0), 1.f);
					} else if (hudMod->color.getSelectedValue() == 8 && arraylistMod->mode.getSelectedValue() == 0) {  // Green
						DrawUtils::fillRectangle(underline, MC_Color(0, 255, 0), 1.f);
					} else if (hudMod->color.getSelectedValue() == 9 && arraylistMod->mode.getSelectedValue() == 0) {  // Blue
						DrawUtils::fillRectangle(underline, MC_Color(0, 170, 255), 1.f);
					} else if (hudMod->color.getSelectedValue() == 11 && arraylistMod->mode.getSelectedValue() == 0) {  // Purple
						DrawUtils::fillRectangle(underline, MC_Color(148, 0, 211), 1.f);
					} else if (hudMod->color.getSelectedValue() == 12 && arraylistMod->mode.getSelectedValue() == 0) {  // Pink
						DrawUtils::fillRectangle(underline, MC_Color(255, 192, 203), 1.f);
					}

					c = 0;
					modContainerList.clear();
				}
			}
		}
	}

	// Zoom calc
	{
		static auto zoomModule = moduleMgr->getModule<Zoom>();
		if (zoomModule->isEnabled()) zoomModule->target = zoomModule->strength;
		zoomModule->modifier = zoomModule->target - ((zoomModule->target - zoomModule->modifier) * 0.8f);
		if (abs(zoomModule->modifier - zoomModule->target) < 0.1f && !zoomModule->isEnabled())
			zoomModule->zooming = false;
	}

	if (shouldPostRender) moduleMgr->onPostRender(renderCtx);
	HImGui.endFrame();
	DrawUtils::flush();

	// Draw HUD
	if (g_Data.canUseMoveKeys() && g_Data.isInGame() && hudMod->isEnabled()) {
		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
		float startY = windowSize.y - 51;
		// FPS
		if (hudMod->fps) {
			std::string fpsText = "FPS: " + std::to_string(g_Data.getFPS());

			float l = DrawUtils::getTextWidth(&fpsText, 1) + 6.5;

			vec4_t rectPos = vec4_t(2.5, startY + 6, l, startY + 20);
			vec2_t textPos = vec2_t(rectPos.x + 2, rectPos.y + 3);

			DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), hudMod->opacity);

			if (hudMod->color.getSelectedValue() != 1)  // Rainbowf
				DrawUtils::drawText(textPos, &fpsText, MC_Color(currColor), 1, 1, true);
			if (hudMod->color.getSelectedValue() == 1) {  // Dynamic
				DrawUtils::drawText(textPos, &fpsText, MC_Color(dynamic, dynamic, dynamic), 1, 1, true);
			} else if (hudMod->color.getSelectedValue() == 4) {  // White
				DrawUtils::drawText(textPos, &fpsText, MC_Color(255, 255, 255), 1, 1, true);
			} else if (hudMod->color.getSelectedValue() == 5) {  // Red
				DrawUtils::drawText(textPos, &fpsText, MC_Color(255, 0, 0), 1, 1, true);
			} else if (hudMod->color.getSelectedValue() == 6) {  // Orange
				DrawUtils::drawText(textPos, &fpsText, MC_Color(255, 127, 0), 1, 1, true);
			} else if (hudMod->color.getSelectedValue() == 7) {  // Yellow
				DrawUtils::drawText(textPos, &fpsText, MC_Color(255, 255, 0), 1, 1, true);
			} else if (hudMod->color.getSelectedValue() == 8) {  // Green
				DrawUtils::drawText(textPos, &fpsText, MC_Color(0, 255, 0), 1, 1, true);
			} else if (hudMod->color.getSelectedValue() == 9) {  // Blue
				DrawUtils::drawText(textPos, &fpsText, MC_Color(0, 170, 255), 1, 1, true);
			} else if (hudMod->color.getSelectedValue() == 11) {  // Purple
				DrawUtils::drawText(textPos, &fpsText, MC_Color(148, 0, 211), 1, 1, true);
			} else if (hudMod->color.getSelectedValue() == 12) {  // Pink
				DrawUtils::drawText(textPos, &fpsText, MC_Color(255, 192, 203), 1, 1, true);
			}
		}

		// Armor HUD
		{
			if (hudMod->displayArmor) {
				vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
				static float constexpr opacity = 1;
				float scale = 3 * 0.26f;
				float spacing = scale + 15.f + 2;
				std::string lenlol = "                      ";  // add more spaces for longer rect lol
				std::string armor = "Armor:";
				float startY = windowSize.y - 37;
				float l = DrawUtils::getTextWidth(&lenlol, 1) + 6.5;
				vec4_t rectPos = vec4_t(2.5, startY + 6, l, startY + 20);
				vec2_t textPos = vec2_t(rectPos.x + 35, rectPos.y + 1);
				vec2_t textPos2 = vec2_t(rectPos.x + 2, rectPos.y + 3);
				DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), hudMod->opacity);
				C_LocalPlayer* player = g_Data.getLocalPlayer();

				if (hudMod->color.getSelectedValue() != 1)  // Rainbow
					DrawUtils::drawText(textPos2, &armor, MC_Color(currColor), 1, 1, true);
				if (hudMod->color.getSelectedValue() == 1) {  // Dynamic
					DrawUtils::drawText(textPos2, &armor, MC_Color(dynamic, dynamic, dynamic), 1, 1, true);
				} else if (hudMod->color.getSelectedValue() == 4) {  // White
					DrawUtils::drawText(textPos2, &armor, MC_Color(255, 255, 255), 1, 1, true);
				} else if (hudMod->color.getSelectedValue() == 5) {  // Red
					DrawUtils::drawText(textPos2, &armor, MC_Color(255, 0, 0), 1, 1, true);
				} else if (hudMod->color.getSelectedValue() == 6) {  // Orange
					DrawUtils::drawText(textPos2, &armor, MC_Color(255, 127, 0), 1, 1, true);
				} else if (hudMod->color.getSelectedValue() == 7) {  // Yellow
					DrawUtils::drawText(textPos2, &armor, MC_Color(255, 255, 0), 1, 1, true);
				} else if (hudMod->color.getSelectedValue() == 8) {  // Green
					DrawUtils::drawText(textPos2, &armor, MC_Color(0, 255, 0), 1, 1, true);
				} else if (hudMod->color.getSelectedValue() == 9) {  // Blue
					DrawUtils::drawText(textPos2, &armor, MC_Color(0, 170, 255), 1, 1, true);
				} else if (hudMod->color.getSelectedValue() == 11) {  // Purple
					DrawUtils::drawText(textPos2, &armor, MC_Color(148, 0, 211), 1, 1, true);
				} else if (hudMod->color.getSelectedValue() == 12) {  // Pink
					DrawUtils::drawText(textPos2, &armor, MC_Color(255, 192, 203), 1, 1, true);
				}

				for (int t = 0; t < 4; t++) {
					C_ItemStack* stack = player->getArmor(t);
					if (stack->isValid()) {
						DrawUtils::drawItem(stack, vec2_t(textPos), 1, scale, false);
						textPos.x += scale * spacing;
					}
				}
			}
		}

		// Coords
		{
			if (hudMod->coordinates) {
				vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
				vec3_t* pos = g_Data.getLocalPlayer()->getPos();

				std::string position = "Position: " + std::to_string((int)floorf(pos->x)) + " " + std::to_string((int)floorf(pos->y)) + " " + std::to_string((int)floorf(pos->z));

				float startY = windowSize.y - 23;
				float l = DrawUtils::getTextWidth(&position, 1) + 6.5;

				vec4_t rectPos = vec4_t(2.5, startY + 6, l, startY + 20);
				vec2_t textPos = vec2_t(rectPos.x + 2, rectPos.y + 3);

				DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), hudMod->opacity);

				if (hudMod->color.getSelectedValue() != 1)  // Rainbow
					DrawUtils::drawText(textPos, &position, MC_Color(currColor), 1, 1, true);
				if (hudMod->color.getSelectedValue() == 1) {  // Dynamic
					DrawUtils::drawText(textPos, &position, MC_Color(dynamic, dynamic, dynamic), 1, 1, true);
				} else if (hudMod->color.getSelectedValue() == 4) {  // White
					DrawUtils::drawText(textPos, &position, MC_Color(255, 255, 255), 1, 1, true);
				} else if (hudMod->color.getSelectedValue() == 5) {  // Red
					DrawUtils::drawText(textPos, &position, MC_Color(255, 0, 0), 1, 1, true);
				} else if (hudMod->color.getSelectedValue() == 6) {  // Orange
					DrawUtils::drawText(textPos, &position, MC_Color(255, 127, 0), 1, 1, true);
				} else if (hudMod->color.getSelectedValue() == 7) {  // Yellow
					DrawUtils::drawText(textPos, &position, MC_Color(255, 255, 0), 1, 1, true);
				} else if (hudMod->color.getSelectedValue() == 8) {  // Green
					DrawUtils::drawText(textPos, &position, MC_Color(0, 255, 0), 1, 1, true);
				} else if (hudMod->color.getSelectedValue() == 9) {  // Blue
					DrawUtils::drawText(textPos, &position, MC_Color(0, 170, 255), 1, 1, true);
				} else if (hudMod->color.getSelectedValue() == 11) {  // Purple
					DrawUtils::drawText(textPos, &position, MC_Color(148, 0, 211), 1, 1, true);
				} else if (hudMod->color.getSelectedValue() == 12) {  // Pink
					DrawUtils::drawText(textPos, &position, MC_Color(255, 192, 203), 1, 1, true);
				}
			}
		}
	}

	// Draw Notifications
	{
		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
		auto notifications = moduleMgr->getModule<Notifications>();
		auto hudMod = moduleMgr->getModule<HudModule>();
		auto box = g_Data.getFreshInfoBox();
		if (box) {
			box->fade();
			if (box->fadeTarget == 1 && box->closeTimer <= 0 && box->closeTimer > -1)
				box->fadeTarget = 0;
			else if (box->closeTimer > 0 && box->fadeVal > 0.9f)
				box->closeTimer -= 1.f / 60;

			const float titleTextSize = box->fadeVal * 1;
			const float messageTextSize = box->fadeVal * 1;
			const float titleTextHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * titleTextSize;
			vec2_t* pos;
			int lines = 1;

			std::string substring = box->message;

			while (lines < 5) {
				auto brea = substring.find("\n");
				if (brea == std::string::npos || brea + 1 >= substring.size())
					break;
				substring = substring.substr(brea + 1);
				lines++;
			}
			if (box->message.size() == 0)
				lines = 0;

			constexpr float boxMessage = 1;
			constexpr float unused = 0.7f;
			static const float textHeight = (boxMessage + unused * 1) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
			constexpr float borderPadding = 1;
			constexpr float margin = 5;

			static std::string version = "";

			float nameLength = DrawUtils::getTextWidth(&substring, boxMessage);
			float fullTextLength = nameLength + DrawUtils::getTextWidth(&version, unused) + 2;

			if (box->closeTimer <= 1 && box->closeTimer > -1) {
				vec4_t rect2 = vec4_t(
					windowSize.x - box->closeTimer * 100,
					windowSize.y - margin - textHeight + 4,
					windowSize.x - box->closeTimer,
					windowSize.y - margin);
				vec2_t textPos = vec2_t(rect2.x + 1.5, rect2.y + 3);
				DrawUtils::drawText(vec2_t(textPos.x + borderPadding, textPos.y), &substring, MC_Color(255, 255, 255), 1, 1, true);
				DrawUtils::fillRectangle(rect2, MC_Color(0, 0, 0), notifications->opacity);
			}

			vec4_t rect = vec4_t(
				windowSize.x - margin - fullTextLength - 2 - borderPadding * 2,
				windowSize.y - margin - textHeight + 4,
				windowSize.x - margin + borderPadding - 2,
				windowSize.y - margin);

			vec2_t textPos = vec2_t(rect.x + 1.5, rect.y + 3);
			if (box->closeTimer > 1) {
				DrawUtils::setColor(1, 1, 1, 1);
				DrawUtils::drawLine(vec2_t(rect.x + fullTextLength - box->closeTimer * 7, rect.y + 14), vec2_t(rect.x + fullTextLength + 3, rect.y + 14), 1), vec2_t(rect.x - box->closeTimer, rect.y + 14);
				DrawUtils::drawText(vec2_t(textPos.x + borderPadding, textPos.y), &substring, MC_Color(255, 255, 255), 1, 1, true);
				DrawUtils::fillRectangle(rect, MC_Color(0, 0, 0), notifications->opacity);
			}
		}
	}
	DrawUtils::flush();

	return retval;
}

float* Hooks::Dimension_getFogColor(__int64 _this, float* color, __int64 a3, float a4) {
	static auto oGetFogColor = g_Hooks.Dimension_getFogColorHook->GetFastcall<float*, __int64, float*, __int64, float>();
	static auto customSky = moduleMgr->getModule<CustomSky>();
	static float rcolors[4];

	if (customSky->isEnabled()) {
		if (customSky->rainbow) {
			if (rcolors[3] < 1) {
				rcolors[0] = 1;
				rcolors[1] = 0.2f;
				rcolors[2] = 0.2f;
				rcolors[3] = 1;
			}
			if (customSky->rainbow) {
				Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);
				rcolors[0] += 0.001f;
				if (rcolors[0] >= 1)
					rcolors[0] = 0;
				Utils::ColorConvertHSVtoRGB(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);
				return rcolors;
			}
		} else {
			color[0] = customSky->red;
			color[1] = customSky->green;
			color[2] = customSky->blue;
			return color;
		}
	}
	return oGetFogColor(_this, color, a3, a4);
}

float Hooks::Dimension_getTimeOfDay(__int64 _this, int a2, float a3) {
	static auto oGetTimeOfDay = g_Hooks.Dimension_getTimeOfDayHook->GetFastcall<float, __int64, int, float>();

	static auto timeChange = moduleMgr->getModule<TimeChanger>();
	if (timeChange->isEnabled()) {
		return timeChange->modifier;
	}

	return oGetTimeOfDay(_this, a2, a3);
}

float Hooks::Dimension_getSunIntensity(__int64 a1, float a2, vec3_t* a3, float a4) {
	static auto oGetSunIntensity = g_Hooks.Dimension_getSunIntensityHook->GetFastcall<float, __int64, float, vec3_t*, float>();

	return oGetSunIntensity(a1, a2, a3, a4);
}

void Hooks::ChestBlockActor_tick(C_ChestBlockActor* _this, void* a) {
	static auto oTick = g_Hooks.ChestBlockActor_tickHook->GetFastcall<void, C_ChestBlockActor*, void*>();
	oTick(_this, a);
	static auto* chestEspMod = moduleMgr->getModule<ChestESP>();
	if (_this != nullptr && chestEspMod->isEnabled())
		GameData::addChestToList(_this);
}

void Hooks::Actor_lerpMotion(C_Entity* _this, vec3_t motVec) {
	static auto oLerp = g_Hooks.Actor_lerpMotionHook->GetFastcall<void, C_Entity*, vec3_t>();

	if (g_Data.getLocalPlayer() != _this)
		return oLerp(_this, motVec);

	static auto noKnockbackmod = moduleMgr->getModule<Velocity>();
	if (noKnockbackmod->isEnabled()) {
		static void* networkSender = nullptr;
		if (!networkSender)
			networkSender = reinterpret_cast<void*>(9 + FindSignature("48 8B CB FF ?? ?? ?? ?? 00 C6 47 ?? 01 48 8B 5C 24"));

		if (networkSender == _ReturnAddress()) {
			motVec = _this->velocity.lerp(motVec, noKnockbackmod->xModifier, noKnockbackmod->yModifier, noKnockbackmod->xModifier);
		}
	}

	oLerp(_this, motVec);
}

void Hooks::PleaseAutoComplete(__int64 a1, __int64 a2, TextHolder* text, int a4) {
	static auto oAutoComplete = g_Hooks.PleaseAutoCompleteHook->GetFastcall<void, __int64, __int64, TextHolder*, int>();
	char* tx = text->getText();

	using syncShit_t = void(__fastcall*)(__int64*, TextHolder*);
	static syncShit_t syncShit = nullptr;
	if (syncShit == nullptr) {
		uintptr_t sigOffset = 0;
		// sig of function: (present 3 times in the exe)
		sigOffset = FindSignature("48 8B D7 48 8B 8B ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 9C") + 11;
		auto funcOffset = *reinterpret_cast<int*>(sigOffset);
		sigOffset += 4 + funcOffset;

		syncShit = reinterpret_cast<syncShit_t>(sigOffset);
	}

	if (tx != nullptr && text->getTextLength() >= 1 && tx[0] == '.') {
		std::string search = tx + 1;                                              // Dont include the '.'
		std::transform(search.begin(), search.end(), search.begin(), ::tolower);  // make the search text lowercase

		struct LilPlump {
			std::string cmdAlias;
			IMCCommand* command = 0;
			bool shouldReplace = true;  // Should replace the current text in the box (autocomplete)

			bool operator<(const LilPlump& o) const {
				return cmdAlias < o.cmdAlias;
			}
		};  // This is needed so the std::set sorts it alphabetically

		std::set<LilPlump> searchResults;

		std::vector<IMCCommand*>* commandList = cmdMgr->getCommandList();
		for (auto it = commandList->begin(); it != commandList->end(); ++it) {  // Loop through commands
			IMCCommand* c = *it;
			auto* aliasList = c->getAliasList();
			for (auto it = aliasList->begin(); it != aliasList->end(); ++it) {  // Loop through aliases
				std::string cmd = *it;
				LilPlump plump;

				for (size_t i = 0; i < search.size(); i++) {  // Loop through search string
					char car = search.at(i);
					if (car == ' ' && i == cmd.size()) {
						plump.shouldReplace = false;
						break;
					} else if (i >= cmd.size())
						goto outerContinue;

					if (car != cmd.at(i))  // and compare
						goto outerContinue;
				}
				// Not at outerContinue? Then we got a good result!
				{
					cmd.insert(0, 1, '.');  // Prepend the '.'

					plump.cmdAlias = cmd;
					plump.command = c;
					searchResults.emplace(plump);
				}

			outerContinue:
				continue;
			}
		}

		if (!searchResults.empty()) {
			LilPlump firstResult = *searchResults.begin();

			size_t maxReplaceLength = firstResult.cmdAlias.size();
			if (searchResults.size() > 1) {
				for (auto it = searchResults.begin()++; it != searchResults.end(); it++) {
					auto alias = it->cmdAlias;
					maxReplaceLength = std::min(maxReplaceLength, alias.size());

					for (int i = 0; i < maxReplaceLength; i++) {
						if (alias[i] != firstResult.cmdAlias[i]) {
							maxReplaceLength = i;
							break;
						}
					}
				}
			} else
				maxReplaceLength = firstResult.cmdAlias.size();

			g_Data.getGuiData()->displayClientMessageF("==========");
			if (searchResults.size() > 1) {
				for (auto it = searchResults.begin(); it != searchResults.end(); ++it) {
					LilPlump plump = *it;
					g_Data.getGuiData()->displayClientMessageF("%s%s - %s%s", plump.cmdAlias.c_str(), GRAY, ITALIC, plump.command->getDescription());
				}
			}
			if (firstResult.command->getUsage(firstResult.cmdAlias.c_str() + 1)[0] == 0)
				g_Data.getGuiData()->displayClientMessageF("%s%s %s- %s", WHITE, firstResult.cmdAlias.c_str(), GRAY, firstResult.command->getDescription());
			else
				g_Data.getGuiData()->displayClientMessageF("%s%s %s %s- %s", WHITE, firstResult.cmdAlias.c_str(), firstResult.command->getUsage(firstResult.cmdAlias.c_str() + 1 /*exclude prefix*/), GRAY, firstResult.command->getDescription());

			if (firstResult.shouldReplace) {
				if (search.size() == firstResult.cmdAlias.size() - 1 && searchResults.size() == 1) {
					maxReplaceLength++;
					firstResult.cmdAlias.append(" ");
				}

				text->setText(firstResult.cmdAlias.substr(0, maxReplaceLength));  // Set text
				// now sync with the UI thread
				syncShit(0, text);
			}
		}

		return;
	}
	oAutoComplete(a1, a2, text, a4);
}

void Hooks::LoopbackPacketSender_sendToServer(C_LoopbackPacketSender* a, C_Packet* packet) {
	static auto oFunc = g_Hooks.LoopbackPacketSender_sendToServerHook->GetFastcall<void, C_LoopbackPacketSender*, C_Packet*>();

	static auto sneakMod = moduleMgr->getModule<Sneak>();
	static auto speed = moduleMgr->getModule<Speed>();
	static auto freecamMod = moduleMgr->getModule<Freecam>();
	static auto flight = moduleMgr->getModule<Flight>();
	static auto disabler = moduleMgr->getModule<Disabler>();
	static auto freetpMod = moduleMgr->getModule<FreeTP>();
	static auto blinkMod = moduleMgr->getModule<Blink>();
	//static auto noPacketMod = moduleMgr->getModule<Packetsfbgh>();
	static auto packetMod = moduleMgr->getModule<Packet>();

	if (packetMod->isEnabled() && g_Data.isInGame() && packetMod->noPacket)
		return;

	if (packet->getId() == 9) {
		C_TextPacket* tp = (C_TextPacket*)packet;
		if (tp->messageType == 69) {
			tp->messageType = 1;
			return oFunc(a, packet);
		}

		if (tp->message.getText()[0] == cmdMgr->prefix)
			return cmdMgr->execute(tp->message.getText());

		else if (tp->message.getText()[0] == '.') {
			// maybe the user forgot his prefix, give him some helpful advice
			static bool helpedUser = false;
			if (!helpedUser) {
				helpedUser = true;
				g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%sYour prefix is: \"%s%c%s\"", RED, GRAY, cmdMgr->prefix, RED);
				g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%sEnter \"%s%cprefix .%s\" to reset your prefix", RED, GRAY, cmdMgr->prefix, RED);
				return;
			}
		}
	}

	if (freecamMod->isEnabled() || blinkMod->isEnabled()) {
		if (packet->isInstanceOf<C_MovePlayerPacket>() || packet->isInstanceOf<PlayerAuthInputPacket>()) {
			if (blinkMod->isEnabled()) {
				if (packet->isInstanceOf<C_MovePlayerPacket>()) {
					C_MovePlayerPacket* meme = reinterpret_cast<C_MovePlayerPacket*>(packet);
					meme->onGround = true;                                                            //Don't take Fall Damages when turned off
					blinkMod->getMovePlayerPacketHolder()->push_back(new C_MovePlayerPacket(*meme));  // Saving the packets
				} else {
					char* npacket = new char[sizeof(PlayerAuthInputPacket)];
					memcpy(npacket, packet, sizeof(PlayerAuthInputPacket));
					blinkMod->getPlayerAuthInputPacketHolder()->push_back((PlayerAuthInputPacket*)npacket);
				}
			}
			return;  // Dont call LoopbackPacketSender_sendToServer
		}
	} else if (!blinkMod->isEnabled()) {
		if (blinkMod->getMovePlayerPacketHolder()->size() > 0) {
			for (auto it : *blinkMod->getMovePlayerPacketHolder()) {
				oFunc(a, (it));
				delete it;
				it = nullptr;
			}
			blinkMod->getMovePlayerPacketHolder()->clear();
			return;
		}
		if (blinkMod->getPlayerAuthInputPacketHolder()->size() > 0) {
			for (PlayerAuthInputPacket* it : *blinkMod->getPlayerAuthInputPacketHolder()) {
				memset((int*)&it->yawUnused + 2, 0, 0x5C);
				oFunc(a, (it));
				delete it;
				it = nullptr;
			}
			blinkMod->getPlayerAuthInputPacketHolder()->clear();
			return;
		}
	}

	if (flight->isEnabled() || blinkMod->isEnabled()) {
		if (flight->blink) {
			if (packet->isInstanceOf<C_MovePlayerPacket>() || packet->isInstanceOf<PlayerAuthInputPacket>()) {
				if (blinkMod->isEnabled()) {
					if (packet->isInstanceOf<C_MovePlayerPacket>()) {
						C_MovePlayerPacket* meme = reinterpret_cast<C_MovePlayerPacket*>(packet);
						meme->onGround = true;                                                            //Don't take Fall Damages when turned off
						blinkMod->getMovePlayerPacketHolder()->push_back(new C_MovePlayerPacket(*meme));  // Saving the packets
					} else {
						char* npacket = new char[sizeof(PlayerAuthInputPacket)];
						memcpy(npacket, packet, sizeof(PlayerAuthInputPacket));
						blinkMod->getPlayerAuthInputPacketHolder()->push_back((PlayerAuthInputPacket*)npacket);
					}
				}
				return;  // Dont call LoopbackPacketSender_sendToServer
			}
		}
	} else if (!blinkMod->isEnabled()) {
		if (flight->blink) {
			if (blinkMod->getMovePlayerPacketHolder()->size() > 0) {
				for (auto it : *blinkMod->getMovePlayerPacketHolder()) {
					oFunc(a, (it));
					delete it;
					it = nullptr;
				}
				blinkMod->getMovePlayerPacketHolder()->clear();
				return;
			}
			if (blinkMod->getPlayerAuthInputPacketHolder()->size() > 0) {
				for (PlayerAuthInputPacket* it : *blinkMod->getPlayerAuthInputPacketHolder()) {
					memset((int*)&it->yawUnused + 2, 0, 0x5C);
					oFunc(a, (it));
					delete it;
					it = nullptr;
				}
				blinkMod->getPlayerAuthInputPacketHolder()->clear();
				return;
			}
		}
	}

	if (freetpMod->isEnabled() || blinkMod->isEnabled()) {
		if (packet->isInstanceOf<C_MovePlayerPacket>() || packet->isInstanceOf<PlayerAuthInputPacket>()) {
			if (blinkMod->isEnabled()) {
				if (packet->isInstanceOf<C_MovePlayerPacket>()) {
					C_MovePlayerPacket* meme = reinterpret_cast<C_MovePlayerPacket*>(packet);
					meme->onGround = true;                                                            //Don't take Fall Damages when turned off
					blinkMod->getMovePlayerPacketHolder()->push_back(new C_MovePlayerPacket(*meme));  // Saving the packets
				} else {
					char* npacket = new char[sizeof(PlayerAuthInputPacket)];
					memcpy(npacket, packet, sizeof(PlayerAuthInputPacket));
					blinkMod->getPlayerAuthInputPacketHolder()->push_back((PlayerAuthInputPacket*)npacket);
				}
			}
			return;  // Dont call LoopbackPacketSender_sendToServer
		}
	} else if (!blinkMod->isEnabled()) {
		if (blinkMod->getMovePlayerPacketHolder()->size() > 0) {
			for (auto it : *blinkMod->getMovePlayerPacketHolder()) {
				oFunc(a, (it));
				delete it;
				it = nullptr;
			}
			blinkMod->getMovePlayerPacketHolder()->clear();
			return;
		}
		if (blinkMod->getPlayerAuthInputPacketHolder()->size() > 0) {
			for (PlayerAuthInputPacket* it : *blinkMod->getPlayerAuthInputPacketHolder()) {
				oFunc(a, (it));
				delete it;
				it = nullptr;
			}
			blinkMod->getPlayerAuthInputPacketHolder()->clear();
			return;
		}

		if (disabler->isEnabled() && disabler->mode.getSelectedValue() == 2 || blinkMod->isEnabled()) {
			if (packet->isInstanceOf<C_MovePlayerPacket>() || packet->isInstanceOf<PlayerAuthInputPacket>()) {
				if (blinkMod->isEnabled()) {
					if (packet->isInstanceOf<C_MovePlayerPacket>()) {
						C_MovePlayerPacket* meme = reinterpret_cast<C_MovePlayerPacket*>(packet);
						meme->onGround = true;                                                            //Don't take Fall Damages when turned off
						blinkMod->getMovePlayerPacketHolder()->push_back(new C_MovePlayerPacket(*meme));  // Saving the packets
					} else {
						blinkMod->getPlayerAuthInputPacketHolder()->push_back(new PlayerAuthInputPacket(*reinterpret_cast<PlayerAuthInputPacket*>(packet)));
					}
				}
				return;  // Dont call LoopbackPacketSender_sendToServer
			}
		} else if (!blinkMod->isEnabled()) {
			if (blinkMod->getMovePlayerPacketHolder()->size() > 0) {
				for (auto it : *blinkMod->getMovePlayerPacketHolder()) {
					oFunc(a, (it));
					delete it;
					it = nullptr;
				}
				blinkMod->getMovePlayerPacketHolder()->clear();
				return;
			}
			if (blinkMod->getPlayerAuthInputPacketHolder()->size() > 0) {
				for (auto it : *blinkMod->getPlayerAuthInputPacketHolder()) {
					oFunc(a, (it));
					delete it;
					it = nullptr;
				}
				blinkMod->getPlayerAuthInputPacketHolder()->clear();
				return;
			}
		}
	}

	if (speed->mode.getSelectedValue() == 1 && speed->isEnabled() && !speed->isOnGround && g_Data.isInGame()) {
	}

	moduleMgr->onSendPacket(packet);

	if (!packetMod->isEnabled()) {
		oFunc(a, packet);
	} else {
		if (g_Data.canUseMoveKeys() && packetMod->multiply) {
			if (packetMod->isEnabled()) {
				oFunc(a, packet);
				oFunc(a, packet);
			}
			if (packetMod->fourx && packetMod->isEnabled() && packetMod->multiply) {
				oFunc(a, packet);
				oFunc(a, packet);
			}
		} else {
			oFunc(a, packet);
		}
	}
}

float Hooks::LevelRendererPlayer_getFov(__int64 _this, float a2, bool a3) {
	static auto oGetFov = g_Hooks.LevelRendererPlayer_getFovHook->GetFastcall<float, __int64, float, bool>();
	static void* renderItemInHand = reinterpret_cast<void*>(FindSignature("F3 0F 59 05 ?? ?? ?? ?? 45 0F 28 C2 F3"));

	static void* setupCamera = reinterpret_cast<void*>(FindSignature("0F 28 F8 F3 0F 59 3D ?? ?? ?? ?? F3 0F 11 7C 24 24 F3 0F 10 87"));

	static auto zoomModule = moduleMgr->getModule<Zoom>();

	void* retAddr = _ReturnAddress();

	if (_ReturnAddress() == renderItemInHand) {
		return oGetFov(_this, a2, a3);
	}
	if (_ReturnAddress() == setupCamera) {
		g_Data.fov = -oGetFov(_this, a2, a3) + 110.f;
		if (moduleMgr->isInitialized()) {
			if (!zoomModule->smooth && zoomModule->isEnabled()) return -zoomModule->target + 110.f;
			if (zoomModule->smooth && zoomModule->zooming) return -zoomModule->modifier + 110.f;
		}
		return oGetFov(_this, a2, a3);
	}
#ifdef _DEBUG
	logF("LevelRendererPlayer_getFov Return Address: %llX", _ReturnAddress());
	//__debugbreak();  // IF we reach here, a sig is broken
#endif
	return oGetFov(_this, a2, a3);
}

void Hooks::MultiLevelPlayer_tick(C_EntityList* _this) {
	static auto oTick = g_Hooks.MultiLevelPlayer_tickHook->GetFastcall<void, C_EntityList*>();
	oTick(_this);
	GameData::EntityList_tick(_this);
}

void Hooks::GameMode_startDestroyBlock(C_GameMode* _this, vec3_ti* a2, uint8_t face, void* a4, void* a5) {
	static auto oFunc = g_Hooks.GameMode_startDestroyBlockHook->GetFastcall<void, C_GameMode*, vec3_ti*, uint8_t, void*, void*>();

	static auto nukerModule = moduleMgr->getModule<Nuker>();
	static auto instaBreakModule = moduleMgr->getModule<InstaBreak>();

	if (nukerModule->isEnabled()) {
		vec3_ti tempPos;

		int range = nukerModule->getNukerRadius();
		const bool isVeinMiner = nukerModule->isVeinMiner();
		const bool tree = nukerModule->treeMode();
		const bool isAutoMode = nukerModule->isAutoMode();

		C_BlockSource* region = g_Data.getLocalPlayer()->region;
		auto selectedBlockId = ((region->getBlock(*a2)->blockLegacy))->blockId;
		uint8_t selectedBlockData = region->getBlock(*a2)->data;

		if (!isAutoMode && !tree) {
			for (int x = -range; x < range; x++) {
				for (int y = -range; y < range; y++) {
					for (int z = -range; z < range; z++) {
						tempPos.x = a2->x + x;
						tempPos.y = a2->y + y;
						tempPos.z = a2->z + z;
						if (tempPos.y > 0) {
							C_Block* blok = region->getBlock(tempPos);
							uint8_t data = blok->data;
							auto id = blok->blockLegacy->blockId;
							if (blok->blockLegacy->material->isSolid == true && (!isVeinMiner || (id == selectedBlockId && data == selectedBlockData)))
								_this->destroyBlock(&tempPos, face);
						}
					}
				}
			}
		}
		return;
	}
	if (instaBreakModule->isEnabled()) {
		_this->destroyBlock(a2, face);
		return;
	}

	oFunc(_this, a2, face, a4, a5);
}

void Hooks::HIDController_keyMouse(C_HIDController* _this, void* a2, void* a3) {
	static auto oFunc = g_Hooks.HIDController_keyMouseHook->GetFastcall<void, C_HIDController*, void*, void*>();
	GameData::setHIDController(_this);
	isTicked = true;
	oFunc(_this, a2, a3);
	return;
}

int Hooks::BlockLegacy_getRenderLayer(C_BlockLegacy* a1) {
	static auto oFunc = g_Hooks.BlockLegacy_getRenderLayerHook->GetFastcall<int, C_BlockLegacy*>();

	static auto xrayMod = moduleMgr->getModule<Xray>();
	if (xrayMod->isEnabled()) {
		char* text = a1->name.getText();
		if (strstr(text, "ore") == NULL)
			if (strcmp(text, "lava") != NULL)
				if (strcmp(text, "water") != NULL)
					if (strcmp(text, "portal") != NULL)
						if (strcmp(text, "amethyst_block") != NULL)
							if (strcmp(text, "ancient_debris") != NULL)
								if (strcmp(text, "command_block") != NULL)
									if (strcmp(text, "repeating_command_block") != NULL)
										if (strcmp(text, "chain_command_block") != NULL)
											if (strcmp(text, "structure_block") != NULL)
												if (strcmp(text, "deny") != NULL)
													if (strcmp(text, "allow") != NULL)
														if (strcmp(text, "bedrock") != NULL)
															if (strcmp(text, "gravel") != NULL)
																if (strcmp(text, "border_block") != NULL)
																	return 10;
	}
	return oFunc(a1);
}

__int64 Hooks::LevelRenderer_renderLevel(__int64 _this, __int64 a2, __int64 a3) {
	static auto oFunc = g_Hooks.LevelRenderer_renderLevelHook->GetFastcall<__int64, __int64, __int64, __int64>();

	using reloadShit_t = void(__fastcall*)(__int64);
	static reloadShit_t reloadChunk = nullptr;

	if (!reloadChunk) {
		// RenderChunkCoordinator::rebuildAllRenderChunkGeometry
		reloadChunk = reinterpret_cast<reloadShit_t>(FindSignature("48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 48 83 EC ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 48 8B F9 48 8D A9"));
	}

	static auto xrayMod = moduleMgr->getModule<Xray>();

	static bool lastState = false;
	if (lastState != xrayMod->isEnabled()) {
		// LevelRenderer::rebuildAllRenderChunkGeometry
		lastState = xrayMod->isEnabled();
		unsigned long long* v5;  // rdi
		unsigned long long* i;   // rbx

		v5 = *(unsigned long long**)(_this + 32);
		for (i = (unsigned long long*)*v5; i != v5; i = (unsigned long long*)*i)
			reloadChunk(i[3]);
	}

	auto ret = oFunc(_this, a2, a3);

	DrawUtils::setGameRenderContext(a2);
	moduleMgr->onLevelRender();
	DrawUtils::setGameRenderContext(0);

	return ret;
}

void Hooks::ClickFunc(__int64 a1, char mouseButton, char isDown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY, char a8) {
	static auto oFunc = g_Hooks.ClickFuncHook->GetFastcall<void, __int64, char, char, __int16, __int16, __int16, __int16, char>();
	static auto clickGuiModule = moduleMgr->getModule<ClickGuiMod>();

	//MouseButtons
	//0 = mouse move
	//1 = left click
	//2 = right click
	//3 = middle click
	//4 = scroll   (isDown: 120 (SCROLL UP) and -120 (SCROLL DOWN))

	ClickGui::onMouseClickUpdate((int)mouseButton, isDown);
	HImGui.onMouseClickUpdate((int)mouseButton, isDown);

	if (isDown)
		if (mouseButton == 1)
			g_Data.leftclickCount++;
		else if (mouseButton == 2)
			g_Data.rightclickCount++;

	if (clickGuiModule->isEnabled()) {
		if (mouseButton == 4) {
			ClickGui::onWheelScroll(isDown > 0);
		}
		if (mouseButton != 0)  // Mouse click event
			return;
	}
	return oFunc(a1, mouseButton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);
}

__int64 Hooks::MoveInputHandler_tick(C_MoveInputHandler* a1, C_Entity* a2) {
	static auto oTick = g_Hooks.MoveInputHandler_tickHook->GetFastcall<__int64, C_MoveInputHandler*, C_Entity*>();

	auto ret = oTick(a1, a2);
	moduleMgr->onMove(a1);

	return 0;
}

__int64 Hooks::ChestScreenController_tick(C_ChestScreenController* a1) {
	static auto oFunc = g_Hooks.ChestScreenController_tickHook->GetFastcall<__int64, C_ChestScreenController*>();

	static auto chestStealerMod = moduleMgr->getModule<ChestStealer>();
	if (chestStealerMod->isEnabled()) chestStealerMod->chestScreenController_tick(a1);

	return oFunc(a1);
}

float Hooks::GetGamma(uintptr_t* a1) {
	static auto cameraMod = moduleMgr->getModule<CameraMod>();
	static auto xrayMod = moduleMgr->getModule<Xray>();
	static auto nametagmod = moduleMgr->getModule<NameTags>();

	uintptr_t** list = (uintptr_t**)a1;

	char obtainedSettings = 0;
	bool hadIt = false;
	for (uint16_t i = 3; i < 450; i++) {
		if (list[i] == nullptr) continue;
		uintptr_t* info = *(uintptr_t**)((uintptr_t)list[i] + 8);
		if (info == nullptr) continue;

		TextHolder* translateName = (TextHolder*)((uintptr_t)info + 0x1D8);
		TextHolder* settingname = (TextHolder*)((uintptr_t)info + 0x188);

		if (!strcmp(translateName->getText(), "options.smoothlighting")) {
			if (hadIt) continue;
			bool* smoothlightning = (bool*)((uintptr_t)list[i] + 16);
			xrayMod->smoothLightningSetting = smoothlightning;
			obtainedSettings++;
			hadIt = true;
		} else if (!strcmp(settingname->getText(), "gfx_ingame_player_names")) {
			bool* ingamePlayerName = (bool*)((uintptr_t)list[i] + 16);
			nametagmod->ingameNametagSetting = ingamePlayerName;
			obtainedSettings++;
		}

		if (obtainedSettings == 2) break;
	}

	if (xrayMod->isEnabled())
		return 25.f;

	if (cameraMod->isEnabled() && cameraMod->fullbright)
		return cameraMod->fbAmount;

	static auto ofunc = g_Hooks.GetGammaHook->GetFastcall<float, uintptr_t*>();
	return ofunc(a1);
}

__int64 Hooks::MinecraftGame_onAppSuspended(__int64 _this) {
	static auto oFunc = g_Hooks.MinecraftGame_onAppSuspendedHook->GetFastcall<__int64, __int64>();
	configMgr->saveConfig();
	return oFunc(_this);
}

//void Hooks::Actor_startSwimming(C_Entity* _this) {
//static auto oFunc = g_Hooks.Actor_startSwimmingHook->GetFastcall<void, C_Entity*>();

//static auto jesusModule = moduleMgr->getModule<Jesus>();
//if (jesusModule->isEnabled() && g_Data.getLocalPlayer() == _this)
//return;

//oFunc(_this);
//}

void Hooks::RakNetInstance_tick(C_RakNetInstance* _this, __int64 a2, __int64 a3) {
	static auto oTick = g_Hooks.RakNetInstance_tickHook->GetFastcall<void, C_RakNetInstance*, __int64, __int64>();
	GameData::setRakNetInstance(_this);
	oTick(_this, a2, a3);
}

float Hooks::GameMode_getPickRange(C_GameMode* _this, __int64 a2, char a3) {
	static auto oFunc = g_Hooks.GameMode_getPickRangeHook->GetFastcall<float, C_GameMode*, __int64, char>();

	if (g_Data.getLocalPlayer() != nullptr) {
		static auto blockreachMod = moduleMgr->getModule<BlockReach>();
		static auto clickTP = moduleMgr->getModule<ClickTP>();

		if (blockreachMod->isEnabled())
			return 250;

		if (clickTP->isEnabled())
			return 255;
	}

	return oFunc(_this, a2, a3);
}

__int64 Hooks::ConnectionRequest_create(__int64 _this, __int64 privateKeyManager, void* a3, TextHolder* selfSignedId, TextHolder* serverAddress, __int64 clientRandomId, TextHolder* skinId, SkinData* skinData, __int64 capeData, CoolSkinData* coolSkinStuff, TextHolder* deviceId, int inputMode, int uiProfile, int guiScale, TextHolder* languageCode, bool sendEduModeParams, TextHolder* tenantId, __int64 unused, TextHolder* platformUserId, TextHolder* thirdPartyName, bool thirdPartyNameOnly, TextHolder* platformOnlineId, TextHolder* platformOfflineId, TextHolder* capeId) {
	static auto oFunc = g_Hooks.ConnectionRequest_createHook->GetFastcall<__int64, __int64, __int64, void*, TextHolder*, TextHolder*, __int64, TextHolder*, SkinData*, __int64, CoolSkinData*, TextHolder*, int, int, int, TextHolder*, bool, TextHolder*, __int64, TextHolder*, TextHolder*, bool, TextHolder*, TextHolder*, TextHolder*>();

	auto geoOverride = g_Data.getCustomGeoOverride();

	if (g_Data.allowWIPFeatures()) {
		logF("Connection Request: InputMode: %i UiProfile: %i GuiScale: %i", inputMode, uiProfile, guiScale);

		//Logger::WriteBigLogFileF(skinGeometryData->getTextLength() + 20, "Geometry: %s", skinGeometryData->getText());
		auto hResourceGeometry = FindResourceA((HMODULE)g_Data.getDllModule(), MAKEINTRESOURCEA(IDR_TEXT1), "TEXT");
		auto hMemoryGeometry = LoadResource((HMODULE)g_Data.getDllModule(), hResourceGeometry);

		auto sizeGeometry = SizeofResource((HMODULE)g_Data.getDllModule(), hResourceGeometry);
		auto ptrGeometry = LockResource(hMemoryGeometry);

		auto hResourceSteve = FindResourceA((HMODULE)g_Data.getDllModule(), MAKEINTRESOURCEA(IDR_STEVE), (char*)RT_RCDATA);
		auto hMemorySteve = LoadResource((HMODULE)g_Data.getDllModule(), hResourceSteve);

		auto sizeSteve = SizeofResource((HMODULE)g_Data.getDllModule(), hResourceSteve);
		auto ptrSteve = LockResource(hMemorySteve);

		//std::unique_ptr<TextHolder> newGeometryData(new TextHolder(ptrGeometry, sizeGeometry));
		TextHolder* newGeometryData = nullptr;

		if (std::get<0>(geoOverride)) {  // Is overriding geometry
			auto overrideGeo = std::get<1>(geoOverride);
			newGeometryData = new TextHolder(*overrideGeo.get());
		} else {  // Default Skin
				  /*char* str;  // Obj text
			{
				auto hResourceObj = FindResourceA(g_Data.getDllModule(), MAKEINTRESOURCEA(IDR_OBJ), "TEXT");
				auto hMemoryObj = LoadResource(g_Data.getDllModule(), hResourceObj);

				auto sizeObj = SizeofResource(g_Data.getDllModule(), hResourceObj);
				auto ptrObj = LockResource(hMemoryObj);

				str = new char[sizeObj + 1];
				memset(str, 0, sizeObj + 1);
				memcpy(str, ptrObj, sizeObj);
			}

			newGeometryData = new TextHolder(SkinUtil::modGeometry(reinterpret_cast<char*>(ptrGeometry), SkinUtil::objToMesh(str)));*/
		}

		SkinData* newSkinData = new SkinData();
		newSkinData->SkinWidth = 128;
		newSkinData->SkinHeight = 128;
		newSkinData->skinData = ptrSteve;
		newSkinData->skinSize = sizeSteve;

		auto texOverride = g_Data.getCustomTextureOverride();
		auto texture = std::get<1>(texOverride);  // Put it here so it won't go out of scope until after it has been used
		if (std::get<0>(texOverride)) {           // Enabled
			newSkinData->skinData = std::get<0>(*texture.get()).get();
			newSkinData->skinSize = std::get<1>(*texture.get());
		}

		//Logger::WriteBigLogFileF(newGeometryData->getTextLength() + 20, "Geometry: %s", newGeometryData->getText());
		TextHolder* newSkinResourcePatch = new TextHolder(Utils::base64_decode("ewogICAiZ2VvbWV0cnkiIDogewogICAgICAiYW5pbWF0ZWRfZmFjZSIgOiAiZ2VvbWV0cnkuYW5pbWF0ZWRfZmFjZV9wZXJzb25hXzRjZGJiZmFjYTI0YTk2OGVfMF8wIiwKICAgICAgImRlZmF1bHQiIDogImdlb21ldHJ5LnBlcnNvbmFfNGNkYmJmYWNhMjRhOTY4ZV8wXzAiCiAgIH0KfQo="));

		TextHolder* fakeName = g_Data.getFakeName();
		TextHolder resourcePatchBackup;

		if (newGeometryData != nullptr) {
			memcpy(&resourcePatchBackup, &coolSkinStuff->skinResourcePatch, sizeof(TextHolder));
			memcpy(&coolSkinStuff->skinResourcePatch, newSkinResourcePatch, sizeof(TextHolder));
			styledReturnText = *newGeometryData;
			overrideStyledReturn = true;
		}

		//  newGeometryData == nullptr ? skinResourcePatch : newSkinResourcePatch, newGeometryData == nullptr ? skinGeometryData : newGeometryData, skinAnimationData, isPremiumSkin, isPersonaSkin,
		__int64 res = oFunc(_this, privateKeyManager, a3, selfSignedId, serverAddress, clientRandomId, skinId, (newGeometryData == nullptr && !std::get<0>(texOverride)) ? skinData : newSkinData, capeData, coolSkinStuff, deviceId, inputMode, uiProfile, guiScale, languageCode, sendEduModeParams, tenantId, unused, platformUserId, fakeName != nullptr ? fakeName : thirdPartyName, fakeName != nullptr ? true : thirdPartyNameOnly, platformOnlineId, platformOfflineId, capeId);

		if (newGeometryData != nullptr) {
			memcpy(&coolSkinStuff->skinResourcePatch, &resourcePatchBackup, sizeof(TextHolder));
			resourcePatchBackup.resetWithoutDelete();
		}
		overrideStyledReturn = false;

		styledReturnText = TextHolder();

		if (hMemoryGeometry)
			FreeResource(hMemoryGeometry);
		if (hMemorySteve)
			FreeResource(hMemorySteve);

		if (newGeometryData)
			delete newGeometryData;
		delete newSkinData;
		delete newSkinResourcePatch;
		return res;
	} else {
		TextHolder* fakeName = g_Data.getFakeName();
		__int64 res = oFunc(_this, privateKeyManager, a3, selfSignedId, serverAddress, clientRandomId, skinId, skinData, capeData, coolSkinStuff, deviceId, inputMode, uiProfile, guiScale, languageCode, sendEduModeParams, tenantId, unused, platformUserId, fakeName != nullptr ? fakeName : thirdPartyName, fakeName != nullptr ? true : thirdPartyNameOnly, platformOnlineId, platformOfflineId, capeId);
		return res;
	}
}

void Hooks::InventoryTransactionManager_addAction(C_InventoryTransactionManager* a1, C_InventoryAction* a2) {
	static auto Func = g_Hooks.InventoryTransactionManager_addActionHook->GetFastcall<void, C_InventoryTransactionManager*, C_InventoryAction*>();
	Func(a1, a2);
}

bool Hooks::DirectoryPackAccessStrategy__isTrusted(__int64 _this) {
	static auto func = g_Hooks.DirectoryPackAccessStrategy__isTrustedHook->GetFastcall<bool, __int64>();

	static uintptr_t** directoryPackAccessStrategyVtable = 0;

	if (!directoryPackAccessStrategyVtable) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 49 89 06 49 8D 76 50");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		directoryPackAccessStrategyVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + 7);
	}

	if (*reinterpret_cast<uintptr_t*>(_this) == (uintptr_t)directoryPackAccessStrategyVtable)
		return true;

	return func(_this);
}

bool Hooks::ReturnTrue(__int64 _this) {
	return true;
}

__int64 Hooks::SkinRepository___loadSkinPack(__int64 _this, __int64 pack, __int64 a3) {
	static auto func = g_Hooks.SkinRepository___loadSkinPackHook->GetFastcall<__int64, __int64, __int64, __int64>();

	//auto res = (*(unsigned __int8 (**)(void))(**(__int64**)(pack + 8) + 48i64))();
	//logF("SkinRepository___loadSkinPack: origin %i, is Trusted: %i", *(int*)((*(__int64*)pack) + 888i64), res);
	*(int*)((*(__int64*)pack) + 888i64) = 2;  // Set pack origin to "2"

	return func(_this, pack, a3);
}

GamerTextHolder* Hooks::toStyledString(__int64 strIn, GamerTextHolder* strOut) {
	static auto func = g_Hooks.toStyledStringHook->GetFastcall<GamerTextHolder*, __int64, GamerTextHolder*>();

	static auto conRequest = reinterpret_cast<__int64>(g_Hooks.ConnectionRequest_createHook->funcPtr);
	if (reinterpret_cast<__int64>(_ReturnAddress()) > conRequest && reinterpret_cast<__int64>(_ReturnAddress()) < conRequest + 10000 && overrideStyledReturn) {
		// Return address is within boundaries of connection request function

		strOut->copyFrom(&styledReturnText);

		return strOut;
	}

	return func(strIn, strOut);
}

void prepCoolBean() {
	if (g_Data.getClientInstance() && g_Data.getClientInstance()->minecraftGame->getServerEntries() && *reinterpret_cast<__int64*>(g_Data.getClientInstance()->minecraftGame->getServerEntries() + 0x50)) {
		auto serverEntries = g_Data.getClientInstance()->minecraftGame->getServerEntries() + 0x48;

		struct ThirdPartyServer {
			TextHolder serverName;
			TextHolder uuid;
			TextHolder masterPlayerAccount;
			TextHolder serverName2;
			TextHolder lobbyDescription;
			TextHolder domain;            // contains the last two parts of the domain .hivebedrock.network .mineplex.com
			TextHolder pathToServerIcon;  // C:\Users\user\AppData\Local\Packages\Microsoft.MinecraftUWP_8wekyb3d8bbwe\LocalCache\minecraftpe\ContentCache\ThirdPartyServer\\<file hash>.jpg
			TextHolder serverAddress;
			int coolBoye;

			ThirdPartyServer() {
				memset(this, 0, sizeof(ThirdPartyServer));
			}
		};

		struct BeansEntry {
			BeansEntry* nextEntry;
			BeansEntry* prevEntry;
			TextHolder masterPlayer;
			float unk;
			char filler[0x3c];
			TextHolder masterPlayer2;
			TextHolder serverName;
			char filler2[8];
			std::shared_ptr<ThirdPartyServer>* start;  // array
			std::shared_ptr<ThirdPartyServer>* end;    // end of array
		};

		auto listEnd = *reinterpret_cast<BeansEntry**>(serverEntries);

		auto current = listEnd;
		int count = 0;
		while (listEnd != current->nextEntry) {
			current = current->nextEntry;
			count++;
		}
		if (count > 5)  // we already added a server
			goto end;

		// make new one
		BeansEntry* epic = new BeansEntry();
		epic->nextEntry = listEnd;
		epic->prevEntry = current;
		epic->masterPlayer.setText("");
		epic->unk = current->unk;
		memcpy(epic->filler, current->filler, sizeof(BeansEntry::filler));
		epic->masterPlayer2.setText("");
		epic->serverName.setText("Epic");
		memcpy(epic->filler2, current->filler2, sizeof(BeansEntry::filler2));

		auto cT = current->start[0].get();

		std::shared_ptr<ThirdPartyServer>* start = new std::shared_ptr<ThirdPartyServer>[1];

		{
			ThirdPartyServer* t = new ThirdPartyServer();

			t->coolBoye = cT->coolBoye;
			t->uuid.setText("");
			t->masterPlayerAccount = cT->masterPlayerAccount;
			t->lobbyDescription = cT->lobbyDescription;
			t->pathToServerIcon.setText("");
			t->serverName.setText("Packet Server");
			t->serverName2.setText("Packet Server");  // This is the one actually displayed
			t->domain.setText(".packetbeta.massivecock");
			t->serverAddress.setText("mc.packetbeta.wtf");
			start[0] = std::shared_ptr<ThirdPartyServer>(t);
		}

		epic->start = start;
		epic->end = &start[1];

		current->nextEntry = epic;

		// increase count
		*reinterpret_cast<__int64*>(g_Data.getClientInstance()->minecraftGame->getServerEntries() + 0x50) += 1;
	end:;
	}
}

__int64 Hooks::prepFeaturedServers(__int64 a1) {
	static auto func = g_Hooks.prepFeaturedServersHook->GetFastcall<__int64, __int64>();
	auto ret = func(a1);

	if (g_Data.getClientInstance() == nullptr)
		return ret;

	if (g_Data.allowWIPFeatures())
		prepCoolBean();

	return ret;
}

__int64 Hooks::prepFeaturedServersFirstTime(__int64 a1, __int64 a2) {
	static auto func = g_Hooks.prepFeaturedServersFirstTimeHook->GetFastcall<__int64, __int64, __int64>();

	if (g_Data.allowWIPFeatures())
		prepCoolBean();

	auto ret = func(a1, a2);

	return ret;
}

__int64 Hooks::GameMode_attack(C_GameMode* _this, C_Entity* ent) {
	auto func = g_Hooks.GameMode_attackHook->GetFastcall<__int64, C_GameMode*, C_Entity*>();
	moduleMgr->onAttack(ent);
	return func(_this, ent);
}

void Hooks::LocalPlayer__updateFromCamera(__int64 a1, C_Camera* camera) {
	auto func = g_Hooks.LocalPlayer__updateFromCameraHook->GetFastcall<__int64, __int64, C_Camera*>();
	static auto noHurtcamMod = moduleMgr->getModule<CameraMod>();
	auto freelookMod = moduleMgr->getModule<Freelook>();
	auto freecamMod = moduleMgr->getModule<Freecam>();

	if (noHurtcamMod->isEnabled() && noHurtcamMod->nohurtMode && g_Data.isInGame() && g_Data.getLocalPlayer()->isAlive()) {
		vec2_t rot;
		camera->getPlayerRotation(&rot);
		if (camera->facesPlayerFront) {
			rot.x *= -1;  // rotate back
			rot.y += 180;
			rot = rot.normAngles();
		}

		camera->setOrientationDeg(rot.x, rot.y, 0);
	}

	if (freelookMod->isEnabled() && freelookMod->redirectMouse) {
		freelookMod->cameraFacesFront = camera->facesPlayerFront;
		freelookMod->isThirdPerson = camera->renderPlayerModel;
		if (freelookMod->resetViewTick >= 0) {
			camera->setOrientationDeg(freelookMod->lastCameraAngle.x, freelookMod->lastCameraAngle.y, 0);
		} else {
			camera->getPlayerRotation(&freelookMod->lastCameraAngle);
		}

		return;
	}

	func(a1, camera);
}

bool Hooks::Mob__isImmobile(C_Entity* ent) {
	auto func = g_Hooks.Mob__isImmobileHook->GetFastcall<bool, C_Entity*>();

	static auto antiImmobileMod = moduleMgr->getModule<AntiImmobile>();
	if (antiImmobileMod->isEnabled() && ent == g_Data.getLocalPlayer())
		return false;

	return func(ent);
}
void Hooks::InventoryTransactionManager__addAction(C_InventoryTransactionManager* _this, C_InventoryAction& action) {
	auto func = g_Hooks.InventoryTransactionManager__addActionHook->GetFastcall<void, C_InventoryTransactionManager*, C_InventoryAction&>();

#ifdef TEST_DEBUG
	char* srcName = "none";
	if (action.sourceItem.item && *action.sourceItem.item)
		srcName = (*action.sourceItem.item)->name.getText();
	char* targetName = "none";
	if (action.targetItem.item && *action.targetItem.item)
		targetName = (*action.targetItem.item)->name.getText();
	logF("%i %i %i %s %s", action.type, action.slot, action.sourceType, srcName, targetName, action.sourceType);

	/*if(/*action.slot == 14 && action.sourceType == 124 && strcmp(targetName, "none") == 0 && *strcmp(srcName, "stone_shovel") == 0){
		std::string tag = "{ench:[{id:9s,lvl:1s}]}";
		action.sourceItem.setUserData(std::move(Mojangson::parseTag(tag)));
	}
	if(/*action.slot == 2 && action.sourceType == 256 && strcmp(srcName, "none") == 0 &&* strcmp(targetName, "stone_shovel") == 0){
		std::string tag = "{ench:[{id:9s,lvl:1s}]}";
		action.targetItem.setUserData(std::move(Mojangson::parseTag(tag)));
	}*/
#endif

	func(_this, action);
}

void Hooks::LevelRendererPlayer__renderNameTags(__int64 a1, __int64 a2, TextHolder* a3, __int64 a4) {
	static auto func = g_Hooks.LevelRendererPlayer__renderNameTagsHook->GetFastcall<void, __int64, __int64, TextHolder*, __int64>();
	static auto nameTagsMod = moduleMgr->getModule<NameTags>();

	if (nameTagsMod->isEnabled() && nameTagsMod->nameTags.size() > 0) {
		std::string text = Utils::sanitize(a3->getText());
		std::size_t found = text.find('\n');

		if (found != std::string::npos)
			text = text.substr(0, found);

		if (nameTagsMod->nameTags.find(text) != nameTagsMod->nameTags.end())
			return;
	}

	return func(a1, a2, a3, a4);
}
