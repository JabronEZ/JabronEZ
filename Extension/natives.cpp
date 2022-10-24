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
#include <server_class.h>
#include <iplayerinfo.h>
#include "natives.h"
#include "globals.h"

CSmokeGrenadeProjectileCreateFn g_CSmokeGrenadeProjectileCreate = NULL;
CFlashbangProjectileCreateFn g_CFlashbangProjectileCreate = NULL;
CMolotovProjectileCreateFn g_CMolotovProjectileCreate = NULL;
CDecoyProjectileCreateFn g_CDecoyProjectileCreate = NULL;
CHEGrenadeProjectileCreateFn g_CHEGrenadeProjectileCreate = NULL;
CSmokeGrenadeProjectileSmokeDetonateFn g_CSmokeGrenadeProjectileSmokeDetonate = NULL;
CSmokeGrenadeProjectileThinkFadeFn g_CSmokeGrenadeProjectileThinkFade = NULL;
CMolotovProjectileDetonateFn g_CMolotovProjectileDetonate = NULL;
CBaseGrenadeDetonateFn g_CBaseGrenadeDetonate = NULL;
CFlashbangProjectileDetonateFn g_CFlashbangProjectileDetonate = NULL;
CHEGrenadeProjectileDetonateFn g_CHEGrenadeProjectileDetonate = NULL;
CCSPlayerGiveNamedItemFn g_CCSPlayerGiveNamedItem = NULL;
GetItemSchemaFn g_GetItemSchema = NULL;
int g_GetItemDefinitionByNameOffset = -1;
int g_GetItemDefinitionIndexOffset = -1;
GetLoadoutSlotFn g_GetLoadoutSlot = NULL;
int g_GetItemInLoadoutOffset = -1;
int g_InventoryOffset = -1;
CCSGameRulesGoToIntermissionFn g_CCSGameRulesGoToIntermission = NULL;
int g_ScoreboardPopupTimeOffset = -1;

inline CBaseEntity *GetCBaseEntity(int num, bool isplayer)
{
	edict_t *pEdict = gamehelpers->EdictOfIndex(num);
	if (!pEdict || pEdict->IsFree())
	{
		return NULL;
	}

	if (num > 0 && num <= playerhelpers->GetMaxClients())
	{
		IGamePlayer *pPlayer = playerhelpers->GetGamePlayer(pEdict);
		if (!pPlayer || !pPlayer->IsConnected())
		{
			return NULL;
		}
	}
	else if (isplayer) 
	{
		return NULL;
	}

	IServerUnknown *pUnk;
	if ((pUnk=pEdict->GetUnknown()) == NULL)
	{
		return NULL;
	}

	return pUnk->GetBaseEntity();
}

CBaseEntity *UTIL_ThrowSmoke(Vector p_Origin, QAngle p_Angle, Vector p_Velocity, Vector p_AngularImpulse, CBasePlayer *p_Player)
{
	if (g_SmokeWeaponInfo == NULL || g_CSmokeGrenadeProjectileCreate == NULL)
		return NULL;

	const CCSWeaponInfo &s_WeaponInfo = (const CCSWeaponInfo &)*g_SmokeWeaponInfo;
	return g_CSmokeGrenadeProjectileCreate(p_Origin, p_Angle, p_Velocity, p_AngularImpulse, p_Player, s_WeaponInfo);
}

CBaseEntity *UTIL_ThrowMolotov(Vector p_Origin, QAngle p_Angle, Vector p_Velocity, Vector p_AngularImpulse, CBasePlayer *p_Player)
{
	if (g_MolotovWeaponInfo == NULL || g_CMolotovProjectileCreate == NULL)
		return NULL;

	const CCSWeaponInfo &s_WeaponInfo = (const CCSWeaponInfo &)*g_MolotovWeaponInfo;
	return g_CMolotovProjectileCreate(p_Origin, p_Angle, p_Velocity, p_AngularImpulse, p_Player, s_WeaponInfo);
}

CBaseEntity *UTIL_ThrowIncendiary(Vector p_Origin, QAngle p_Angle, Vector p_Velocity, Vector p_AngularImpulse, CBasePlayer *p_Player)
{
	if (g_IncendiaryWeaponInfo == NULL || g_CMolotovProjectileCreate == NULL)
		return NULL;

	const CCSWeaponInfo &s_WeaponInfo = (const CCSWeaponInfo &)*g_IncendiaryWeaponInfo;
	return g_CMolotovProjectileCreate(p_Origin, p_Angle, p_Velocity, p_AngularImpulse, p_Player, s_WeaponInfo);
}

CBaseEntity *UTIL_ThrowFlash(Vector p_Origin, QAngle p_Angle, Vector p_Velocity, Vector p_AngularImpulse, CBasePlayer *p_Player)
{
	if (g_FlashWeaponInfo == NULL || g_CFlashbangProjectileCreate == NULL)
		return NULL;

	const CCSWeaponInfo &s_WeaponInfo = (const CCSWeaponInfo &)*g_FlashWeaponInfo;
	return g_CFlashbangProjectileCreate(p_Origin, p_Angle, p_Velocity, p_AngularImpulse, p_Player, s_WeaponInfo);
}

CBaseEntity *UTIL_ThrowDecoy(Vector p_Origin, QAngle p_Angle, Vector p_Velocity, Vector p_AngularImpulse, CBasePlayer *p_Player)
{
	if (g_DecoyWeaponInfo == NULL || g_CDecoyProjectileCreate == NULL)
		return NULL;

	const CCSWeaponInfo &s_WeaponInfo = (const CCSWeaponInfo &)*g_DecoyWeaponInfo;
	return g_CDecoyProjectileCreate(p_Origin, p_Angle, p_Velocity, p_AngularImpulse, p_Player, s_WeaponInfo);
}

CBaseEntity *UTIL_ThrowHEGrenade(Vector p_Origin, QAngle p_Angle, Vector p_Velocity, Vector p_AngularImpulse, CBasePlayer *p_Player)
{
	if (g_HEGrenadeWeaponInfo == NULL || g_CHEGrenadeProjectileCreate == NULL)
		return NULL;

	const CCSWeaponInfo &s_WeaponInfo = (const CCSWeaponInfo &)*g_HEGrenadeWeaponInfo;
	return g_CHEGrenadeProjectileCreate(p_Origin, p_Angle, p_Velocity, p_AngularImpulse, p_Player, s_WeaponInfo);
}

void UTIL_DetonateSmokeGrenade(CBaseEntity *p_SmokeGrenade)
{
	if (g_CSmokeGrenadeProjectileSmokeDetonate == NULL)
		return;
	
	g_CSmokeGrenadeProjectileSmokeDetonate(p_SmokeGrenade);
}

