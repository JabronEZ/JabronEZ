#include "forwards.h"
#include "globals.h"

CDetour *g_CSmokeGrenadeEmitGrenadeDetour = NULL;
bool g_CSmokeGrenadeEmitGrenadeDetoured = false;
IForward *g_EmitSmokeGrenadeForward = NULL;

CDetour *g_CFlashbangEmitGrenadeDetour = NULL;
bool g_CFlashbangEmitGrenadeDetoured = false;
IForward *g_EmitFlashGrenadeForward = NULL;

CDetour *g_CMolotovEmitGrenadeDetour = NULL;
bool g_CMolotovEmitGrenadeDetoured = false;
#ifndef WIN32
CDetour *g_CIncendiaryGrenadeEmitGrenadeDetour = NULL;
bool g_CIncendiaryGrenadeEmitGrenadeDetoured = false;
#endif
IForward *g_EmitMolotovGrenadeForward = NULL;
IForward *g_EmitIncendiaryGrenadeForward = NULL;

CDetour *g_CDecoyGrenadeEmitGrenadeDetour = NULL;
bool g_CDecoyGrenadeEmitGrenadeDetoured = false;
IForward *g_EmitDecoyGrenadeForward = NULL;

CDetour *g_CHEGrenadeEmitGrenadeDetour = NULL;
bool g_CHEGrenadeEmitGrenadeDetoured = false;
IForward *g_EmitHEGrenadeForward = NULL;

CDetour *g_CSmokeGrenadeProjectileSmokeDetonateDetour = NULL;
bool g_CSmokeGrenadeProjectileSmokeDetonateDetoured = false;
IForward *g_OnSmokeGrenadeDetonateForward = NULL;

CDetour *g_CMolotovProjectileDetonateDetour = NULL;
bool g_CMolotovProjectileDetonateDetoured = false;
IForward *g_OnMolotovDetonateForward = NULL;
IForward *g_OnIncendiaryDetonateForward = NULL;

CDetour *g_CFlashbangProjectileDetonateDetour = NULL;
bool g_CFlashbangProjectileDetonateDetoured = false;
IForward *g_OnFlashbangDetonateForward = NULL;

CDetour *g_CCSPlayerDeafenDetour = NULL;
bool g_CCSPlayerDeafenDetoured = false;
IForward *g_OnFlashbangDeafenForward = NULL;
bool g_ShouldBlockDeafenHook = false;

CDetour *g_CCSPlayerBlindDetour = NULL;
bool g_CCSPlayerBlindDetoured = false;
IForward *g_OnFlashbangBlindForward = NULL;
bool g_ShouldBlockBlindHook = false;

CDetour *g_CHEGrenadeProjectileDetonateDetour = NULL;
bool g_CHEGrenadeProjectileDetonateDetoured = false;
IForward *g_OnHEGrenadeDetonateForward = NULL;

CDetour *g_CDecoyProjectileGunfireThinkDetour = NULL;
bool g_CDecoyProjectileGunfireThinkDetoured = false;

CDetour *g_CInfernoInfernoThinkDetour = NULL;
bool g_CInfernoInfernoThinkDetoured = false;

CDetour *g_PassServerEntityFilterDetour = NULL;
bool g_PassServerEntityFilterDetoured = false;

IForward *g_PassServerEntityFilterForward = NULL;
IForward *g_ShouldCollideForward = NULL;

void CallEmitSmokeGrenadeForward(int p_Client, Vector p_Origin, QAngle p_Angle, Vector p_Velocity, Vector p_AngularImpulse, bool &p_BlockEmit)
{
	p_BlockEmit = false;

	if (!g_EmitSmokeGrenadeForward)
		return;

	cell_t s_OriginArray[3];
	s_OriginArray[0] = sp_ftoc(p_Origin.x);
	s_OriginArray[1] = sp_ftoc(p_Origin.y);
	s_OriginArray[2] = sp_ftoc(p_Origin.z);

	cell_t s_AngleArray[3];
	s_AngleArray[0] = sp_ftoc(p_Angle.x);
	s_AngleArray[1] = sp_ftoc(p_Angle.y);
	s_AngleArray[2] = sp_ftoc(p_Angle.z);

	cell_t s_VelocityArray[3];
	s_VelocityArray[0] = sp_ftoc(p_Velocity.x);
	s_VelocityArray[1] = sp_ftoc(p_Velocity.y);
	s_VelocityArray[2] = sp_ftoc(p_Velocity.z);

	cell_t s_AngularImpulseArray[3];
	s_AngularImpulseArray[0] = sp_ftoc(p_AngularImpulse.x);
	s_AngularImpulseArray[1] = sp_ftoc(p_AngularImpulse.y);
	s_AngularImpulseArray[2] = sp_ftoc(p_AngularImpulse.z);

	cell_t s_Result = 0;
	cell_t s_BlockParam = 0;
	g_EmitSmokeGrenadeForward->PushCell(p_Client);
	g_EmitSmokeGrenadeForward->PushArray(s_OriginArray, 3);
	g_EmitSmokeGrenadeForward->PushArray(s_AngleArray, 3);
	g_EmitSmokeGrenadeForward->PushArray(s_VelocityArray, 3);
	g_EmitSmokeGrenadeForward->PushArray(s_AngularImpulseArray, 3);
	g_EmitSmokeGrenadeForward->PushCellByRef(&s_BlockParam);
	g_EmitSmokeGrenadeForward->Execute(&s_Result);

	p_BlockEmit = s_BlockParam != 0;
}

