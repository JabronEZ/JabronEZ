#include "globals.h"

CCSWeaponInfo *g_SmokeWeaponInfo = NULL;
CCSWeaponInfo *g_FlashWeaponInfo = NULL;
CCSWeaponInfo *g_DecoyWeaponInfo = NULL;
CCSWeaponInfo *g_MolotovWeaponInfo = NULL;
CCSWeaponInfo *g_IncendiaryWeaponInfo = NULL;
CCSWeaponInfo *g_HEGrenadeWeaponInfo = NULL;
GetWeaponInfoFn g_GetWeaponInfo = NULL;

ISDKTools *g_pSDKTools = NULL;
IGameConfig *g_pGameConf = NULL;
unsigned int g_ModelNameOffset = 0;
IPhysics *g_Physics = NULL;

cell_t g_CurrentlyDetonatingSmokeGrenade = INVALID_ENT_REFERENCE;
cell_t g_CurrentlyDetonatingFlashbang = INVALID_ENT_REFERENCE;
cell_t g_CurrentlyDetonatingHEGrenade = INVALID_ENT_REFERENCE;
cell_t g_CurrentlyDetonatingMolotov = INVALID_ENT_REFERENCE;
cell_t g_CurrentlyDetonatingIncendiary = INVALID_ENT_REFERENCE;
cell_t g_CurrentlyGunfireThinkingDecoy = INVALID_ENT_REFERENCE;
cell_t g_CurrentlyThinkingInferno = INVALID_ENT_REFERENCE;

void GetGrenadeWeaponInfo(bool p_Reload)
{
	if (!g_GetWeaponInfo)
		return;

	if (p_Reload)
	{
		g_SmokeWeaponInfo = NULL;
		g_FlashWeaponInfo = NULL;
		g_DecoyWeaponInfo = NULL;
		g_MolotovWeaponInfo = NULL;
		g_IncendiaryWeaponInfo = NULL;
		g_HEGrenadeWeaponInfo = NULL;
	}

	if (!g_SmokeWeaponInfo)
		g_SmokeWeaponInfo = g_GetWeaponInfo(SMOKE_WEAPON_ID);

	if (!g_FlashWeaponInfo)
		g_FlashWeaponInfo = g_GetWeaponInfo(FLASH_WEAPON_ID);

	if (!g_DecoyWeaponInfo)
		g_DecoyWeaponInfo = g_GetWeaponInfo(DECOY_WEAPON_ID);

	if (!g_MolotovWeaponInfo)
		g_MolotovWeaponInfo = g_GetWeaponInfo(MOLOTOV_WEAPON_ID);

	if (!g_IncendiaryWeaponInfo)
		g_IncendiaryWeaponInfo = g_GetWeaponInfo(INCENDIARY_WEAPON_ID);

	if (!g_HEGrenadeWeaponInfo)
		g_HEGrenadeWeaponInfo = g_GetWeaponInfo(HEGRENADE_WEAPON_ID);
}