void UTIL_DetonateBaseGrenade(CBaseEntity *p_BaseGrenade)
{
	if (g_CBaseGrenadeDetonate == NULL)
		return;

	g_CBaseGrenadeDetonate(p_BaseGrenade);
}

void UTIL_DetonateMolotovGrenade(CBaseEntity *p_MolotovGrenade)
{
	if (g_CMolotovProjectileDetonate == NULL)
		return;

	g_CMolotovProjectileDetonate(p_MolotovGrenade);
}

void UTIL_SmokeThinkFade(CBaseEntity *p_SmokeGrenade)
{
	if (g_CSmokeGrenadeProjectileThinkFade == NULL)
		return;

	g_CSmokeGrenadeProjectileThinkFade(p_SmokeGrenade);
}

void UTIL_DetonateFlashbang(CBaseEntity *p_Flashbang)
{
	if (g_CFlashbangProjectileDetonate == NULL)
		return;

	g_CFlashbangProjectileDetonate(p_Flashbang);
}

void UTIL_DetonateHEGrenade(CBaseEntity *p_HEGrenade)
{
	if (g_CHEGrenadeProjectileDetonate == NULL)
		return;

	g_CHEGrenadeProjectileDetonate(p_HEGrenade);
}


static cell_t ThrowSmokeNative(IPluginContext *pContext, const cell_t *params)
{
	if (params[0] < 5)
		return -1;

	int s_Client = params[1];
	IGamePlayer *s_GamePlayer = playerhelpers->GetGamePlayer(s_Client);
	IServerUnknown *s_Unknown = s_GamePlayer->GetEdict()->GetUnknown();
	CBasePlayer *s_BasePlayer = (CBasePlayer*)s_Unknown->GetBaseEntity();

	cell_t *s_OriginArray;
	pContext->LocalToPhysAddr(params[2], &s_OriginArray);
	float s_OriginX = sp_ctof(s_OriginArray[0]);
	float s_OriginY = sp_ctof(s_OriginArray[1]);
	float s_OriginZ = sp_ctof(s_OriginArray[2]);

	cell_t *s_AngleArray;
	pContext->LocalToPhysAddr(params[3], &s_AngleArray);
	float s_AngleX = sp_ctof(s_AngleArray[0]);
	float s_AngleY = sp_ctof(s_AngleArray[1]);
	float s_AngleZ = sp_ctof(s_AngleArray[2]);

	cell_t *s_VelocityArray;
	pContext->LocalToPhysAddr(params[4], &s_VelocityArray);
	float s_VelocityX = sp_ctof(s_VelocityArray[0]);
	float s_VelocityY = sp_ctof(s_VelocityArray[1]);
	float s_VelocityZ = sp_ctof(s_VelocityArray[2]);

	cell_t *s_AngularImpluseArray;
	pContext->LocalToPhysAddr(params[5], &s_AngularImpluseArray);
	float s_AngularImpulseX = sp_ctof(s_AngularImpluseArray[0]);
	float s_AngularImpulseY = sp_ctof(s_AngularImpluseArray[1]);
	float s_AngularImpulseZ = sp_ctof(s_AngularImpluseArray[2]);

	Vector s_Origin;
	s_Origin.x = s_OriginX;
	s_Origin.y = s_OriginY;
	s_Origin.z = s_OriginZ;

	QAngle s_Angle;
	s_Angle.x = s_AngleX;
	s_Angle.y = s_AngleY;
	s_Angle.z = s_AngleZ;

	Vector s_Velocity;
	s_Velocity.x = s_VelocityX;
	s_Velocity.y = s_VelocityY;
	s_Velocity.z = s_VelocityZ;

	Vector s_AngularImpulse;
	s_AngularImpulse.x = s_AngularImpulseX;
	s_AngularImpulse.y = s_AngularImpulseY;
	s_AngularImpulse.z = s_AngularImpulseZ;

	CBaseEntity *s_Grenade = UTIL_ThrowSmoke(s_Origin, s_Angle, s_Velocity, s_AngularImpulse, s_BasePlayer);

	cell_t s_Reference = gamehelpers->EntityToReference(s_Grenade);
	return gamehelpers->ReferenceToIndex(s_Reference);
}

static cell_t ThrowFlashNative(IPluginContext *pContext, const cell_t *params)
{
	if (params[0] < 5)
		return -1;

	int s_Client = params[1];
	IGamePlayer *s_GamePlayer = playerhelpers->GetGamePlayer(s_Client);
	IServerUnknown *s_Unknown = s_GamePlayer->GetEdict()->GetUnknown();
	CBasePlayer *s_BasePlayer = (CBasePlayer*)s_Unknown->GetBaseEntity();

	cell_t *s_OriginArray;
	pContext->LocalToPhysAddr(params[2], &s_OriginArray);
	float s_OriginX = sp_ctof(s_OriginArray[0]);
	float s_OriginY = sp_ctof(s_OriginArray[1]);
	float s_OriginZ = sp_ctof(s_OriginArray[2]);

	cell_t *s_AngleArray;
	pContext->LocalToPhysAddr(params[3], &s_AngleArray);
	float s_AngleX = sp_ctof(s_AngleArray[0]);
	float s_AngleY = sp_ctof(s_AngleArray[1]);
	float s_AngleZ = sp_ctof(s_AngleArray[2]);

	cell_t *s_VelocityArray;
	pContext->LocalToPhysAddr(params[4], &s_VelocityArray);
	float s_VelocityX = sp_ctof(s_VelocityArray[0]);
	float s_VelocityY = sp_ctof(s_VelocityArray[1]);
	float s_VelocityZ = sp_ctof(s_VelocityArray[2]);

	cell_t *s_AngularImpluseArray;
	pContext->LocalToPhysAddr(params[5], &s_AngularImpluseArray);
	float s_AngularImpulseX = sp_ctof(s_AngularImpluseArray[0]);
	float s_AngularImpulseY = sp_ctof(s_AngularImpluseArray[1]);
	float s_AngularImpulseZ = sp_ctof(s_AngularImpluseArray[2]);

	Vector s_Origin;
	s_Origin.x = s_OriginX;
	s_Origin.y = s_OriginY;
	s_Origin.z = s_OriginZ;

	QAngle s_Angle;
	s_Angle.x = s_AngleX;
	s_Angle.y = s_AngleY;
	s_Angle.z = s_AngleZ;

	Vector s_Velocity;
	s_Velocity.x = s_VelocityX;
	s_Velocity.y = s_VelocityY;
	s_Velocity.z = s_VelocityZ;

	Vector s_AngularImpulse;
	s_AngularImpulse.x = s_AngularImpulseX;
	s_AngularImpulse.y = s_AngularImpulseY;
	s_AngularImpulse.z = s_AngularImpulseZ;

	CBaseEntity *s_Grenade = UTIL_ThrowFlash(s_Origin, s_Angle, s_Velocity, s_AngularImpulse, s_BasePlayer);

	cell_t s_Reference = gamehelpers->EntityToReference(s_Grenade);
	return gamehelpers->ReferenceToIndex(s_Reference);
}