void CallEmitFlashGrenadeForward(int p_Client, Vector p_Origin, QAngle p_Angle, Vector p_Velocity, Vector p_AngularImpulse, bool &p_BlockEmit)
{
	p_BlockEmit = false;

	if (!g_EmitFlashGrenadeForward)
		return;

	cell_t s_OriginArray[3];
	s_OriginArray[0] = sp_ftoc(p_Origin.x);
	s_OriginArray[1] = sp_ftoc(p_Origin.y);
	s_OriginArray[2] = sp_ftoc(p_Origin.z);

	cell_t s_AngleArray[3];
	s_AngleArray[0] = sp_ftoc(p_Angle.x);
	s_AngleArray[1] = sp_ftoc(p_Angle.y);
	s_AngleArray[2] = sp_ftoc(p_Angle.z);

	cell_t s_VelocityArray[3];
	s_VelocityArray[0] = sp_ftoc(p_Velocity.x);
	s_VelocityArray[1] = sp_ftoc(p_Velocity.y);
	s_VelocityArray[2] = sp_ftoc(p_Velocity.z);

	cell_t s_AngularImpulseArray[3];
	s_AngularImpulseArray[0] = sp_ftoc(p_AngularImpulse.x);
	s_AngularImpulseArray[1] = sp_ftoc(p_AngularImpulse.y);
	s_AngularImpulseArray[2] = sp_ftoc(p_AngularImpulse.z);

	cell_t s_Result = 0;
	cell_t s_BlockParam = 0;
	g_EmitFlashGrenadeForward->PushCell(p_Client);
	g_EmitFlashGrenadeForward->PushArray(s_OriginArray, 3);
	g_EmitFlashGrenadeForward->PushArray(s_AngleArray, 3);
	g_EmitFlashGrenadeForward->PushArray(s_VelocityArray, 3);
	g_EmitFlashGrenadeForward->PushArray(s_AngularImpulseArray, 3);
	g_EmitFlashGrenadeForward->PushCellByRef(&s_BlockParam);
	g_EmitFlashGrenadeForward->Execute(&s_Result);

	p_BlockEmit = s_BlockParam != 0;
}

void CallEmitMolotovGrenadeForward(int p_Client, Vector p_Origin, QAngle p_Angle, Vector p_Velocity, Vector p_AngularImpulse, bool &p_BlockEmit)
{
	p_BlockEmit = false;

	if (!g_EmitMolotovGrenadeForward)
		return;

	cell_t s_OriginArray[3];
	s_OriginArray[0] = sp_ftoc(p_Origin.x);
	s_OriginArray[1] = sp_ftoc(p_Origin.y);
	s_OriginArray[2] = sp_ftoc(p_Origin.z);

	cell_t s_AngleArray[3];
	s_AngleArray[0] = sp_ftoc(p_Angle.x);
	s_AngleArray[1] = sp_ftoc(p_Angle.y);
	s_AngleArray[2] = sp_ftoc(p_Angle.z);

	cell_t s_VelocityArray[3];
	s_VelocityArray[0] = sp_ftoc(p_Velocity.x);
	s_VelocityArray[1] = sp_ftoc(p_Velocity.y);
	s_VelocityArray[2] = sp_ftoc(p_Velocity.z);

	cell_t s_AngularImpulseArray[3];
	s_AngularImpulseArray[0] = sp_ftoc(p_AngularImpulse.x);
	s_AngularImpulseArray[1] = sp_ftoc(p_AngularImpulse.y);
	s_AngularImpulseArray[2] = sp_ftoc(p_AngularImpulse.z);

	cell_t s_Result = 0;
	cell_t s_BlockParam = 0;
	g_EmitMolotovGrenadeForward->PushCell(p_Client);
	g_EmitMolotovGrenadeForward->PushArray(s_OriginArray, 3);
	g_EmitMolotovGrenadeForward->PushArray(s_AngleArray, 3);
	g_EmitMolotovGrenadeForward->PushArray(s_VelocityArray, 3);
	g_EmitMolotovGrenadeForward->PushArray(s_AngularImpulseArray, 3);
	g_EmitMolotovGrenadeForward->PushCellByRef(&s_BlockParam);
	g_EmitMolotovGrenadeForward->Execute(&s_Result);

	p_BlockEmit = s_BlockParam != 0;
}

void CallEmitIncendiaryGrenadeForward(int p_Client, Vector p_Origin, QAngle p_Angle, Vector p_Velocity, Vector p_AngularImpulse, bool &p_BlockEmit)
{
	p_BlockEmit = false;

	if (!g_EmitIncendiaryGrenadeForward)
		return;

	cell_t s_OriginArray[3];
	s_OriginArray[0] = sp_ftoc(p_Origin.x);
	s_OriginArray[1] = sp_ftoc(p_Origin.y);
	s_OriginArray[2] = sp_ftoc(p_Origin.z);

	cell_t s_AngleArray[3];
	s_AngleArray[0] = sp_ftoc(p_Angle.x);
	s_AngleArray[1] = sp_ftoc(p_Angle.y);
	s_AngleArray[2] = sp_ftoc(p_Angle.z);

	cell_t s_VelocityArray[3];
	s_VelocityArray[0] = sp_ftoc(p_Velocity.x);
	s_VelocityArray[1] = sp_ftoc(p_Velocity.y);
	s_VelocityArray[2] = sp_ftoc(p_Velocity.z);

	cell_t s_AngularImpulseArray[3];
	s_AngularImpulseArray[0] = sp_ftoc(p_AngularImpulse.x);
	s_AngularImpulseArray[1] = sp_ftoc(p_AngularImpulse.y);
	s_AngularImpulseArray[2] = sp_ftoc(p_AngularImpulse.z);

	cell_t s_Result = 0;
	cell_t s_BlockParam = 0;
	g_EmitIncendiaryGrenadeForward->PushCell(p_Client);
	g_EmitIncendiaryGrenadeForward->PushArray(s_OriginArray, 3);
	g_EmitIncendiaryGrenadeForward->PushArray(s_AngleArray, 3);
	g_EmitIncendiaryGrenadeForward->PushArray(s_VelocityArray, 3);
	g_EmitIncendiaryGrenadeForward->PushArray(s_AngularImpulseArray, 3);
	g_EmitIncendiaryGrenadeForward->PushCellByRef(&s_BlockParam);
	g_EmitIncendiaryGrenadeForward->Execute(&s_Result);

	p_BlockEmit = s_BlockParam != 0;
}

