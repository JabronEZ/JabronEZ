/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod Counter-Strike:Source Extension
 * Copyright (C) 2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#include "extension.h"
#include "iplayerinfo.h"
#include <eiface.h>
#include "globals.h"
#include "natives.h"
#include "forwards.h"

SH_DECL_HOOK0(IPhysics, CreateEnvironment, SH_NOATTRIB, 0, IPhysicsEnvironment *);
SH_DECL_HOOK1_void(IPhysicsEnvironment, SetCollisionSolver, SH_NOATTRIB, 0, IPhysicsCollisionSolver *);
SH_DECL_HOOK4(IPhysicsCollisionSolver, ShouldCollide, SH_NOATTRIB, 0, int, IPhysicsObject *, IPhysicsObject *, void *, void *);

Freedom35 g_Freedom35;
SMEXT_LINK(&g_Freedom35);

bool Freedom35::SDK_OnLoad(char *error, size_t maxlength, bool late)
{
	if (strcmp(g_pSM->GetGameFolderName(), "csgo") != 0)
	{
		snprintf(error, maxlength, "Cannot Load Freedom Extension on mod other than CS:GO");
		return false;
	}

	sharesys->AddDependency(myself, "bintools.ext", true, true);

	char s_ConfError[256];
	if (!gameconfs->LoadGameConfigFile("freedom35.games", &g_pGameConf, s_ConfError, sizeof(s_ConfError)))
	{
		if (error)
			snprintf(error, maxlength, "Could not read freedom35.games: %s", s_ConfError);

		return false;
	}

	CDetourManager::Init(g_pSM->GetScriptingEngine(), g_pGameConf);
	
	if (!CreateCSmokeGrenadeEmitGrenadeDetour(error, maxlength) ||
		!CreateCFlashbangEmitGrenadeDetour(error, maxlength) ||
		!CreateCMolotovEmitGrenadeDetour(error, maxlength) ||
#ifndef WIN32
		!CreateCIncendiaryGrenadeEmitGrenadeDetour(error, maxlength) ||
#endif
		!CreateCDecoyGrenadeEmitGrenadeDetour(error, maxlength) ||
		!CreateCHEGrenadeEmitGrenadeDetour(error, maxlength) ||
		!CreateCSmokeGrenadeProjectileSmokeDetonateDetour(error, maxlength) ||
		!CreateCMolotovProjectileDetonateDetour(error, maxlength) ||
		!CreateCFlashbangProjectileDetonateDetour(error, maxlength) ||
		!CreateCCSPlayerBlindDetour(error, maxlength) ||
		!CreateCCSPlayerDeafenDetour(error, maxlength) ||
		!CreateCHEGrenadeProjectileDetonateDetour(error, maxlength) ||
		!CreateCDecoyProjectileGunfireThinkDetour(error, maxlength) ||
		!CreateCInfernoInfernoThinkDetour(error, maxlength) ||
		!CreatePassServerEntityFilterDetour(error, maxlength))
	{
		return false;
	}
	
	if (!FindCSmokeGrenadeProjectileCreate(error, maxlength) ||
		!FindCFlashbangProjectileCreate(error, maxlength) ||
		!FindCMolotovProjectileCreate(error, maxlength) ||
		!FindCDecoyProjectileCreate(error, maxlength) ||
		!FindCHEGrenadeProjectileCreate(error, maxlength) ||
		!FindGetWeaponInfo(error, maxlength) ||
		!FindCSmokeGrenadeProjectileSmokeDetonate(error, maxlength) ||
		!FindCSmokeGrenadeProjectileThinkFade(error, maxlength) ||
		!FindCMolotovProjectileDetonate(error, maxlength) ||
		!FindCBaseGrenadeDetonate(error, maxlength) ||
		!FindCFlashbangProjectileDetonate(error, maxlength) ||
		!FindCHEGrenadeProjectileDetonate(error, maxlength) ||
		!FindCCSPlayerGiveNamedItem(error, maxlength) ||
		!FindGetItemSchema(error, maxlength) ||
		!FindGetItemDefinitionByNameOffset(error, maxlength) ||
		!FindGetItemDefinitionIndexOffset(error, maxlength) ||
		!FindGetLoadoutSlot(error, maxlength) || 
		!FindInventoryOffset(error, maxlength) ||
		!FindGetItemInLoadoutOffset(error, maxlength) ||
		!FindCCSGameRulesGoToIntermission(error, maxlength) ||
		!FindScoreboardPopupTimeOffset(error, maxlength))
		/**!FindCCSPlayerBlind(error, maxlength) || 
		!FindCCSPlayerDeafen(error, maxlength))*/
	{
		return false;
	}

	sharesys->AddNatives(myself, g_Freedom35Natives);
	
	g_EmitSmokeGrenadeForward = forwards->CreateForward("JEZ_OnEmitSmokeGrenade", ET_Ignore, 6, NULL, Param_Cell, Param_Array, Param_Array, Param_Array, Param_Array, Param_CellByRef);
	g_EmitFlashGrenadeForward = forwards->CreateForward("JEZ_OnEmitFlashGrenade", ET_Ignore, 6, NULL, Param_Cell, Param_Array, Param_Array, Param_Array, Param_Array, Param_CellByRef);
	g_EmitMolotovGrenadeForward = forwards->CreateForward("JEZ_OnEmitMolotovGrenade", ET_Ignore, 6, NULL, Param_Cell, Param_Array, Param_Array, Param_Array, Param_Array, Param_CellByRef);
	g_EmitIncendiaryGrenadeForward = forwards->CreateForward("JEZ_OnEmitIncendiaryGrenade", ET_Ignore, 6, NULL, Param_Cell, Param_Array, Param_Array, Param_Array, Param_Array, Param_CellByRef);
	g_EmitDecoyGrenadeForward = forwards->CreateForward("JEZ_OnEmitDecoyGrenade", ET_Ignore, 6, NULL, Param_Cell, Param_Array, Param_Array, Param_Array, Param_Array, Param_CellByRef);
	g_EmitHEGrenadeForward = forwards->CreateForward("JEZ_OnEmitHEGrenade", ET_Ignore, 6, NULL, Param_Cell, Param_Array, Param_Array, Param_Array, Param_Array, Param_CellByRef);
	g_OnSmokeGrenadeDetonateForward = forwards->CreateForward("JEZ_OnSmokeGrenadeDetonate", ET_Ignore, 2, NULL, Param_Cell, Param_CellByRef);
	g_OnMolotovDetonateForward = forwards->CreateForward("JEZ_OnMolotovDetonate", ET_Ignore, 2, NULL, Param_Cell, Param_CellByRef);
	g_OnIncendiaryDetonateForward = forwards->CreateForward("JEZ_OnIncendiaryDetonate", ET_Ignore, 2, NULL, Param_Cell, Param_CellByRef);
	g_OnFlashbangDetonateForward = forwards->CreateForward("JEZ_OnFlashbangDetonate", ET_Ignore, 2, NULL, Param_Cell, Param_CellByRef);
	g_OnFlashbangBlindForward = forwards->CreateForward("JEZ_OnFlashbangBlind", ET_Ignore, 6, NULL, Param_Cell, Param_Cell, Param_Cell, Param_Cell, Param_Cell, Param_CellByRef);
	g_OnFlashbangDeafenForward = forwards->CreateForward("JEZ_OnFlashbangDeafen", ET_Ignore, 4, NULL, Param_Cell, Param_Cell, Param_Cell, Param_CellByRef);
	g_OnHEGrenadeDetonateForward = forwards->CreateForward("JEZ_OnHEGrenadeDetonate", ET_Ignore, 2, NULL, Param_Cell, Param_CellByRef);
	g_PassServerEntityFilterForward = forwards->CreateForward("JEZ_OnPassServerEntityFilter", ET_Hook, 3, NULL, Param_Cell, Param_Cell, Param_CellByRef);
	g_ShouldCollideForward = forwards->CreateForward("JEZ_OnShouldCollide", ET_Hook, 3, NULL, Param_Cell, Param_Cell, Param_CellByRef);
	
	return true;
}

