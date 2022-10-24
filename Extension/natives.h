#ifndef __NATIVES_H__
#define __NATIVES_H__

class CBaseEntity;
class CBasePlayer;
class CCSWeaponInfo;
class CEconItemView;
class CEconItemDefinition;

#ifdef WIN32
	typedef CBaseEntity* (__fastcall *CSmokeGrenadeProjectileCreateFn)(const Vector &, const QAngle &, const Vector &, const Vector &, CBasePlayer *, const CCSWeaponInfo &);
#else
	typedef CBaseEntity* (__cdecl *CSmokeGrenadeProjectileCreateFn)(const Vector &, const QAngle &, const Vector &, const Vector &, CBasePlayer *, const CCSWeaponInfo &);
#endif
extern CSmokeGrenadeProjectileCreateFn g_CSmokeGrenadeProjectileCreate;

#ifdef WIN32
	typedef CBaseEntity* (__fastcall *CFlashbangProjectileCreateFn)(const Vector &, const QAngle &, const Vector &, const Vector &, CBasePlayer *, const CCSWeaponInfo &);
#else
	typedef CBaseEntity* (__cdecl *CFlashbangProjectileCreateFn)(const Vector &, const QAngle &, const Vector &, const Vector &, CBasePlayer *, const CCSWeaponInfo &);
#endif
extern CFlashbangProjectileCreateFn g_CFlashbangProjectileCreate;

#ifdef WIN32
	typedef CBaseEntity* (__fastcall *CMolotovProjectileCreateFn)(const Vector &, const QAngle &, const Vector &, const Vector &, CBasePlayer *, const CCSWeaponInfo &);
#else
	typedef CBaseEntity* (__cdecl *CMolotovProjectileCreateFn)(const Vector &, const QAngle &, const Vector &, const Vector &, CBasePlayer *, const CCSWeaponInfo &);
#endif
extern CMolotovProjectileCreateFn g_CMolotovProjectileCreate;

#ifdef WIN32
	typedef CBaseEntity* (__fastcall *CDecoyProjectileCreateFn)(const Vector &, const QAngle &, const Vector &, const Vector &, CBasePlayer *, const CCSWeaponInfo &);
#else
	typedef CBaseEntity* (__cdecl *CDecoyProjectileCreateFn)(const Vector &, const QAngle &, const Vector &, const Vector &, CBasePlayer *, const CCSWeaponInfo &);
#endif
extern CDecoyProjectileCreateFn g_CDecoyProjectileCreate;

#ifdef WIN32
	typedef CBaseEntity* (__fastcall *CHEGrenadeProjectileCreateFn)(const Vector &, const QAngle &, const Vector &, const Vector &, CBasePlayer *, const CCSWeaponInfo &);
#else
	typedef CBaseEntity* (__cdecl *CHEGrenadeProjectileCreateFn)(const Vector &, const QAngle &, const Vector &, const Vector &, CBasePlayer *, const CCSWeaponInfo &);
#endif
extern CHEGrenadeProjectileCreateFn g_CHEGrenadeProjectileCreate;

#ifdef WIN32
	typedef void (__thiscall *CSmokeGrenadeProjectileSmokeDetonateFn)(CBaseEntity*);
#else
	typedef void(__cdecl *CSmokeGrenadeProjectileSmokeDetonateFn)(CBaseEntity*);
#endif
extern CSmokeGrenadeProjectileSmokeDetonateFn g_CSmokeGrenadeProjectileSmokeDetonate;

#ifdef WIN32
	typedef void(__thiscall *CSmokeGrenadeProjectileThinkFadeFn)(CBaseEntity *);
#else
	typedef void(__cdecl *CSmokeGrenadeProjectileThinkFadeFn)(CBaseEntity *);
#endif
extern CSmokeGrenadeProjectileThinkFadeFn g_CSmokeGrenadeProjectileThinkFade;

#ifdef WIN32
	typedef void(__thiscall *CMolotovProjectileDetonateFn)(CBaseEntity *);
#else
	typedef void(__cdecl *CMolotovProjectileDetonateFn)(CBaseEntity *);
#endif
extern CMolotovProjectileDetonateFn g_CMolotovProjectileDetonate;

#ifdef WIN32
	typedef void(__thiscall *CBaseGrenadeDetonateFn)(CBaseEntity *);
#else
	typedef void(__cdecl *CBaseGrenadeDetonateFn)(CBaseEntity *);
#endif
extern CBaseGrenadeDetonateFn g_CBaseGrenadeDetonate;