void CallEmitDecoyGrenadeForward(int p_Client, Vector p_Origin, QAngle p_Angle, Vector p_Velocity, Vector p_AngularImpulse, bool &p_BlockEmit)
{
	p_BlockEmit = false;

	if (!g_EmitDecoyGrenadeForward)
		return;

	cell_t s_OriginArray[3];
	s_OriginArray[0] = sp_ftoc(p_Origin.x);
	s_OriginArray[1] = sp_ftoc(p_Origin.y);
	s_OriginArray[2] = sp_ftoc(p_Origin.z);

	cell_t s_AngleArray[3];
	s_AngleArray[0] = sp_ftoc(p_Angle.x);
	s_AngleArray[1] = sp_ftoc(p_Angle.y);
	s_AngleArray[2] = sp_ftoc(p_Angle.z);

	cell_t s_VelocityArray[3];
	s_VelocityArray[0] = sp_ftoc(p_Velocity.x);
	s_VelocityArray[1] = sp_ftoc(p_Velocity.y);
	s_VelocityArray[2] = sp_ftoc(p_Velocity.z);

	cell_t s_AngularImpulseArray[3];
	s_AngularImpulseArray[0] = sp_ftoc(p_AngularImpulse.x);
	s_AngularImpulseArray[1] = sp_ftoc(p_AngularImpulse.y);
	s_AngularImpulseArray[2] = sp_ftoc(p_AngularImpulse.z);

	cell_t s_Result = 0;
	cell_t s_BlockParam = 0;
	g_EmitDecoyGrenadeForward->PushCell(p_Client);
	g_EmitDecoyGrenadeForward->PushArray(s_OriginArray, 3);
	g_EmitDecoyGrenadeForward->PushArray(s_AngleArray, 3);
	g_EmitDecoyGrenadeForward->PushArray(s_VelocityArray, 3);
	g_EmitDecoyGrenadeForward->PushArray(s_AngularImpulseArray, 3);
	g_EmitDecoyGrenadeForward->PushCellByRef(&s_BlockParam);
	g_EmitDecoyGrenadeForward->Execute(&s_Result);

	p_BlockEmit = s_BlockParam != 0;
}

void CallEmitHEGrenadeForward(int p_Client, Vector p_Origin, QAngle p_Angle, Vector p_Velocity, Vector p_AngularImpulse, bool &p_BlockEmit)
{
	p_BlockEmit = false;

	if (!g_EmitHEGrenadeForward)
		return;

	cell_t s_OriginArray[3];
	s_OriginArray[0] = sp_ftoc(p_Origin.x);
	s_OriginArray[1] = sp_ftoc(p_Origin.y);
	s_OriginArray[2] = sp_ftoc(p_Origin.z);

	cell_t s_AngleArray[3];
	s_AngleArray[0] = sp_ftoc(p_Angle.x);
	s_AngleArray[1] = sp_ftoc(p_Angle.y);
	s_AngleArray[2] = sp_ftoc(p_Angle.z);

	cell_t s_VelocityArray[3];
	s_VelocityArray[0] = sp_ftoc(p_Velocity.x);
	s_VelocityArray[1] = sp_ftoc(p_Velocity.y);
	s_VelocityArray[2] = sp_ftoc(p_Velocity.z);

	cell_t s_AngularImpulseArray[3];
	s_AngularImpulseArray[0] = sp_ftoc(p_AngularImpulse.x);
	s_AngularImpulseArray[1] = sp_ftoc(p_AngularImpulse.y);
	s_AngularImpulseArray[2] = sp_ftoc(p_AngularImpulse.z);

	cell_t s_Result = 0;
	cell_t s_BlockParam = 0;
	g_EmitHEGrenadeForward->PushCell(p_Client);
	g_EmitHEGrenadeForward->PushArray(s_OriginArray, 3);
	g_EmitHEGrenadeForward->PushArray(s_AngleArray, 3);
	g_EmitHEGrenadeForward->PushArray(s_VelocityArray, 3);
	g_EmitHEGrenadeForward->PushArray(s_AngularImpulseArray, 3);
	g_EmitHEGrenadeForward->PushCellByRef(&s_BlockParam);
	g_EmitHEGrenadeForward->Execute(&s_Result);

	p_BlockEmit = s_BlockParam != 0;
}

void CallOnSmokeDetonateForward(int p_SmokeIndex, bool &p_BlockDetonate)
{
	p_BlockDetonate = false;

	if (!g_OnSmokeGrenadeDetonateForward)
		return;

	cell_t s_Result = 0;
	cell_t s_BlockParam = 0;
	g_OnSmokeGrenadeDetonateForward->PushCell(p_SmokeIndex);
	g_OnSmokeGrenadeDetonateForward->PushCellByRef(&s_BlockParam);
	g_OnSmokeGrenadeDetonateForward->Execute(&s_Result);

	p_BlockDetonate = s_BlockParam != 0;
}

void CallOnMolotovDetonateForward(int p_MolotovIndex, bool &p_BlockDetonate)
{
	p_BlockDetonate = false;

	if (!g_OnMolotovDetonateForward)
		return;

	cell_t s_Result = 0;
	cell_t s_BlockParam = 0;
	g_OnMolotovDetonateForward->PushCell(p_MolotovIndex);
	g_OnMolotovDetonateForward->PushCellByRef(&s_BlockParam);
	g_OnMolotovDetonateForward->Execute(&s_Result);

	p_BlockDetonate = s_BlockParam != 0;
}

void CallOnIncendiaryDetonateForward(int p_IncendiaryIndex, bool &p_BlockDetonate)
{
	p_BlockDetonate = false;

	if (!g_OnIncendiaryDetonateForward)
		return;

	cell_t s_Result = 0;
	cell_t s_BlockParam = 0;
	g_OnIncendiaryDetonateForward->PushCell(p_IncendiaryIndex);
	g_OnIncendiaryDetonateForward->PushCellByRef(&s_BlockParam);
	g_OnIncendiaryDetonateForward->Execute(&s_Result);

	p_BlockDetonate = s_BlockParam != 0;
}

void CallOnFlashbangDetonateForward(int p_FlashbangIndex, bool &p_BlockDetonate)
{
	p_BlockDetonate = false;

	if (!g_OnFlashbangDetonateForward)
		return;

	cell_t s_Result = 0;
	cell_t s_BlockParam = 0;
	g_OnFlashbangDetonateForward->PushCell(p_FlashbangIndex);
	g_OnFlashbangDetonateForward->PushCellByRef(&s_BlockParam);
	g_OnFlashbangDetonateForward->Execute(&s_Result);

	p_BlockDetonate = s_BlockParam != 0;
}