static cell_t ThrowMolotovNative(IPluginContext *pContext, const cell_t *params)
{
	if (params[0] < 5)
		return -1;

	int s_Client = params[1];
	IGamePlayer *s_GamePlayer = playerhelpers->GetGamePlayer(s_Client);
	IServerUnknown *s_Unknown = s_GamePlayer->GetEdict()->GetUnknown();
	CBasePlayer *s_BasePlayer = (CBasePlayer*)s_Unknown->GetBaseEntity();

	cell_t *s_OriginArray;
	pContext->LocalToPhysAddr(params[2], &s_OriginArray);
	float s_OriginX = sp_ctof(s_OriginArray[0]);
	float s_OriginY = sp_ctof(s_OriginArray[1]);
	float s_OriginZ = sp_ctof(s_OriginArray[2]);

	cell_t *s_AngleArray;
	pContext->LocalToPhysAddr(params[3], &s_AngleArray);
	float s_AngleX = sp_ctof(s_AngleArray[0]);
	float s_AngleY = sp_ctof(s_AngleArray[1]);
	float s_AngleZ = sp_ctof(s_AngleArray[2]);

	cell_t *s_VelocityArray;
	pContext->LocalToPhysAddr(params[4], &s_VelocityArray);
	float s_VelocityX = sp_ctof(s_VelocityArray[0]);
	float s_VelocityY = sp_ctof(s_VelocityArray[1]);
	float s_VelocityZ = sp_ctof(s_VelocityArray[2]);

	cell_t *s_AngularImpluseArray;
	pContext->LocalToPhysAddr(params[5], &s_AngularImpluseArray);
	float s_AngularImpulseX = sp_ctof(s_AngularImpluseArray[0]);
	float s_AngularImpulseY = sp_ctof(s_AngularImpluseArray[1]);
	float s_AngularImpulseZ = sp_ctof(s_AngularImpluseArray[2]);

	Vector s_Origin;
	s_Origin.x = s_OriginX;
	s_Origin.y = s_OriginY;
	s_Origin.z = s_OriginZ;

	QAngle s_Angle;
	s_Angle.x = s_AngleX;
	s_Angle.y = s_AngleY;
	s_Angle.z = s_AngleZ;

	Vector s_Velocity;
	s_Velocity.x = s_VelocityX;
	s_Velocity.y = s_VelocityY;
	s_Velocity.z = s_VelocityZ;

	Vector s_AngularImpulse;
	s_AngularImpulse.x = s_AngularImpulseX;
	s_AngularImpulse.y = s_AngularImpulseY;
	s_AngularImpulse.z = s_AngularImpulseZ;

	CBaseEntity *s_Grenade = UTIL_ThrowMolotov(s_Origin, s_Angle, s_Velocity, s_AngularImpulse, s_BasePlayer);

	cell_t s_Reference = gamehelpers->EntityToReference(s_Grenade);
	return gamehelpers->ReferenceToIndex(s_Reference);
}

static cell_t ThrowIncendiaryNative(IPluginContext *pContext, const cell_t *params)
{
	if (params[0] < 5)
		return -1;

	int s_Client = params[1];
	IGamePlayer *s_GamePlayer = playerhelpers->GetGamePlayer(s_Client);
	IServerUnknown *s_Unknown = s_GamePlayer->GetEdict()->GetUnknown();
	CBasePlayer *s_BasePlayer = (CBasePlayer*)s_Unknown->GetBaseEntity();

	cell_t *s_OriginArray;
	pContext->LocalToPhysAddr(params[2], &s_OriginArray);
	float s_OriginX = sp_ctof(s_OriginArray[0]);
	float s_OriginY = sp_ctof(s_OriginArray[1]);
	float s_OriginZ = sp_ctof(s_OriginArray[2]);

	cell_t *s_AngleArray;
	pContext->LocalToPhysAddr(params[3], &s_AngleArray);
	float s_AngleX = sp_ctof(s_AngleArray[0]);
	float s_AngleY = sp_ctof(s_AngleArray[1]);
	float s_AngleZ = sp_ctof(s_AngleArray[2]);

	cell_t *s_VelocityArray;
	pContext->LocalToPhysAddr(params[4], &s_VelocityArray);
	float s_VelocityX = sp_ctof(s_VelocityArray[0]);
	float s_VelocityY = sp_ctof(s_VelocityArray[1]);
	float s_VelocityZ = sp_ctof(s_VelocityArray[2]);

	cell_t *s_AngularImpluseArray;
	pContext->LocalToPhysAddr(params[5], &s_AngularImpluseArray);
	float s_AngularImpulseX = sp_ctof(s_AngularImpluseArray[0]);
	float s_AngularImpulseY = sp_ctof(s_AngularImpluseArray[1]);
	float s_AngularImpulseZ = sp_ctof(s_AngularImpluseArray[2]);

	Vector s_Origin;
	s_Origin.x = s_OriginX;
	s_Origin.y = s_OriginY;
	s_Origin.z = s_OriginZ;

	QAngle s_Angle;
	s_Angle.x = s_AngleX;
	s_Angle.y = s_AngleY;
	s_Angle.z = s_AngleZ;

	Vector s_Velocity;
	s_Velocity.x = s_VelocityX;
	s_Velocity.y = s_VelocityY;
	s_Velocity.z = s_VelocityZ;

	Vector s_AngularImpulse;
	s_AngularImpulse.x = s_AngularImpulseX;
	s_AngularImpulse.y = s_AngularImpulseY;
	s_AngularImpulse.z = s_AngularImpulseZ;

	CBaseEntity *s_Grenade = UTIL_ThrowIncendiary(s_Origin, s_Angle, s_Velocity, s_AngularImpulse, s_BasePlayer);

	cell_t s_Reference = gamehelpers->EntityToReference(s_Grenade);
	return gamehelpers->ReferenceToIndex(s_Reference);
}