bool Freedom35::SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer, INTERFACEVERSION_VENGINESERVER);
	GET_V_IFACE_CURRENT(GetPhysicsFactory, g_Physics, IPhysics, VPHYSICS_INTERFACE_VERSION);

	META_CONPRINTF("Physics found: %p\n", g_Physics);

	SH_ADD_HOOK(IPhysics, CreateEnvironment, g_Physics, SH_MEMBER(this, &Freedom35::CreateEnvironmentHook), false);

	ismm->AddListener(this, this);

	return true;
}

void Freedom35::SDK_OnUnload()
{
	gameconfs->CloseGameConfigFile(g_pGameConf);
}

void Freedom35::SDK_OnAllLoaded()
{
	SM_GET_LATE_IFACE(SDKTOOLS, g_pSDKTools);

	if (g_pSDKTools == NULL)
		smutils->LogError(myself, "SDKTools interface not found.");
}

bool Freedom35::QueryRunning(char *error, size_t maxlength)
{
	return true;
}

const char *Freedom35::GetExtensionVerString()
{
	return SMEXT_CONF_VERSION;
}

const char *Freedom35::GetExtensionDateString()
{
	return SMEXT_CONF_DATESTRING;
}

void Freedom35::OnLevelInit(char const *pMapName, char const *pMapEntities, char const *pOldLevel, char const *pLandmarkName, bool loadGame, bool background)
{
	GetGrenadeWeaponInfo(true);

	META_CONPRINTF("Flash: %p\nSmoke: %p\nDecoy: %p\nMolotov: %p\nIncendiary: %p\nHE Grenade: %p\n", g_FlashWeaponInfo, g_SmokeWeaponInfo, g_DecoyWeaponInfo, g_MolotovWeaponInfo, g_IncendiaryWeaponInfo, g_HEGrenadeWeaponInfo);
}