void CallOnHEGrenadeDetonateForward(int p_HEGrenadeIndex, bool &p_BlockDetonate)
{
	p_BlockDetonate = false;

	if (!g_OnHEGrenadeDetonateForward)
		return;

	cell_t s_Result = 0;
	cell_t s_BlockParam = 0;
	g_OnHEGrenadeDetonateForward->PushCell(p_HEGrenadeIndex);
	g_OnHEGrenadeDetonateForward->PushCellByRef(&s_BlockParam);
	g_OnHEGrenadeDetonateForward->Execute(&s_Result);

	p_BlockDetonate = s_BlockParam != 0;
}

void CallOnFlashbangDeafenForward(int p_ClientIndex, int p_FlashbangIndex, float p_Distance, bool &p_BlockDeafen)
{
	p_BlockDeafen = false;

	if (!g_OnFlashbangDeafenForward)
		return;

	cell_t s_Result = 0;
	cell_t s_BlockParam = 0;
	g_OnFlashbangDeafenForward->PushCell(p_ClientIndex);
	g_OnFlashbangDeafenForward->PushCell(p_FlashbangIndex);
	g_OnFlashbangDeafenForward->PushCell(sp_ftoc(p_Distance));
	g_OnFlashbangDeafenForward->PushCellByRef(&s_BlockParam);
	g_OnFlashbangDeafenForward->Execute(&s_Result);

	p_BlockDeafen = s_BlockParam != 0;
}

void CallOnFlashbangBlindForward(int p_ClientIndex, int p_FlashbangIndex, float p_HoldTime, float p_FadeTime, float p_StartingAlpha, bool &p_BlockBlind)
{
	p_BlockBlind = false;

	if (!g_OnFlashbangBlindForward)
		return;

	cell_t s_Result = 0;
	cell_t s_BlockParam = 0;
	g_OnFlashbangBlindForward->PushCell(p_ClientIndex);
	g_OnFlashbangBlindForward->PushCell(p_FlashbangIndex);
	g_OnFlashbangBlindForward->PushCell(sp_ftoc(p_HoldTime));
	g_OnFlashbangBlindForward->PushCell(sp_ftoc(p_FadeTime));
	g_OnFlashbangBlindForward->PushCell(sp_ftoc(p_StartingAlpha));
	g_OnFlashbangBlindForward->PushCellByRef(&s_BlockParam);
	g_OnFlashbangBlindForward->Execute(&s_Result);

	p_BlockBlind = s_BlockParam != 0;
}

DETOUR_DECL_MEMBER6(DetourCSmokeGrenadeEmitGrenade, CBaseEntity*, Vector, p_Origin, QAngle, p_Angle, Vector, p_Velocity, Vector, p_AngularImpulse, CBasePlayer *, p_Player, const CCSWeaponInfo &, p_WeaponInfo)
{
	cell_t s_Reference = gamehelpers->EntityToReference((CBaseEntity*)p_Player);
	int s_Client = gamehelpers->ReferenceToIndex(s_Reference);

	bool s_BlockEmit = false;
	CallEmitSmokeGrenadeForward(s_Client, p_Origin, p_Angle, p_Velocity, p_AngularImpulse, s_BlockEmit);

	if (!s_BlockEmit)
		return DETOUR_MEMBER_CALL(DetourCSmokeGrenadeEmitGrenade)(p_Origin, p_Angle, p_Velocity, p_AngularImpulse, p_Player, p_WeaponInfo);

	return NULL;
}

DETOUR_DECL_MEMBER6(DetourCFlashbangEmitGrenade, CBaseEntity*, Vector, p_Origin, QAngle, p_Angle, Vector, p_Velocity, Vector, p_AngularImpulse, CBasePlayer *, p_Player, const CCSWeaponInfo &, p_WeaponInfo)
{
	META_CONPRINTF("Emitting flash...\n");

	cell_t s_Reference = gamehelpers->EntityToReference((CBaseEntity*)p_Player);
	int s_Client = gamehelpers->ReferenceToIndex(s_Reference);

	bool s_BlockEmit = false;
	CallEmitFlashGrenadeForward(s_Client, p_Origin, p_Angle, p_Velocity, p_AngularImpulse, s_BlockEmit);

	if (!s_BlockEmit)
		return DETOUR_MEMBER_CALL(DetourCFlashbangEmitGrenade)(p_Origin, p_Angle, p_Velocity, p_AngularImpulse, p_Player, p_WeaponInfo);

	return NULL;
}

DETOUR_DECL_MEMBER6(DetourCMolotovEmitGrenade, CBaseEntity*, Vector, p_Origin, QAngle, p_Angle, Vector, p_Velocity, Vector, p_AngularImpulse, CBasePlayer *, p_Player, const CCSWeaponInfo &, p_WeaponInfo)
{
#ifdef WIN32
	CCSWeaponInfo *s_WeaponInfoPointer = (CCSWeaponInfo*)&p_WeaponInfo;
	int s_WeaponType = *(int *)((intptr_t)s_WeaponInfoPointer + 0x750);
#endif

	cell_t s_Reference = gamehelpers->EntityToReference((CBaseEntity*)p_Player);
	int s_Client = gamehelpers->ReferenceToIndex(s_Reference);

	bool s_BlockEmit = false;

#ifdef WIN32
	if (s_WeaponType == 48)
		CallEmitIncendiaryGrenadeForward(s_Client, p_Origin, p_Angle, p_Velocity, p_AngularImpulse, s_BlockEmit);
	else
		CallEmitMolotovGrenadeForward(s_Client, p_Origin, p_Angle, p_Velocity, p_AngularImpulse, s_BlockEmit);
#else
	CallEmitMolotovGrenadeForward(s_Client, p_Origin, p_Angle, p_Velocity, p_AngularImpulse, s_BlockEmit);
#endif

	if (!s_BlockEmit)
		return DETOUR_MEMBER_CALL(DetourCMolotovEmitGrenade)(p_Origin, p_Angle, p_Velocity, p_AngularImpulse, p_Player, p_WeaponInfo);

	return NULL;
}