static cell_t ThrowDecoyNative(IPluginContext *pContext, const cell_t *params)
{
	if (params[0] < 5)
		return -1;

	int s_Client = params[1];
	IGamePlayer *s_GamePlayer = playerhelpers->GetGamePlayer(s_Client);
	IServerUnknown *s_Unknown = s_GamePlayer->GetEdict()->GetUnknown();
	CBasePlayer *s_BasePlayer = (CBasePlayer*)s_Unknown->GetBaseEntity();

	cell_t *s_OriginArray;
	pContext->LocalToPhysAddr(params[2], &s_OriginArray);
	float s_OriginX = sp_ctof(s_OriginArray[0]);
	float s_OriginY = sp_ctof(s_OriginArray[1]);
	float s_OriginZ = sp_ctof(s_OriginArray[2]);

	cell_t *s_AngleArray;
	pContext->LocalToPhysAddr(params[3], &s_AngleArray);
	float s_AngleX = sp_ctof(s_AngleArray[0]);
	float s_AngleY = sp_ctof(s_AngleArray[1]);
	float s_AngleZ = sp_ctof(s_AngleArray[2]);

	cell_t *s_VelocityArray;
	pContext->LocalToPhysAddr(params[4], &s_VelocityArray);
	float s_VelocityX = sp_ctof(s_VelocityArray[0]);
	float s_VelocityY = sp_ctof(s_VelocityArray[1]);
	float s_VelocityZ = sp_ctof(s_VelocityArray[2]);

	cell_t *s_AngularImpluseArray;
	pContext->LocalToPhysAddr(params[5], &s_AngularImpluseArray);
	float s_AngularImpulseX = sp_ctof(s_AngularImpluseArray[0]);
	float s_AngularImpulseY = sp_ctof(s_AngularImpluseArray[1]);
	float s_AngularImpulseZ = sp_ctof(s_AngularImpluseArray[2]);

	Vector s_Origin;
	s_Origin.x = s_OriginX;
	s_Origin.y = s_OriginY;
	s_Origin.z = s_OriginZ;

	QAngle s_Angle;
	s_Angle.x = s_AngleX;
	s_Angle.y = s_AngleY;
	s_Angle.z = s_AngleZ;

	Vector s_Velocity;
	s_Velocity.x = s_VelocityX;
	s_Velocity.y = s_VelocityY;
	s_Velocity.z = s_VelocityZ;

	Vector s_AngularImpulse;
	s_AngularImpulse.x = s_AngularImpulseX;
	s_AngularImpulse.y = s_AngularImpulseY;
	s_AngularImpulse.z = s_AngularImpulseZ;

	CBaseEntity *s_Grenade = UTIL_ThrowDecoy(s_Origin, s_Angle, s_Velocity, s_AngularImpulse, s_BasePlayer);

	cell_t s_Reference = gamehelpers->EntityToReference(s_Grenade);
	return gamehelpers->ReferenceToIndex(s_Reference);
}

static cell_t ThrowHEGrenadeNative(IPluginContext *pContext, const cell_t *params)
{
	if (params[0] < 5)
		return -1;

	int s_Client = params[1];
	IGamePlayer *s_GamePlayer = playerhelpers->GetGamePlayer(s_Client);
	IServerUnknown *s_Unknown = s_GamePlayer->GetEdict()->GetUnknown();
	CBasePlayer *s_BasePlayer = (CBasePlayer*)s_Unknown->GetBaseEntity();

	cell_t *s_OriginArray;
	pContext->LocalToPhysAddr(params[2], &s_OriginArray);
	float s_OriginX = sp_ctof(s_OriginArray[0]);
	float s_OriginY = sp_ctof(s_OriginArray[1]);
	float s_OriginZ = sp_ctof(s_OriginArray[2]);

	cell_t *s_AngleArray;
	pContext->LocalToPhysAddr(params[3], &s_AngleArray);
	float s_AngleX = sp_ctof(s_AngleArray[0]);
	float s_AngleY = sp_ctof(s_AngleArray[1]);
	float s_AngleZ = sp_ctof(s_AngleArray[2]);

	cell_t *s_VelocityArray;
	pContext->LocalToPhysAddr(params[4], &s_VelocityArray);
	float s_VelocityX = sp_ctof(s_VelocityArray[0]);
	float s_VelocityY = sp_ctof(s_VelocityArray[1]);
	float s_VelocityZ = sp_ctof(s_VelocityArray[2]);

	cell_t *s_AngularImpluseArray;
	pContext->LocalToPhysAddr(params[5], &s_AngularImpluseArray);
	float s_AngularImpulseX = sp_ctof(s_AngularImpluseArray[0]);
	float s_AngularImpulseY = sp_ctof(s_AngularImpluseArray[1]);
	float s_AngularImpulseZ = sp_ctof(s_AngularImpluseArray[2]);

	Vector s_Origin;
	s_Origin.x = s_OriginX;
	s_Origin.y = s_OriginY;
	s_Origin.z = s_OriginZ;

	QAngle s_Angle;
	s_Angle.x = s_AngleX;
	s_Angle.y = s_AngleY;
	s_Angle.z = s_AngleZ;

	Vector s_Velocity;
	s_Velocity.x = s_VelocityX;
	s_Velocity.y = s_VelocityY;
	s_Velocity.z = s_VelocityZ;

	Vector s_AngularImpulse;
	s_AngularImpulse.x = s_AngularImpulseX;
	s_AngularImpulse.y = s_AngularImpulseY;
	s_AngularImpulse.z = s_AngularImpulseZ;

	CBaseEntity *s_Grenade = UTIL_ThrowHEGrenade(s_Origin, s_Angle, s_Velocity, s_AngularImpulse, s_BasePlayer);

	cell_t s_Reference = gamehelpers->EntityToReference(s_Grenade);
	return gamehelpers->ReferenceToIndex(s_Reference);
}

static cell_t DetonateSmokeGrenadeNative(IPluginContext *pContext, const cell_t *params)
{
	if (params[0] < 1)
		return 0;

	int s_SmokeIndex = params[1];
	edict_t *s_Edict = gamehelpers->EdictOfIndex(s_SmokeIndex);

	if (!s_Edict || s_Edict->IsFree())
		return 0;

	IServerUnknown *s_Unknown = s_Edict->GetUnknown();

	if (!s_Unknown)
		return 0;

	CBaseEntity *s_SmokeGrenade = (CBaseEntity*)s_Unknown->GetBaseEntity();

	UTIL_DetonateSmokeGrenade(s_SmokeGrenade);
	return 0;
}

static cell_t SmokeThinkFadeNative(IPluginContext *pContext, const cell_t *params)
{
	if (params[0] < 1)
		return 0;

	int s_SmokeIndex = params[1];
	edict_t *s_Edict = gamehelpers->EdictOfIndex(s_SmokeIndex);

	if (!s_Edict || s_Edict->IsFree())
		return 0;

	IServerUnknown *s_Unknown = s_Edict->GetUnknown();

	if (!s_Unknown)
		return 0;

	CBaseEntity *s_SmokeGrenade = (CBaseEntity*)s_Unknown->GetBaseEntity();

	UTIL_SmokeThinkFade(s_SmokeGrenade);
	return 0;
}

