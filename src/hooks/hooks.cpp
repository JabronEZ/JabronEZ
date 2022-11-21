/**
 * =============================================================================
 * Copyright (C) 2022 JabronEZ Team.  All rights reserved.
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
 */

#include "hooks.h"
#include "cmolotov_projectile_detonate_hook.h"
#include "player_run_cmd_hook.h"
#include "ccsplayer_bump_weapon_hook.h"
#include "ccsplayer_slot_occupied_hook.h"
#include "ccsplayer_weapon_equip_hook.h"
#include "cbasecsgrenade_start_grenade_throw_hook.h"
#include "projectile_create_hooks.h"
#include "ccsplayer_can_acquire_hook.h"
#include <CDetour/detours.h>
#include "extension.h"
#include "player_manager.h"
#include "player.h"
#include "smsdk_ext.h"
#include "entity_utilities.h"
#include "grenade_throw_tickrate.h"

JEZ_HOOK_MEMBER_DEF3_VOID(
        CCSPlayerCSWeaponDrop,
        CBaseEntity,
        CBaseEntity*,
        weapon,
        bool,
        unk1,
        bool,
        unk2)
{
    auto self = reinterpret_cast<CBaseEntity*>(this);

    auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByBaseEntity(self);

    if (player == nullptr)
    {
        Hook_Call_CCSPlayerCSWeaponDrop(self, weapon, unk1, unk2);
        return;
    }

    if (!player->OnDropWeapon(weapon))
        return;

    Hook_Call_CCSPlayerCSWeaponDrop(self, weapon, unk1, unk2);
}

JEZ_HOOK_MEMBER_DEF0_VOID(
        CSmokeGrenadeProjectileDetonate,
        CBaseEntity)
{
    auto projectileEntity = reinterpret_cast<CBaseEntity*>(this);
    Hook_Call_CSmokeGrenadeProjectileDetonate(projectileEntity);

    auto throwerEntity = g_JabronEZ.GetEntityUtilities()->GetProjectileThrower(projectileEntity);

    if (throwerEntity != nullptr)
    {
        auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByBaseEntity(throwerEntity);

        if (player != nullptr && player->GetShortGrenades())
        {
            static unsigned int smokeEffectTickBeginOffset = 0;

            if (smokeEffectTickBeginOffset == 0)
            {
                sm_sendprop_info_t sendpropInfo {};
                gamehelpers->FindSendPropInfo("CSmokeGrenadeProjectile", "m_nSmokeEffectTickBegin", &sendpropInfo);

                if (sendpropInfo.prop == nullptr)
                    return;

                smokeEffectTickBeginOffset = sendpropInfo.actual_offset;
            }

            static unsigned int didSmokeEffectOffset = 0;

            if (didSmokeEffectOffset == 0)
            {
                sm_sendprop_info_t sendpropInfo {};
                gamehelpers->FindSendPropInfo("CSmokeGrenadeProjectile", "m_bDidSmokeEffect", &sendpropInfo);

                if (sendpropInfo.prop == nullptr)
                    return;

                didSmokeEffectOffset = sendpropInfo.actual_offset;
            }

            // Setting these values right after the detonate function will cause the original smoke particle to never appear.
            *(bool*)((uint8_t*)projectileEntity + didSmokeEffectOffset) = false;
            *(uint32_t*)((uint8_t*)projectileEntity + smokeEffectTickBeginOffset) = 0;
        }
    }
}

bool Hooks_Init(
        ISourcePawnEngine *sourcePawnEngine,
        IGameConfig *gameConfig,
        IGameConfig *sdktoolsGameConfig,
        char *error,
        size_t maxlength)
{
    CDetourManager::Init(sourcePawnEngine, gameConfig);

    if (!Hooks_Init_CMolotovProjectileDetonateHook(gameConfig, error, maxlength)
        || !Hooks_Init_PlayerRunCmdHook(sdktoolsGameConfig, error, maxlength)
        || !Hooks_Init_CCSPlayerBumpWeaponHook(gameConfig, error, maxlength)
        || !Hooks_Init_CCSPlayerSlotOccupiedHook(gameConfig, error, maxlength)
        || !Hooks_Init_CCSPlayerWeaponEquipHook(gameConfig, error, maxlength)
        || !Hooks_Init_CBaseCSGrenadeStartGrenadeThrowHook(gameConfig, error, maxlength)
        || !Hooks_Init_ProjectileCreateHooks(error, maxlength)
        || !Hooks_Init_CCSPlayerCanAcquireHook(error, maxlength))
        return false;

    JEZ_HOOK_MEMBER_CREATE(CCSPlayerCSWeaponDrop, "CCSPlayerCSWeaponDrop");
    JEZ_HOOK_MEMBER_CREATE(CSmokeGrenadeProjectileDetonate, "CSmokeGrenadeProjectileDetonate");

    return true;
}

void Hooks_Cleanup()
{
    Hooks_Cleanup_ProjectileCreateHooks();
    Hooks_Cleanup_CCSPlayerCanAcquireHook();
    JEZ_HOOK_CLEANUP(CCSPlayerCSWeaponDrop);
    JEZ_HOOK_CLEANUP(CSmokeGrenadeProjectileDetonate);

    Hooks_Cleanup_CMolotovProjectileDetonateHook();
    Hooks_Cleanup_PlayerRunCmdHook();
    Hooks_Cleanup_CCSPlayerBumpWeaponHook();
    Hooks_Cleanup_CCSPlayerSlotOccupiedHook();
    Hooks_Cleanup_CCSPlayerWeaponEquipHook();
    Hooks_Cleanup_CBaseCSGrenadeStartGrenadeThrowHook();
}