#ifndef WIN32
DETOUR_DECL_MEMBER6(DetourCIncendiaryGrenadeEmitGrenade, CBaseEntity*, Vector, p_Origin, QAngle, p_Angle, Vector, p_Velocity, Vector, p_AngularImpulse, CBasePlayer *, p_Player, const CCSWeaponInfo &, p_WeaponInfo)
{
	CCSWeaponInfo *s_WeaponInfoPointer = (CCSWeaponInfo*)&p_WeaponInfo;

	cell_t s_Reference = gamehelpers->EntityToReference((CBaseEntity*)p_Player);
	int s_Client = gamehelpers->ReferenceToIndex(s_Reference);

	bool s_BlockEmit = false;

	CallEmitIncendiaryGrenadeForward(s_Client, p_Origin, p_Angle, p_Velocity, p_AngularImpulse, s_BlockEmit);

	if (!s_BlockEmit)
		return DETOUR_MEMBER_CALL(DetourCIncendiaryGrenadeEmitGrenade)(p_Origin, p_Angle, p_Velocity, p_AngularImpulse, p_Player, p_WeaponInfo);

	return NULL;
}
#endif

DETOUR_DECL_MEMBER6(DetourCDecoyGrenadeEmitGrenade, CBaseEntity*, Vector, p_Origin, QAngle, p_Angle, Vector, p_Velocity, Vector, p_AngularImpulse, CBasePlayer *, p_Player, const CCSWeaponInfo &, p_WeaponInfo)
{
	cell_t s_Reference = gamehelpers->EntityToReference((CBaseEntity*)p_Player);
	int s_Client = gamehelpers->ReferenceToIndex(s_Reference);

	bool s_BlockEmit = false;
	CallEmitDecoyGrenadeForward(s_Client, p_Origin, p_Angle, p_Velocity, p_AngularImpulse, s_BlockEmit);

	if (!s_BlockEmit)
		return DETOUR_MEMBER_CALL(DetourCDecoyGrenadeEmitGrenade)(p_Origin, p_Angle, p_Velocity, p_AngularImpulse, p_Player, p_WeaponInfo);

	return NULL;
}

DETOUR_DECL_MEMBER6(DetourCHEGrenadeEmitGrenade, CBaseEntity*, Vector, p_Origin, QAngle, p_Angle, Vector, p_Velocity, Vector, p_AngularImpulse, CBasePlayer *, p_Player, const CCSWeaponInfo &, p_WeaponInfo)
{
	cell_t s_Reference = gamehelpers->EntityToReference((CBaseEntity*)p_Player);
	int s_Client = gamehelpers->ReferenceToIndex(s_Reference);

	bool s_BlockEmit = false;
	CallEmitHEGrenadeForward(s_Client, p_Origin, p_Angle, p_Velocity, p_AngularImpulse, s_BlockEmit);

	if (!s_BlockEmit)
		return DETOUR_MEMBER_CALL(DetourCHEGrenadeEmitGrenade)(p_Origin, p_Angle, p_Velocity, p_AngularImpulse, p_Player, p_WeaponInfo);

	return NULL;
}

DETOUR_DECL_MEMBER0(DetourCSmokeGrenadeProjectileSmokeDetonate, void)
{
	CBaseEntity *s_Instance = (CBaseEntity*)this;

	cell_t s_Reference = gamehelpers->EntityToReference(s_Instance);
	int s_SmokeIndex = gamehelpers->ReferenceToIndex(s_Reference);
	
	META_CONPRINTF("Entity index is %d classname is %s\n", s_SmokeIndex, gamehelpers->GetEntityClassname(s_Instance));

	bool s_BlockDetonate = false;
	CallOnSmokeDetonateForward(s_SmokeIndex, s_BlockDetonate);
	
	if (!s_BlockDetonate)
	{
		cell_t s_OldSmokeGrenade = g_CurrentlyDetonatingSmokeGrenade;

		g_CurrentlyDetonatingSmokeGrenade = s_Reference;
		DETOUR_MEMBER_CALL(DetourCSmokeGrenadeProjectileSmokeDetonate)();
		g_CurrentlyDetonatingSmokeGrenade = s_OldSmokeGrenade;
	}
}

DETOUR_DECL_MEMBER0(DetourCMolotovProjectileDetonate, void)
{
	CBaseEntity *s_Instance = (CBaseEntity*)this;

	cell_t s_Reference = gamehelpers->EntityToReference(s_Instance);
	int s_GrenadeIndex = gamehelpers->ReferenceToIndex(s_Reference);

	bool s_BlockDetonate = false;
	
	// HACK: Determine if this is a molotov or Incendiary
	datamap_t *s_Datamap = gamehelpers->GetDataMap(s_Instance);
	sm_datatable_info_t s_DataInfo;
	gamehelpers->FindDataMapInfo(s_Datamap, "m_ModelName", &s_DataInfo);
	
	string_t s_String;
	s_String = *(string_t *)((uint8_t *)s_Instance + s_DataInfo.actual_offset);
	const char *s_ModelName = (s_String == NULL_STRING) ? "" : STRING(s_String);
	
	bool s_IsMolotov = strcmp(s_ModelName, "models/Weapons/w_eq_molotov_thrown.mdl") == 0;

	if (s_IsMolotov)
		CallOnMolotovDetonateForward(s_GrenadeIndex, s_BlockDetonate);
	else
		CallOnIncendiaryDetonateForward(s_GrenadeIndex, s_BlockDetonate);

	if (!s_BlockDetonate)
	{
		if (s_IsMolotov)
		{
			cell_t s_OldMolotov = g_CurrentlyDetonatingMolotov;

			g_CurrentlyDetonatingMolotov = s_Reference;
			DETOUR_MEMBER_CALL(DetourCMolotovProjectileDetonate)();
			g_CurrentlyDetonatingMolotov = s_OldMolotov;
		}
		else
		{
			cell_t s_OldIncendiary = g_CurrentlyDetonatingIncendiary;

			g_CurrentlyDetonatingIncendiary = s_Reference;
			DETOUR_MEMBER_CALL(DetourCMolotovProjectileDetonate)();
			g_CurrentlyDetonatingIncendiary = s_OldIncendiary;
		}
	}
}

DETOUR_DECL_MEMBER0(DetourCFlashbangProjectileDetonate, void)
{
	CBaseEntity *s_Instance = (CBaseEntity*)this;

	cell_t s_Reference = gamehelpers->EntityToReference(s_Instance);
	int s_FlashbangIndex = gamehelpers->ReferenceToIndex(s_Reference);

	bool s_BlockDetonate = false;

	CallOnFlashbangDetonateForward(s_FlashbangIndex, s_BlockDetonate);

	if (!s_BlockDetonate)
	{
		cell_t s_OldFlashbang = g_CurrentlyDetonatingFlashbang;

		g_CurrentlyDetonatingFlashbang = s_Reference;
		DETOUR_MEMBER_CALL(DetourCFlashbangProjectileDetonate)();
		g_CurrentlyDetonatingFlashbang = s_OldFlashbang;
	}
}