static cell_t DetonateFireGrenadeNative(IPluginContext *pContext, const cell_t *params)
{
	if (params[0] < 1)
		return 0;

	int s_FireGrenadeIndex = params[1];
	edict_t *s_Edict = gamehelpers->EdictOfIndex(s_FireGrenadeIndex);

	if (!s_Edict || s_Edict->IsFree())
		return 0;

	IServerUnknown *s_Unknown = s_Edict->GetUnknown();

	if (!s_Unknown)
		return 0;

	CBaseEntity *s_FireGrenade = (CBaseEntity*)s_Unknown->GetBaseEntity();

	UTIL_DetonateMolotovGrenade(s_FireGrenade);
	return 0;
}

static cell_t DetonateBaseGrenadeNative(IPluginContext *pContext, const cell_t *params)
{
	if (params[0] < 1)
		return 0;

	int s_GrenadeIndex = params[1];
	edict_t *s_Edict = gamehelpers->EdictOfIndex(s_GrenadeIndex);

	if (!s_Edict || s_Edict->IsFree())
		return 0;

	IServerUnknown *s_Unknown = s_Edict->GetUnknown();

	if (!s_Unknown)
		return 0;

	CBaseEntity *s_BaseGrenade = (CBaseEntity*)s_Unknown->GetBaseEntity();

	UTIL_DetonateBaseGrenade(s_BaseGrenade);
	return 0;
}

static cell_t DetonateFlashbangNative(IPluginContext *pContext, const cell_t *params)
{
	if (params[0] < 1)
		return 0;

	int s_GrenadeIndex = params[1];
	edict_t *s_Edict = gamehelpers->EdictOfIndex(s_GrenadeIndex);

	if (!s_Edict || s_Edict->IsFree())
		return 0;

	IServerUnknown *s_Unknown = s_Edict->GetUnknown();

	if (!s_Unknown)
		return 0;

	CBaseEntity *s_BaseGrenade = (CBaseEntity*)s_Unknown->GetBaseEntity();

	UTIL_DetonateFlashbang(s_BaseGrenade);
	return 0;
}

static cell_t DetonateHEGrenadeNative(IPluginContext *pContext, const cell_t *params)
{
	if (params[0] < 1)
		return 0;

	int s_GrenadeIndex = params[1];
	edict_t *s_Edict = gamehelpers->EdictOfIndex(s_GrenadeIndex);

	if (!s_Edict || s_Edict->IsFree())
		return 0;

	IServerUnknown *s_Unknown = s_Edict->GetUnknown();

	if (!s_Unknown)
		return 0;

	CBaseEntity *s_BaseGrenade = (CBaseEntity*)s_Unknown->GetBaseEntity();

	UTIL_DetonateHEGrenade(s_BaseGrenade);
	return 0;
}

static cell_t GetCurrentlyDetonatingSmokeGrenadeNative(IPluginContext *pContext, const cell_t *params)
{
	if (g_CurrentlyDetonatingSmokeGrenade == INVALID_ENT_REFERENCE)
		return -1;

	int s_SmokeGrenadeIndex = gamehelpers->ReferenceToIndex(g_CurrentlyDetonatingSmokeGrenade);

	if (s_SmokeGrenadeIndex == INVALID_ENT_REFERENCE)
		return -1;

	return s_SmokeGrenadeIndex;
}

static cell_t GetCurrentlyDetonatingFlashbangNative(IPluginContext *pContext, const cell_t *params)
{
	if (g_CurrentlyDetonatingFlashbang == INVALID_ENT_REFERENCE)
		return -1;

	int s_FlashbangIndex = gamehelpers->ReferenceToIndex(g_CurrentlyDetonatingFlashbang);
	
	if (s_FlashbangIndex == INVALID_ENT_REFERENCE)
		return -1;

	return s_FlashbangIndex;
}

static cell_t GetCurrentlyDetonatingHEGrenadeNative(IPluginContext *pContext, const cell_t *params)
{
	if (g_CurrentlyDetonatingHEGrenade == INVALID_ENT_REFERENCE)
		return -1;

	int s_HEGrenadeIndex = gamehelpers->ReferenceToIndex(g_CurrentlyDetonatingHEGrenade);

	if (s_HEGrenadeIndex == INVALID_ENT_REFERENCE)
		return -1;

	return s_HEGrenadeIndex;
}

static cell_t GetCurrentlyGunfireThinkingDecoyNative(IPluginContext *pContext, const cell_t *params)
{
	if (g_CurrentlyGunfireThinkingDecoy == INVALID_ENT_REFERENCE)
		return -1;

	int s_DecoyIndex = gamehelpers->ReferenceToIndex(g_CurrentlyGunfireThinkingDecoy);

	if (s_DecoyIndex == INVALID_ENT_REFERENCE)
		return -1;

	return s_DecoyIndex;
}

static cell_t GetCurrentlyThinkingInfernoNative(IPluginContext *pContext, const cell_t *params)
{
	if (g_CurrentlyThinkingInferno == INVALID_ENT_REFERENCE)
		return -1;

	int s_InfernoIndex = gamehelpers->ReferenceToIndex(g_CurrentlyThinkingInferno);

	if (s_InfernoIndex == INVALID_ENT_REFERENCE)
		return -1;

	return s_InfernoIndex;
}

static cell_t GetCurrentlyDetonatingMolotovNative(IPluginContext *pContext, const cell_t *params)
{
	if (g_CurrentlyDetonatingMolotov == INVALID_ENT_REFERENCE)
		return -1;

	int s_MolotovIndex = gamehelpers->ReferenceToIndex(g_CurrentlyDetonatingMolotov);

	if (s_MolotovIndex == INVALID_ENT_REFERENCE)
		return -1;

	return s_MolotovIndex;
}

static cell_t GetCurrentlyDetonatingIncendiaryNative(IPluginContext *pContext, const cell_t *params)
{
	if (g_CurrentlyDetonatingIncendiary == INVALID_ENT_REFERENCE)
		return -1;

	int s_IncendiaryIndex = gamehelpers->ReferenceToIndex(g_CurrentlyDetonatingIncendiary);

	if (s_IncendiaryIndex == INVALID_ENT_REFERENCE)
		return -1;

	return s_IncendiaryIndex;
}

void *UTIL_GetItemSchema()
{
	return g_GetItemSchema();
}

unsigned int UTIL_GetItemDefinitionIndex(void *p_ItemDefinition)
{
	if(!p_ItemDefinition)
		return 0;

	void *s_VTable = *((void **)((intptr_t)p_ItemDefinition));
	GetItemDefinitionIndexFn s_Func = (GetItemDefinitionIndexFn)*((void **)((intptr_t)s_VTable + (g_GetItemDefinitionIndexOffset * sizeof(intptr_t))));

	unsigned int s_Result = s_Func(p_ItemDefinition);
	return s_Result;
}