#ifdef WIN32
typedef void(__thiscall *CFlashbangProjectileDetonateFn)(CBaseEntity *);
#else
typedef void(__cdecl *CFlashbangProjectileDetonateFn)(CBaseEntity *);
#endif
extern CFlashbangProjectileDetonateFn g_CFlashbangProjectileDetonate;

#ifdef WIN32
typedef void(__thiscall *CHEGrenadeProjectileDetonateFn)(CBaseEntity *);
#else
typedef void(__cdecl *CHEGrenadeProjectileDetonateFn)(CBaseEntity *);
#endif
extern CHEGrenadeProjectileDetonateFn g_CHEGrenadeProjectileDetonate;

#ifdef WIN32
typedef CBaseEntity* (__thiscall *CCSPlayerGiveNamedItemFn)(CBaseEntity *, const char *, int, CEconItemView *, bool);
#else
typedef CBaseEntity* (__cdecl *CCSPlayerGiveNamedItemFn)(CBaseEntity *, const char *, int, CEconItemView *, bool);
#endif
extern CCSPlayerGiveNamedItemFn g_CCSPlayerGiveNamedItem;

typedef void* (__cdecl *GetItemSchemaFn)();
extern GetItemSchemaFn g_GetItemSchema;

#ifdef WIN32
typedef CEconItemDefinition* (__thiscall *GetItemDefinitionByNameFn)(void *, const char *);
#else
typedef CEconItemDefinition* (__cdecl *GetItemDefinitionByNameFn)(void *, const char *);
#endif
extern int g_GetItemDefinitionByNameOffset;

#ifdef WIN32
typedef unsigned int (__thiscall *GetItemDefinitionIndexFn)(void *);
#else
typedef unsigned int (__cdecl *GetItemDefinitionIndexFn)(void *);
#endif
extern int g_GetItemDefinitionIndexOffset;

#ifdef WIN32
typedef int (__thiscall *GetLoadoutSlotFn)(void *, int);
#else
typedef int (__cdecl *GetLoadoutSlotFn)(void *, int);
#endif
extern GetLoadoutSlotFn g_GetLoadoutSlot;

#ifdef WIN32
typedef CEconItemView* (__thiscall *GetItemInLoadoutFn)(void *, int, int);
#else
typedef CEconItemView* (__cdecl *GetItemInLoadoutFn)(void *, int, int);
#endif
extern int g_GetItemInLoadoutOffset;
extern int g_InventoryOffset;
extern int g_ScoreboardPopupTimeOffset;

#ifdef WIN32
typedef int (__thiscall *CCSGameRulesGoToIntermissionFn)(void *, bool);
#else
typedef int(__cdecl *CCSGameRulesGoToIntermissionFn)(void *, bool);
#endif
extern CCSGameRulesGoToIntermissionFn g_CCSGameRulesGoToIntermission;

bool FindCSmokeGrenadeProjectileCreate(char *error, int maxlength);
bool FindCFlashbangProjectileCreate(char *error, int maxlength);
bool FindCMolotovProjectileCreate(char *error, int maxlength);
bool FindCDecoyProjectileCreate(char *error, int maxlength);
bool FindCHEGrenadeProjectileCreate(char *error, int maxlength);
bool FindGetWeaponInfo(char *error, int maxlength);
bool FindCSmokeGrenadeProjectileSmokeDetonate(char *error, int maxlength);
bool FindCSmokeGrenadeProjectileThinkFade(char *error, int maxlength);
bool FindCMolotovProjectileDetonate(char *error, int maxlength);
bool FindCBaseGrenadeDetonate(char *error, int maxlength);
bool FindCFlashbangProjectileDetonate(char *error, int maxlength);
bool FindCHEGrenadeProjectileDetonate(char *error, int maxlength);
bool FindCCSPlayerGiveNamedItem(char *error, int maxlength);
bool FindGetItemSchema(char *error, int maxlength);
bool FindGetItemDefinitionByNameOffset(char *error, int maxlength);
bool FindGetItemDefinitionIndexOffset(char *error, int maxlength);
bool FindGetLoadoutSlot(char *error, int maxlength);
bool FindInventoryOffset(char *error, int maxlength);
bool FindGetItemInLoadoutOffset(char *error, int maxlength);
bool FindCCSGameRulesGoToIntermission(char *error, int maxlength);
bool FindScoreboardPopupTimeOffset(char *error, int maxlength);

extern sp_nativeinfo_t g_Freedom35Natives[];

#endif