#ifdef WIN32
DETOUR_DECL_MEMBER0(DetourCCSPlayerDeafen, void)
#else
DETOUR_DECL_MEMBER1(DetourCCSPlayerDeafen, void, float, p_Distance)
#endif
{
#ifdef WIN32
	float p_Distance;

	//Save the distance
	__asm
	{
		movss p_Distance, xmm1
	}
#endif

	if (g_ShouldBlockDeafenHook)
	{
#ifdef WIN32
		__asm
		{
			movss xmm1, p_Distance
		}

		DETOUR_MEMBER_CALL(DetourCCSPlayerDeafen)();
#else
		DETOUR_MEMBER_CALL(DetourCCSPlayerDeafen)(p_Distance);
#endif
		return;
	}

	CBaseEntity *s_Instance = (CBaseEntity*)this;

	cell_t s_Reference = gamehelpers->EntityToReference(s_Instance);
	int s_ClientIndex = gamehelpers->ReferenceToIndex(s_Reference);

	if (g_CurrentlyDetonatingFlashbang == INVALID_ENT_REFERENCE)
	{
#ifdef WIN32
		__asm
		{
			movss xmm1, p_Distance
		}

		DETOUR_MEMBER_CALL(DetourCCSPlayerDeafen)();
#else
		DETOUR_MEMBER_CALL(DetourCCSPlayerDeafen)(p_Distance);
#endif

		return;
	}

	int s_FlashbangIndex = gamehelpers->ReferenceToIndex(g_CurrentlyDetonatingFlashbang);

	if (s_FlashbangIndex == INVALID_ENT_REFERENCE)
	{
#ifdef WIN32
		__asm
		{
			movss xmm1, p_Distance
		}

		DETOUR_MEMBER_CALL(DetourCCSPlayerDeafen)();
#else
		DETOUR_MEMBER_CALL(DetourCCSPlayerDeafen)(p_Distance);
#endif

		return;
	}

	
	bool s_BlockDeafen = false;

	g_ShouldBlockDeafenHook = true;
	CallOnFlashbangDeafenForward(s_ClientIndex, s_FlashbangIndex, p_Distance, s_BlockDeafen);
	g_ShouldBlockDeafenHook = false;

	if (!s_BlockDeafen)
	{
#ifdef WIN32
		__asm
		{
			movss xmm1, p_Distance
		}

		DETOUR_MEMBER_CALL(DetourCCSPlayerDeafen)();
#else
		DETOUR_MEMBER_CALL(DetourCCSPlayerDeafen)(p_Distance);
#endif
	}
}

DETOUR_DECL_MEMBER3(DetourCCSPlayerBlind, void, float, p_HoldTime, float, p_FadeTime, float, p_StartingAlpha)
{
	if (g_ShouldBlockBlindHook)
	{
		DETOUR_MEMBER_CALL(DetourCCSPlayerBlind)(p_HoldTime, p_FadeTime, p_StartingAlpha);
		return;
	}

	CBaseEntity *s_Instance = (CBaseEntity*)this;

	cell_t s_Reference = gamehelpers->EntityToReference(s_Instance);
	int s_ClientIndex = gamehelpers->ReferenceToIndex(s_Reference);

	if (g_CurrentlyDetonatingFlashbang == INVALID_ENT_REFERENCE)
	{
		DETOUR_MEMBER_CALL(DetourCCSPlayerBlind)(p_HoldTime, p_FadeTime, p_StartingAlpha);
		return;
	}

	int s_FlashbangIndex = gamehelpers->ReferenceToIndex(g_CurrentlyDetonatingFlashbang);

	if (s_FlashbangIndex == INVALID_ENT_REFERENCE)
	{
		DETOUR_MEMBER_CALL(DetourCCSPlayerBlind)(p_HoldTime, p_FadeTime, p_StartingAlpha);
		return;
	}

	bool s_BlockBlind = false;

	g_ShouldBlockBlindHook = true;
	CallOnFlashbangBlindForward(s_ClientIndex, s_FlashbangIndex, p_HoldTime, p_FadeTime, p_StartingAlpha, s_BlockBlind);
	g_ShouldBlockBlindHook = false;

	if (!s_BlockBlind)
		DETOUR_MEMBER_CALL(DetourCCSPlayerBlind)(p_HoldTime, p_FadeTime, p_StartingAlpha);
}

DETOUR_DECL_MEMBER0(DetourCHEGrenadeProjectileDetonate, void)
{
	CBaseEntity *s_Instance = (CBaseEntity*)this;

	cell_t s_Reference = gamehelpers->EntityToReference(s_Instance);
	int s_HEGrenadeIndex = gamehelpers->ReferenceToIndex(s_Reference);

	bool s_BlockDetonate = false;

	CallOnHEGrenadeDetonateForward(s_HEGrenadeIndex, s_BlockDetonate);

	if (!s_BlockDetonate)
	{
		cell_t s_OldHEGrenade = g_CurrentlyDetonatingHEGrenade;

		g_CurrentlyDetonatingHEGrenade = s_Reference;
		DETOUR_MEMBER_CALL(DetourCHEGrenadeProjectileDetonate)();
		g_CurrentlyDetonatingHEGrenade = s_OldHEGrenade;
	}
}

DETOUR_DECL_MEMBER0(DetourCDecoyProjectileGunfireThink, void)
{
	CBaseEntity *s_Instance = (CBaseEntity*)this;
	cell_t s_Reference = gamehelpers->EntityToReference(s_Instance);

	cell_t s_OldDecoy = g_CurrentlyGunfireThinkingDecoy;

	g_CurrentlyGunfireThinkingDecoy = s_Reference;
	DETOUR_MEMBER_CALL(DetourCDecoyProjectileGunfireThink)();
	g_CurrentlyGunfireThinkingDecoy = s_OldDecoy;
}

DETOUR_DECL_MEMBER0(DetourCInfernoInfernoThink, void)
{
	CBaseEntity *s_Instance = (CBaseEntity*)this;
	cell_t s_Reference = gamehelpers->EntityToReference(s_Instance);

	cell_t s_OldInferno = g_CurrentlyThinkingInferno;

	g_CurrentlyThinkingInferno = s_Reference;
	DETOUR_MEMBER_CALL(DetourCInfernoInfernoThink)();
	g_CurrentlyThinkingInferno = s_OldInferno;
}