int UTIL_GetLoadoutSlot(void *p_ItemDefinition, int p_Team)
{
	if(!p_ItemDefinition)
		return 0;

	int s_Slot = g_GetLoadoutSlot(p_ItemDefinition, p_Team);
	return s_Slot;
}

void *UTIL_GetItemDefinitionByName(const char *p_ItemName)
{
#ifdef WIN32
	void *s_Temp = UTIL_GetItemSchema();
	void *s_Schema = (void *)((intptr_t)s_Temp + 4);
#else
	void *s_Schema = UTIL_GetItemSchema();
#endif

	if (!s_Schema)
		return NULL;

	void *s_VTable = *((void **)((intptr_t)s_Schema));
	GetItemDefinitionByNameFn s_Func = (GetItemDefinitionByNameFn)*((void **)((intptr_t)s_VTable + (g_GetItemDefinitionByNameOffset * sizeof(intptr_t))));

	CEconItemDefinition *s_Result = s_Func(s_Schema, p_ItemName);

	return (void*)s_Result;
}

CEconItemView *UTIL_GetEconItemView(CBasePlayer *p_Player, const char *p_ItemName)
{
	if(!p_Player)
		return NULL;

	CBaseEntity *s_PlayerBaseEntity = (CBaseEntity*)p_Player;
	IPlayerInfo *s_PlayerInfo = playerhelpers->GetGamePlayer(gamehelpers->EntityToBCompatRef(s_PlayerBaseEntity))->GetPlayerInfo();

	if(!s_PlayerInfo)
		return NULL;

	void *s_ItemDefinition = UTIL_GetItemDefinitionByName(p_ItemName);

	if(!s_ItemDefinition)
		return NULL;
	
	uint16_t s_ItemDefinitionIndex = UTIL_GetItemDefinitionIndex(s_ItemDefinition);

	if(!s_ItemDefinitionIndex)
		return NULL;

	int s_Team = s_PlayerInfo->GetTeamIndex();

	if(s_Team != 2 && s_Team != 3)
		return NULL;

	int s_LoadoutSlot = UTIL_GetLoadoutSlot(s_ItemDefinition, s_Team);

	if (s_LoadoutSlot == -1)
	{
		if(s_Team == 3)
		{
			s_Team = 2;
			s_LoadoutSlot = UTIL_GetLoadoutSlot(s_ItemDefinition, s_Team);
		}
		else
		{
			s_Team = 3;
			s_LoadoutSlot = UTIL_GetLoadoutSlot(s_ItemDefinition, s_Team);
		}
	}

	if(s_LoadoutSlot == -1)
		return NULL;

	void *s_Inventory = (void *)((intptr_t)s_PlayerBaseEntity + g_InventoryOffset);
	void *s_VTable = *((void **)((intptr_t)s_Inventory));
	GetItemInLoadoutFn s_Func = (GetItemInLoadoutFn)*((void **)((intptr_t)s_VTable + (g_GetItemInLoadoutOffset * sizeof(intptr_t))));

	CEconItemView *s_View = s_Func(s_Inventory, s_Team, s_LoadoutSlot);
	return s_View;
}

CBaseEntity *UTIL_GiveNamedItemEcon(CBasePlayer *p_Player, const char *p_ItemName, int p_SubType = 0, bool p_RemoveIfNotCarried = false)
{
	if(!p_Player)
		return NULL;

	CBaseEntity *s_PlayerBaseEntity = (CBaseEntity*)p_Player;
	IPlayerInfo *s_PlayerInfo = playerhelpers->GetGamePlayer(gamehelpers->EntityToBCompatRef(s_PlayerBaseEntity))->GetPlayerInfo();

	if (!s_PlayerInfo)
		return NULL;
	
	CEconItemView *s_EconItemView = UTIL_GetEconItemView(p_Player, p_ItemName);
	CBaseEntity *s_ItemEntity = g_CCSPlayerGiveNamedItem(s_PlayerBaseEntity, p_ItemName, p_SubType, s_EconItemView, p_RemoveIfNotCarried);

	return s_ItemEntity;
}

void UTIL_GameRulesGoToIntermission(void *p_GameRules, float p_PanelTime, bool p_BlockDrops = false)
{
	if (!g_CCSGameRulesGoToIntermission)
		return;

	float *s_Unknown = (float*)((intptr_t)p_GameRules + g_ScoreboardPopupTimeOffset);
	*s_Unknown = p_PanelTime;

	g_CCSGameRulesGoToIntermission(p_GameRules, p_BlockDrops);
}

static cell_t GiveNamedItemEconNative(IPluginContext *pContext, const cell_t *params)
{
	if(params[0] < 4)
		return INVALID_ENT_REFERENCE;

	int s_Client = params[1];
	IGamePlayer *s_GamePlayer = playerhelpers->GetGamePlayer(s_Client);

	if(!s_GamePlayer)
		return INVALID_ENT_REFERENCE;

	IServerUnknown *s_Unknown = s_GamePlayer->GetEdict()->GetUnknown();

	if(!s_Unknown)
		return INVALID_ENT_REFERENCE;

	CBasePlayer *s_BasePlayer = (CBasePlayer*)s_Unknown->GetBaseEntity();

	if(!s_BasePlayer)
		return INVALID_ENT_REFERENCE;

	char *s_ItemName;
	pContext->LocalToString(params[2], &s_ItemName);

	int s_SubType = params[3];
	bool s_RemoveIfNotCarried = params[4] != 0;

	CBaseEntity *s_ItemEntity = UTIL_GiveNamedItemEcon(s_BasePlayer, s_ItemName, s_SubType, s_RemoveIfNotCarried);

	if(!s_ItemEntity)
		return INVALID_ENT_REFERENCE;

	cell_t s_Reference = gamehelpers->EntityToReference(s_ItemEntity);
	return gamehelpers->ReferenceToIndex(s_Reference);
}

static cell_t GameRulesGoToIntermissionNative(IPluginContext *pContext, const cell_t *params)
{
	if (params[0] < 2)
		return 0;

	float s_PanelTime = sp_ctof(params[1]);
	bool s_BlockDrops = params[2] != 0;
	void *s_GameRules = g_pSDKTools->GetGameRules();

	if (!s_GameRules)
		return 0;

	UTIL_GameRulesGoToIntermission(s_GameRules, s_PanelTime, s_BlockDrops);
	return 1;
}

bool FindCSmokeGrenadeProjectileCreate(char *error, int maxlength)
{
	void *s_CSmokeGrenadeProjectileCreateAddress;
	if (!g_pGameConf->GetMemSig("CSmokeGrenadeProjectileCreate", &s_CSmokeGrenadeProjectileCreateAddress) || !s_CSmokeGrenadeProjectileCreateAddress)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate function: CSmokeGrenadeProjectileCreate");

		return false;
	}

	g_CSmokeGrenadeProjectileCreate = (CSmokeGrenadeProjectileCreateFn)s_CSmokeGrenadeProjectileCreateAddress;
	return true;
}

