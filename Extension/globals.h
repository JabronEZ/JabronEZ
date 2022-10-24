#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "extension.h"
#include "ISDKTools.h"

class CBaseEntity;
class CBasePlayer;
class CCSWeaponInfo;
class IPhysics;

extern ISDKTools *g_pSDKTools;
extern IPhysics *g_Physics;

extern CCSWeaponInfo *g_SmokeWeaponInfo;
extern CCSWeaponInfo *g_FlashWeaponInfo;
extern CCSWeaponInfo *g_DecoyWeaponInfo;
extern CCSWeaponInfo *g_IncendiaryWeaponInfo;
extern CCSWeaponInfo *g_MolotovWeaponInfo;
extern CCSWeaponInfo *g_HEGrenadeWeaponInfo;

#ifdef WIN32
	typedef CCSWeaponInfo* (__thiscall *GetWeaponInfoFn)(int);
#else
	typedef CCSWeaponInfo* (__cdecl *GetWeaponInfoFn)(int);
#endif
extern GetWeaponInfoFn g_GetWeaponInfo;

#define FLASH_WEAPON_ID 43
#define HEGRENADE_WEAPON_ID 44
#define SMOKE_WEAPON_ID 45
#define MOLOTOV_WEAPON_ID 46
#define DECOY_WEAPON_ID 47
#define INCENDIARY_WEAPON_ID 48	

void GetGrenadeWeaponInfo(bool p_Reload = false);

extern IGameConfig *g_pGameConf;
extern unsigned int g_ModelNameOffset;

#ifndef INVALID_ENT_REFERENCE
#define INVALID_ENT_REFERENCE (cell_t)0xFFFFFFFF
#endif

extern cell_t g_CurrentlyDetonatingSmokeGrenade;
extern cell_t g_CurrentlyDetonatingFlashbang;
extern cell_t g_CurrentlyDetonatingHEGrenade;
extern cell_t g_CurrentlyDetonatingMolotov;
extern cell_t g_CurrentlyDetonatingIncendiary;
extern cell_t g_CurrentlyGunfireThinkingDecoy;
extern cell_t g_CurrentlyThinkingInferno;

#endif