// adapted from util_shared.h
inline const CBaseEntity *UTIL_EntityFromEntityHandle(const IHandleEntity *pConstHandleEntity)
{
	IHandleEntity *pHandleEntity = const_cast<IHandleEntity *>(pConstHandleEntity);
	IServerUnknown *pUnk = static_cast<IServerUnknown *>(pHandleEntity);

	return pUnk->GetBaseEntity();
}

#ifdef _WIN32
bool (__fastcall *DetourPassServerEntityFilter_Actual)(const IHandleEntity *, const IHandleEntity *) = NULL;
bool __fastcall DetourPassServerEntityFilter(const IHandleEntity *p_Touch, const IHandleEntity *p_Pass)
#else
bool (__cdecl *DetourPassServerEntityFilter_Actual)(const IHandleEntity *, const IHandleEntity *) = NULL;
bool __cdecl DetourPassServerEntityFilter(const IHandleEntity *p_Touch, const IHandleEntity *p_Pass)
#endif
{
	if(g_PassServerEntityFilterForward->GetFunctionCount() == 0)
		return DetourPassServerEntityFilter_Actual(p_Touch, p_Pass);

	if(p_Touch == p_Pass)
		return DetourPassServerEntityFilter_Actual(p_Touch, p_Pass);

	if(!p_Touch || !p_Pass)
		return DetourPassServerEntityFilter_Actual(p_Touch, p_Pass);

	CBaseEntity *s_Ent1 = const_cast<CBaseEntity *>(UTIL_EntityFromEntityHandle(p_Touch));
	CBaseEntity *s_Ent2 = const_cast<CBaseEntity *>(UTIL_EntityFromEntityHandle(p_Pass));

	if(!s_Ent1 || !s_Ent2)
		return DetourPassServerEntityFilter_Actual(p_Touch, p_Pass);

	cell_t s_EntRef1 = gamehelpers->EntityToBCompatRef(s_Ent1);
	cell_t s_EntRef2 = gamehelpers->EntityToBCompatRef(s_Ent2);

	cell_t s_Result = 0;
	g_PassServerEntityFilterForward->PushCell(s_EntRef1);
	g_PassServerEntityFilterForward->PushCell(s_EntRef2);
	g_PassServerEntityFilterForward->PushCellByRef(&s_Result);

	cell_t s_RetValue = 0;
	g_PassServerEntityFilterForward->Execute(&s_RetValue);

	if(s_RetValue > Pl_Continue)
		return s_Result == 1;

	// otherwise, game decides
	return DetourPassServerEntityFilter_Actual(p_Touch, p_Pass);
}

bool CreateCSmokeGrenadeEmitGrenadeDetour(char *error, int maxlength)
{
	g_CSmokeGrenadeEmitGrenadeDetour = DETOUR_CREATE_MEMBER(DetourCSmokeGrenadeEmitGrenade, "CSmokeGrenadeEmitGrenade");

	if (g_CSmokeGrenadeEmitGrenadeDetour != NULL)
	{
		g_CSmokeGrenadeEmitGrenadeDetour->EnableDetour();
		g_CSmokeGrenadeEmitGrenadeDetoured = true;
		return true;
	}

	if (error)
		snprintf(error, maxlength, "Unable to find address of CSmokeGrenadeEmitGrenade for detour");

	return false;
}

bool CreateCFlashbangEmitGrenadeDetour(char *error, int maxlength)
{
	g_CFlashbangEmitGrenadeDetour = DETOUR_CREATE_MEMBER(DetourCFlashbangEmitGrenade, "CFlashbangEmitGrenade");

	if (g_CFlashbangEmitGrenadeDetour != NULL)
	{
		g_CFlashbangEmitGrenadeDetour->EnableDetour();
		g_CFlashbangEmitGrenadeDetoured = true;
		return true;
	}

	if (error)
		snprintf(error, maxlength, "Unable to find address of CFlashbangEmitGrenade for detour");

	return false;
}

bool CreateCMolotovEmitGrenadeDetour(char *error, int maxlength)
{
	g_CMolotovEmitGrenadeDetour = DETOUR_CREATE_MEMBER(DetourCMolotovEmitGrenade, "CMolotovEmitGrenade");

	if (g_CMolotovEmitGrenadeDetour != NULL)
	{
		g_CMolotovEmitGrenadeDetour->EnableDetour();
		g_CMolotovEmitGrenadeDetoured = true;
		return true;
	}

	if (error)
		snprintf(error, maxlength, "Unable to find address of CMolotovEmitGrenade for detour");

	return false;
}

#ifndef WIN32
bool CreateCIncendiaryGrenadeEmitGrenadeDetour(char *error, int maxlength)
{
	g_CIncendiaryGrenadeEmitGrenadeDetour = DETOUR_CREATE_MEMBER(DetourCIncendiaryGrenadeEmitGrenade, "CIncendiaryGrenadeEmitGrenade");

	if (g_CIncendiaryGrenadeEmitGrenadeDetour != NULL)
	{
		g_CIncendiaryGrenadeEmitGrenadeDetour->EnableDetour();
		g_CIncendiaryGrenadeEmitGrenadeDetoured = true;
		return true;
	}

	if (error)
		snprintf(error, maxlength, "Unable to find address of CIncendiaryGrenadeEmitGrenade for detour");

	return false;
}
#endif

bool CreateCDecoyGrenadeEmitGrenadeDetour(char *error, int maxlength)
{
	g_CDecoyGrenadeEmitGrenadeDetour = DETOUR_CREATE_MEMBER(DetourCDecoyGrenadeEmitGrenade, "CDecoyGrenadeEmitGrenade");

	if (g_CDecoyGrenadeEmitGrenadeDetour != NULL)
	{
		g_CDecoyGrenadeEmitGrenadeDetour->EnableDetour();
		g_CDecoyGrenadeEmitGrenadeDetoured = true;
		return true;
	}

	if (error)
		snprintf(error, maxlength, "Unable to find address of CDecoyGrenadeEmitGrenade for detour");

	return false;
}