bool FindCFlashbangProjectileCreate(char *error, int maxlength)
{
	void *s_CFlashbangProjectileCreateAddress;
	if (!g_pGameConf->GetMemSig("CFlashbangProjectileCreate", &s_CFlashbangProjectileCreateAddress) || !s_CFlashbangProjectileCreateAddress)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate function: CFlashbangProjectileCreate");

		return false;
	}

	g_CFlashbangProjectileCreate = (CFlashbangProjectileCreateFn)s_CFlashbangProjectileCreateAddress;
	return true;
}

bool FindCMolotovProjectileCreate(char *error, int maxlength)
{
	void *s_CMolotovProjectileCreateAddress;
	if (!g_pGameConf->GetMemSig("CMolotovProjectileCreate", &s_CMolotovProjectileCreateAddress) || !s_CMolotovProjectileCreateAddress)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate function: CMolotovProjectileCreate");

		return false;
	}

	g_CMolotovProjectileCreate = (CMolotovProjectileCreateFn)s_CMolotovProjectileCreateAddress;
	return true;
}

bool FindCDecoyProjectileCreate(char *error, int maxlength)
{
	void *s_CDecoyProjectileCreateAddress;
	if (!g_pGameConf->GetMemSig("CDecoyProjectileCreate", &s_CDecoyProjectileCreateAddress) || !s_CDecoyProjectileCreateAddress)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate function: CDecoyProjectileCreate");

		return false;
	}

	g_CDecoyProjectileCreate = (CDecoyProjectileCreateFn)s_CDecoyProjectileCreateAddress;
	return true;
}

bool FindCHEGrenadeProjectileCreate(char *error, int maxlength)
{
	void *s_CHEGrenadeProjectileCreateAddress;
	if (!g_pGameConf->GetMemSig("CHEGrenadeProjectileCreate", &s_CHEGrenadeProjectileCreateAddress) || !s_CHEGrenadeProjectileCreateAddress)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate function: CHEGrenadeProjectileCreate");

		return false;
	}

	g_CHEGrenadeProjectileCreate = (CHEGrenadeProjectileCreateFn)s_CHEGrenadeProjectileCreateAddress;
	return true;
}

bool FindGetWeaponInfo(char *error, int maxlength)
{
	void *s_GetWeaponInfoAddress;
	if (!g_pGameConf->GetMemSig("GetWeaponInfo", &s_GetWeaponInfoAddress) || !s_GetWeaponInfoAddress)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate function: GetWeaponInfo");

		return false;
	}

	g_GetWeaponInfo = (GetWeaponInfoFn)s_GetWeaponInfoAddress;
	return true;
}

bool FindCSmokeGrenadeProjectileSmokeDetonate(char *error, int maxlength)
{
	void *s_CSmokeGrenadeProjectileSmokeDetonateAddress;
	if (!g_pGameConf->GetMemSig("CSmokeGrenadeProjectileSmokeDetonate", &s_CSmokeGrenadeProjectileSmokeDetonateAddress) || !s_CSmokeGrenadeProjectileSmokeDetonateAddress)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate function: CSmokeGrenadeProjectileSmokeDetonate");

		return false;
	}

	g_CSmokeGrenadeProjectileSmokeDetonate = (CSmokeGrenadeProjectileSmokeDetonateFn)s_CSmokeGrenadeProjectileSmokeDetonateAddress;
	return true;
}

bool FindCSmokeGrenadeProjectileThinkFade(char *error, int maxlength)
{
	void *s_CSmokeGrenadeProjectileThinkFadeAddress;
	if (!g_pGameConf->GetMemSig("CSmokeGrenadeProjectileThinkFade", &s_CSmokeGrenadeProjectileThinkFadeAddress) || !s_CSmokeGrenadeProjectileThinkFadeAddress)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate function: CSmokeGrenadeProjectileThinkFade");

		return false;
	}

	g_CSmokeGrenadeProjectileThinkFade = (CSmokeGrenadeProjectileThinkFadeFn)s_CSmokeGrenadeProjectileThinkFadeAddress;
	return true;
}

bool FindCMolotovProjectileDetonate(char *error, int maxlength)
{
	void *s_CMolotovProjectileDetonateAddress;
	if (!g_pGameConf->GetMemSig("CMolotovProjectileDetonate", &s_CMolotovProjectileDetonateAddress) || !s_CMolotovProjectileDetonateAddress)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate function: CMolotovProjectileDetonate");

		return false;
	}

	g_CMolotovProjectileDetonate = (CMolotovProjectileDetonateFn)s_CMolotovProjectileDetonateAddress;
	return true;
}

bool FindCBaseGrenadeDetonate(char *error, int maxlength)
{
	void *s_CBaseGrenadeDetonateAddress;
	if (!g_pGameConf->GetMemSig("CBaseGrenadeDetonate", &s_CBaseGrenadeDetonateAddress) || !s_CBaseGrenadeDetonateAddress)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate function: CBaseGrenadeDetonate");

		return false;
	}

	g_CBaseGrenadeDetonate = (CBaseGrenadeDetonateFn)s_CBaseGrenadeDetonateAddress;
	return true;
}

bool FindCFlashbangProjectileDetonate(char *error, int maxlength)
{
	void *s_CFlashbangProjectileDetonateAddress;
	if (!g_pGameConf->GetMemSig("CFlashbangProjectileDetonate", &s_CFlashbangProjectileDetonateAddress) || !s_CFlashbangProjectileDetonateAddress)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate function: CFlashbangProjectileDetonate");

		return false;
	}

	g_CFlashbangProjectileDetonate = (CFlashbangProjectileDetonateFn)s_CFlashbangProjectileDetonateAddress;
	return true;
}

bool FindCHEGrenadeProjectileDetonate(char *error, int maxlength)
{
	void *s_CHEGrenadeProjectileDetonateAddress;
	if (!g_pGameConf->GetMemSig("CHEGrenadeProjectileDetonate", &s_CHEGrenadeProjectileDetonateAddress) || !s_CHEGrenadeProjectileDetonateAddress)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate function: CHEGrenadeProjectileDetonate");

		return false;
	}

	g_CHEGrenadeProjectileDetonate = (CHEGrenadeProjectileDetonateFn)s_CHEGrenadeProjectileDetonateAddress;
	return true;
}