IPhysicsEnvironment *Freedom35::CreateEnvironmentHook()
{
	// in order to hook IPhysicsCollisionSolver::ShouldCollide, we need to know when a solver is installed
	// in order to hook any installed solvers, we need to hook any created physics environments

	IPhysicsEnvironment *pEnvironment = SH_CALL(g_Physics, &IPhysics::CreateEnvironment)();

	if (!pEnvironment)
		RETURN_META_VALUE(MRES_SUPERCEDE, pEnvironment); // just in case

	// hook so we know when a solver is installed
	SH_ADD_HOOK(IPhysicsEnvironment, SetCollisionSolver, pEnvironment, SH_MEMBER(this, &Freedom35::SetCollisionSolverHook), false);

	RETURN_META_VALUE(MRES_SUPERCEDE, pEnvironment);
}

void Freedom35::SetCollisionSolverHook(IPhysicsCollisionSolver *p_Solver)
{
	if (!p_Solver)
		RETURN_META(MRES_IGNORED);

	SH_ADD_HOOK(IPhysicsCollisionSolver, ShouldCollide, p_Solver, SH_MEMBER(this, &Freedom35::ShouldCollideHook), false);
	RETURN_META(MRES_IGNORED);
}

int Freedom35::ShouldCollideHook(IPhysicsObject *p_Obj1, IPhysicsObject *p_Obj2, void *p_GameData1, void *p_GameData2)
{
	if (g_ShouldCollideForward->GetFunctionCount() == 0)
		RETURN_META_VALUE(MRES_IGNORED, 1);

	if (p_Obj1 == p_Obj2)
		RETURN_META_VALUE(MRES_IGNORED, 1);

	CBaseEntity *s_Ent1 = reinterpret_cast<CBaseEntity *>(p_GameData1);
	CBaseEntity *s_Ent2 = reinterpret_cast<CBaseEntity *>(p_GameData2);

	if(!s_Ent1 || !s_Ent2)
		RETURN_META_VALUE(MRES_IGNORED, 1);

	cell_t s_EntRef1 = gamehelpers->EntityToBCompatRef(s_Ent1);
	cell_t s_EntRef2 = gamehelpers->EntityToBCompatRef(s_Ent2);

	cell_t s_Result = 0;
	g_ShouldCollideForward->PushCell(s_EntRef1);
	g_ShouldCollideForward->PushCell(s_EntRef2);
	g_ShouldCollideForward->PushCellByRef(&s_Result);

	cell_t s_RetValue = 0;
	g_ShouldCollideForward->Execute(&s_RetValue);

	if(s_RetValue > Pl_Continue)
		RETURN_META_VALUE(MRES_SUPERCEDE, s_Result == 1);

	RETURN_META_VALUE(MRES_IGNORED, 0);
}