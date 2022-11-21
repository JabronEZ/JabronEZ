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
#include "ccsplayer_cs_weapon_drop_hook.h"
#include "csmokegrenade_projectile_detonate_hook.h"
#include <CDetour/detours.h>
#include "smsdk_ext.h"

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
        || !Hooks_Init_CCSPlayerCanAcquireHook(error, maxlength)
        || !Hooks_Init_CCSPlayerCSWeaponDropHook(error, maxlength)
        || !Hooks_Init_CSmokeGrenadeProjectileDetonateHook(error, maxlength))
        return false;

    return true;
}

void Hooks_Cleanup()
{
    Hooks_Cleanup_ProjectileCreateHooks();
    Hooks_Cleanup_CCSPlayerCanAcquireHook();
    Hooks_Cleanup_CCSPlayerCSWeaponDropHook();
    Hooks_Cleanup_CSmokeGrenadeProjectileDetonateHook();
    Hooks_Cleanup_CMolotovProjectileDetonateHook();
    Hooks_Cleanup_PlayerRunCmdHook();
    Hooks_Cleanup_CCSPlayerBumpWeaponHook();
    Hooks_Cleanup_CCSPlayerSlotOccupiedHook();
    Hooks_Cleanup_CCSPlayerWeaponEquipHook();
    Hooks_Cleanup_CBaseCSGrenadeStartGrenadeThrowHook();
}