bool FindCCSPlayerGiveNamedItem(char *error, int maxlength)
{
	void *s_CCSPlayerGiveNamedItemAddress;
	if (!g_pGameConf->GetMemSig("CCSPlayerGiveNamedItem", &s_CCSPlayerGiveNamedItemAddress) || !s_CCSPlayerGiveNamedItemAddress)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate function: CCSPlayerGiveNamedItem");

		return false;
	}

	g_CCSPlayerGiveNamedItem = (CCSPlayerGiveNamedItemFn)s_CCSPlayerGiveNamedItemAddress;
	return true;
}

bool FindGetItemSchema(char *error, int maxlength)
{
	void *s_GetItemSchemaAddress;
	if (!g_pGameConf->GetMemSig("GetItemSchema", &s_GetItemSchemaAddress) || !s_GetItemSchemaAddress)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate function: GetItemSchema");

		return false;
	}

	g_GetItemSchema = (GetItemSchemaFn)s_GetItemSchemaAddress;
	return true;
}

bool FindGetItemDefinitionByNameOffset(char *error, int maxlength)
{
	int s_GetItemDefinitionByNameOffset = -1;

	if(!g_pGameConf->GetOffset("GetItemDefinitionByName", &s_GetItemDefinitionByNameOffset) || s_GetItemDefinitionByNameOffset == -1)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate offset: GetItemDefinitionByName");

		return false;
	}

	g_GetItemDefinitionByNameOffset = s_GetItemDefinitionByNameOffset;
	return true;
}

bool FindGetItemDefinitionIndexOffset(char *error, int maxlength)
{
	int s_GetItemDefinitionIndexOffset = -1;

	if (!g_pGameConf->GetOffset("GetItemDefinitionIndex", &s_GetItemDefinitionIndexOffset) || s_GetItemDefinitionIndexOffset == -1)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate offset: GetItemDefinitionIndex");

		return false;
	}

	g_GetItemDefinitionIndexOffset = s_GetItemDefinitionIndexOffset;
	return true;
}

bool FindGetLoadoutSlot(char *error, int maxlength)
{
	void *s_GetLoadoutSlotAddress;
	if (!g_pGameConf->GetMemSig("GetLoadoutSlot", &s_GetLoadoutSlotAddress) || !s_GetLoadoutSlotAddress)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate function: GetLoadoutSlot");

		return false;
	}

	g_GetLoadoutSlot = (GetLoadoutSlotFn)s_GetLoadoutSlotAddress;
	return true;
}

bool FindInventoryOffset(char *error, int maxlength)
{
	int s_InventoryOffsetRelative = -1;

	if (!g_pGameConf->GetOffset("InventoryOffsetRelative", &s_InventoryOffsetRelative) || s_InventoryOffsetRelative == -1)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate offset: InventoryOffsetRelative");

		return false;
	}

	void *s_HandleCommandBuyInternalAddress;
	if (!g_pGameConf->GetMemSig("HandleCommand_Buy_Internal", &s_HandleCommandBuyInternalAddress) || !s_HandleCommandBuyInternalAddress)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate function: HandleCommand_Buy_Internal");

		return false;
	}

	g_InventoryOffset = *(int *)((intptr_t)s_HandleCommandBuyInternalAddress + s_InventoryOffsetRelative);
	return true;
}

bool FindGetItemInLoadoutOffset(char *error, int maxlength)
{
	int s_GetItemInLoadoutOffset = -1;

	if (!g_pGameConf->GetOffset("GetItemInLoadout", &s_GetItemInLoadoutOffset) || s_GetItemInLoadoutOffset == -1)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate offset: GetItemInLoadout");

		return false;
	}

	g_GetItemInLoadoutOffset = s_GetItemInLoadoutOffset;
	return true;
}

bool FindCCSGameRulesGoToIntermission(char *error, int maxlength)
{
	void *s_CCSGameRulesGoToIntermissionAddress;
	if (!g_pGameConf->GetMemSig("CCSGameRulesGoToIntermission", &s_CCSGameRulesGoToIntermissionAddress) || !s_CCSGameRulesGoToIntermissionAddress)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate function: CCSGameRulesGoToIntermission");

		return false;
	}

	g_CCSGameRulesGoToIntermission = (CCSGameRulesGoToIntermissionFn)s_CCSGameRulesGoToIntermissionAddress;
	return true;
}

bool FindScoreboardPopupTimeOffset(char *error, int maxlength)
{
	int s_ScoreboardPopupTimeOffset = -1;

	if (!g_pGameConf->GetOffset("ScoreboardPopupTime", &s_ScoreboardPopupTimeOffset) || s_ScoreboardPopupTimeOffset == -1)
	{
		if (error)
			snprintf(error, maxlength, "Could not locate offset: ScoreboardPopupTime");

		return false;
	}

	g_ScoreboardPopupTimeOffset = s_ScoreboardPopupTimeOffset;
	return true;
}

sp_nativeinfo_t g_Freedom35Natives[] = 
{
	{"JEZ_ThrowSmoke",	ThrowSmokeNative},
	{"JEZ_ThrowFlash", ThrowFlashNative},
	{"JEZ_ThrowMolotov", ThrowMolotovNative},
	{"JEZ_ThrowIncendiary", ThrowIncendiaryNative},
	{"JEZ_ThrowDecoy", ThrowDecoyNative},
	{"JEZ_ThrowHEGrenade", ThrowHEGrenadeNative},
	{"JEZ_DetonateSmokeGrenade", DetonateSmokeGrenadeNative},
	{"JEZ_SmokeThinkFade", SmokeThinkFadeNative},
	{"JEZ_DetonateFireGrenade", DetonateFireGrenadeNative},
	{"JEZ_DetonateBaseGrenade", DetonateBaseGrenadeNative},
	{"JEZ_DetonateFlashbang", DetonateFlashbangNative},
	{"JEZ_DetonateHEGrenade", DetonateHEGrenadeNative},
	{"JEZ_GetCurrentlyDetonatingSmokeGrenade", GetCurrentlyDetonatingSmokeGrenadeNative},
	{"JEZ_GetCurrentlyDetonatingFlashbang", GetCurrentlyDetonatingFlashbangNative},
	{"JEZ_GetCurrentlyDetonatingHEGrenade", GetCurrentlyDetonatingHEGrenadeNative},
	{"JEZ_GetCurrentlyGunfireThinkingDecoy", GetCurrentlyGunfireThinkingDecoyNative},
	{"JEZ_GetCurrentlyThinkingInferno", GetCurrentlyThinkingInfernoNative},
	{"JEZ_GetCurrentlyDetonatingMolotov", GetCurrentlyDetonatingMolotovNative},
	{"JEZ_GetCurrentlyDetonatingIncendiary", GetCurrentlyDetonatingIncendiaryNative},
	{"JEZ_GiveNamedItemEcon", GiveNamedItemEconNative},
	{"JEZ_GameRulesGoToIntermission", GameRulesGoToIntermissionNative},
	{NULL,							NULL}
};