bool CreateCHEGrenadeEmitGrenadeDetour(char *error, int maxlength)
{
	g_CHEGrenadeEmitGrenadeDetour = DETOUR_CREATE_MEMBER(DetourCHEGrenadeEmitGrenade, "CHEGrenadeEmitGrenade");

	if (g_CHEGrenadeEmitGrenadeDetour != NULL)
	{
		g_CHEGrenadeEmitGrenadeDetour->EnableDetour();
		g_CHEGrenadeEmitGrenadeDetoured = true;
		return true;
	}

	if (error)
		snprintf(error, maxlength, "Unable to find address of CHEGrenadeEmitGrenade for detour");

	return false;
}

bool CreateCSmokeGrenadeProjectileSmokeDetonateDetour(char *error, int maxlength)
{
	g_CSmokeGrenadeProjectileSmokeDetonateDetour = DETOUR_CREATE_MEMBER(DetourCSmokeGrenadeProjectileSmokeDetonate, "CSmokeGrenadeProjectileSmokeDetonate");

	if (g_CSmokeGrenadeProjectileSmokeDetonateDetour != NULL)
	{
		g_CSmokeGrenadeProjectileSmokeDetonateDetour->EnableDetour();
		g_CSmokeGrenadeProjectileSmokeDetonateDetoured = true;
		return true;
	}

	if (error)
		snprintf(error, maxlength, "Unable to find address of CSmokeGrenadeProjectileSmokeDetonate for detour");

	return false;
}

bool CreateCMolotovProjectileDetonateDetour(char *error, int maxlength)
{
	g_CMolotovProjectileDetonateDetour = DETOUR_CREATE_MEMBER(DetourCMolotovProjectileDetonate, "CMolotovProjectileDetonate");

	if (g_CMolotovProjectileDetonateDetour != NULL)
	{
		g_CMolotovProjectileDetonateDetour->EnableDetour();
		g_CMolotovProjectileDetonateDetoured = true;
		return true;
	}

	if (error)
		snprintf(error, maxlength, "Unable to find address of CMolotovProjectileDetonate for detour");

	return false;
}

bool CreateCFlashbangProjectileDetonateDetour(char *error, int maxlength)
{
	g_CFlashbangProjectileDetonateDetour = DETOUR_CREATE_MEMBER(DetourCFlashbangProjectileDetonate, "CFlashbangProjectileDetonate");

	if (g_CFlashbangProjectileDetonateDetour != NULL)
	{
		g_CFlashbangProjectileDetonateDetour->EnableDetour();
		g_CFlashbangProjectileDetonateDetoured = true;
		return true;
	}

	if (error)
		snprintf(error, maxlength, "Unable to find address of CFlashbangProjectileDetonate for detour");

	return false;
}

bool CreateCCSPlayerDeafenDetour(char *error, int maxlength)
{
	g_CCSPlayerDeafenDetour = DETOUR_CREATE_MEMBER(DetourCCSPlayerDeafen, "CCSPlayerDeafen");

	if (g_CCSPlayerDeafenDetour != NULL)
	{
		g_CCSPlayerDeafenDetour->EnableDetour();
		g_CCSPlayerDeafenDetoured = true;
		return true;
	}

	if (error)
		snprintf(error, maxlength, "Unable to find address of CCSPlayerDeafen for detour");

	return false;
}

bool CreateCCSPlayerBlindDetour(char *error, int maxlength)
{
	g_CCSPlayerBlindDetour = DETOUR_CREATE_MEMBER(DetourCCSPlayerBlind, "CCSPlayerBlind");

	if (g_CCSPlayerBlindDetour != NULL)
	{
		g_CCSPlayerBlindDetour->EnableDetour();
		g_CCSPlayerBlindDetoured = true;
		return true;
	}

	if (error)
		snprintf(error, maxlength, "Unable to find address of CCSPlayerBlind for detour");

	return false;
}

bool CreateCHEGrenadeProjectileDetonateDetour(char *error, int maxlength)
{
	g_CHEGrenadeProjectileDetonateDetour = DETOUR_CREATE_MEMBER(DetourCHEGrenadeProjectileDetonate, "CHEGrenadeProjectileDetonate");

	if (g_CHEGrenadeProjectileDetonateDetour != NULL)
	{
		g_CHEGrenadeProjectileDetonateDetour->EnableDetour();
		g_CHEGrenadeProjectileDetonateDetoured = true;
		return true;
	}

	if (error)
		snprintf(error, maxlength, "Unable to find address of CHEGrenadeProjectileDetonate for detour");

	return false;
}

bool CreateCDecoyProjectileGunfireThinkDetour(char *error, int maxlength)
{
	g_CDecoyProjectileGunfireThinkDetour = DETOUR_CREATE_MEMBER(DetourCDecoyProjectileGunfireThink, "CDecoyProjectileGunfireThink");

	if (g_CDecoyProjectileGunfireThinkDetour != NULL)
	{
		g_CDecoyProjectileGunfireThinkDetour->EnableDetour();
		g_CDecoyProjectileGunfireThinkDetoured = true;
		return true;
	}

	if (error)
		snprintf(error, maxlength, "Unable to find address of CDecoyProjectileGunfireThink for detour");

	return false;
}

bool CreateCInfernoInfernoThinkDetour(char *error, int maxlength)
{
	g_CInfernoInfernoThinkDetour = DETOUR_CREATE_MEMBER(DetourCInfernoInfernoThink, "CInfernoInfernoThink");

	if (g_CInfernoInfernoThinkDetour != NULL)
	{
		g_CInfernoInfernoThinkDetour->EnableDetour();
		g_CInfernoInfernoThinkDetoured = true;
		return true;
	}

	if (error)
		snprintf(error, maxlength, "Unable to find address of CInfernoInfernoThink for detour");

	return false;
}

bool CreatePassServerEntityFilterDetour(char *error, int maxlength)
{
	g_PassServerEntityFilterDetour = DETOUR_CREATE_STATIC(DetourPassServerEntityFilter, "PassServerEntityFilter");

	if (g_PassServerEntityFilterDetour != NULL)
	{
		g_PassServerEntityFilterDetour->EnableDetour();
		g_PassServerEntityFilterDetoured = true;
		return true;
	}

	if (error)
		snprintf(error, maxlength, "Unable to find address of PassServerEntityFilter for detour");

	return false;
}