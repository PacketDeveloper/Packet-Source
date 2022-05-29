#include "CCamera.h"

#include "../Utils/Utils.h"

void C_Camera::getEulerAngles(vec3_t* vec) {
	using cam_getEuler_t = void(__fastcall*)(C_Camera*, vec3_t*);
	static auto cam_getEuler = reinterpret_cast<cam_getEuler_t>(FindSignature("48 8B C4 53 48 81 EC ?? ?? ?? ?? 0F 29 70 ?? 48 8B DA 0F 29 78 ?? F3"));
	cam_getEuler(this, vec);
}
void C_Camera::setOrientation(float pitch, float yaw, float roll) {
	using cam_setOrientation_t = void(__fastcall*)(C_Camera*, float, float, float);
	static auto cam_setOrientation = reinterpret_cast<cam_setOrientation_t>(FindSignature("40 53 48 81 EC ?? ?? ?? ?? 0F 29 74 24 ?? 0F 57"));
	cam_setOrientation(this, pitch, yaw, roll);
}
void C_Camera::getPlayerRotation(vec2_t* angOut) {
	vec3_t angles;
	this->getEulerAngles(&angles);

	float v4;
	if ( this->facesPlayerFront )
		v4 = -1.0;
	else
		v4 = 1.0;

	//angOut->x = (float)((float)(angles.x * 57.295776) + *(float *)(reinterpret_cast<__int64>(this) + 268)) * v4;
	//angOut->y = (float)(angles.y * 57.295776) + *(float *)(reinterpret_cast<__int64>(this) + 272);
	angOut->x = (float)((float)(angles.x * 57.295776) + *(float *)(reinterpret_cast<__int64>(this) + 276)) * v4;
	angOut->y = (float)(angles.y * 57.295776) + *(float *)(reinterpret_cast<__int64>(this) + 280);
	angOut->normAngles();/*

	__int64 v2;                     // rdi
	__int64 v3;                     // rbx
	__int64 v4;                     // rax
	float v5;                       // xmm1_4
	__int64 v6;                     // rax
	int v7;                         // eax
	char v8;                        // [rsp+20h] [rbp-78h]
	float v9;                       // [rsp+28h] [rbp-70h]
	float v10;                      // [rsp+2Ch] [rbp-6Ch]
	__int64(__fastcall * *v11)();   // [rsp+38h] [rbp-60h]
	__int64(__fastcall * v12)();    // [rsp+40h] [rbp-58h]
	__int64 v13;                    // [rsp+48h] [rbp-50h]
	__int64 v14;                    // [rsp+50h] [rbp-48h]
	char v15;                       // [rsp+58h] [rbp-40h]
	__int64(__fastcall * **v16)();  // [rsp+70h] [rbp-28h]
	float v17;                      // [rsp+78h] [rbp-20h]
	float v18;                      // [rsp+7Ch] [rbp-1Ch]



	v2 = angOut;
	v3 = this;
	if (*(char *)(a2 + 273) && *(int *)(a1 + 8560) == 16 && !(*(unsigned __int8(__fastcall **)(__int64))(**(__int64 **)(a1 + 8552) + 368i64))(*(__int64 *)(a1 + 8552))) {
		v4 = (*(__int64(__fastcall **)(__int64))(**(__int64 **)(v3 + 888) + 1392i64))(*(__int64 *)(v3 + 888));
		v13 = v2;
		v11 = off_143CF9470;
		v14 = v3;
		v12 = sub_1419E2154;
		v15 = v8;
		v16 = &v11;
		sub_141DB0440(v4, &v11);
		sub_140941F40(v2, &v9);
		if (*(char *)(v2 + 272))
			v5 = -1.0;
		else
			v5 = 1.0;
		v6 = *(__int64 *)v3;
		v18 = (float)(v10 * 57.295776) + *(float *)(v2 + 280);
		v17 = (float)((float)(v9 * 57.295776) + *(float *)(v2 + 276)) * v5;
		(*(void(__fastcall **)(__int64, float *))(v6 + 160))(v3, &v17);
		v7 = *(int *)(v3 + 1832);
		*(float *)(v3 + 1832) = v18;
		*(int *)(v3 + 1836) = v7;
	}*/

}
void C_Camera::getForwardVector(vec3_t* forward) {
	vec3_t eulerAngles;
	this->getEulerAngles(&eulerAngles);

	float calcYaw = eulerAngles.y - (90 * (PI / 180));
	float calcPitch = -eulerAngles.x;

	forward->x = cos(calcYaw) * cos(calcPitch);
	forward->y = sin(calcPitch);
	forward->z = sin(calcYaw) * cos(calcPitch);
}

C_Camera* C_CameraManager::getCameraOrDebugCamera() {
	/*using camerManager_getCameraOrDebugCamera_t = C_Camera*(__fastcall*)(C_CameraManager*);
	static auto camerManager_getCameraOrDebugCamera = reinterpret_cast<camerManager_getCameraOrDebugCamera_t>(FindSignature("40 53 48 83 EC ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 80 39 00 48 8B D9 74 ?? 48"));
	return camerManager_getCameraOrDebugCamera(this);*/
	return reinterpret_cast<C_Camera*>(reinterpret_cast<__int64>(this) + 0x40);
}
