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

#include "cbasecsgrenade_start_grenade_throw_hook.h"
#include "extension.h"
#include "entity_utilities.h"
#include "player_manager.h"
#include "player.h"

SH_DECL_MANUALHOOK0_void(CBaseCSGrenadeStartGrenadeThrow, 0, 0, 0);
int g_StartGrenadeThrowHookId[GrenadeType_COUNT] = { 0, 0, 0, 0, 0, 0 };

void Hook_Callback_CBaseCSGrenadeStartGrenadeThrow()
{
    auto weaponEntity = META_IFACEPTR(CBaseEntity);
    auto playerEntity = g_JabronEZ.GetEntityUtilities()->GetWeaponOwner(weaponEntity);

    if (playerEntity == nullptr)
        RETURN_META(MRES_IGNORED);

    auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByBaseEntity(playerEntity);

    if (player == nullptr || player->GetGrenadeThrowTickRate() != GrenadeThrowTickRate_64)
        RETURN_META(MRES_IGNORED);

    static unsigned int throwTimeOffset = 0;

    if (throwTimeOffset == 0)
    {
        sm_sendprop_info_t sendpropInfo {};
        gamehelpers->FindSendPropInfo("CBaseCSGrenade", "m_fThrowTime", &sendpropInfo);

        if (sendpropInfo.prop == nullptr)
            RETURN_META(MRES_IGNORED);

        throwTimeOffset = sendpropInfo.actual_offset;
    }

    float newValue = g_JabronEZ.GetGlobalVars()->curtime + 0.109375f;
    *(float*)((uint8_t *)weaponEntity + throwTimeOffset) = newValue;

    RETURN_META(MRES_IGNORED);
}

void Hooks_MaybeSetupCBaseCSGrenadeStartGrenadeThrow(CBaseEntity *weaponEntity, GrenadeType grenadeType)
{
    if (weaponEntity == nullptr)
        return;

    int grenadeTypeIndex = (int)grenadeType;
    if (g_StartGrenadeThrowHookId[grenadeTypeIndex] == 0)
    {
        void *weaponVtable = *(void **)weaponEntity;
        g_StartGrenadeThrowHookId[grenadeTypeIndex] = SH_ADD_MANUALDVPHOOK(CBaseCSGrenadeStartGrenadeThrow, weaponVtable, SH_STATIC(Hook_Callback_CBaseCSGrenadeStartGrenadeThrow), true);
    }
}

bool Hooks_Init_CBaseCSGrenadeStartGrenadeThrowHook(
        IGameConfig *gameConfig,
        char *error,
        size_t maxlength)
{
    int grenadeStartGrenadeThrowOffset;
    if (!gameConfig->GetOffset("CBaseCSGrenadeStartGrenadeThrow", &grenadeStartGrenadeThrowOffset))
    {
        snprintf(error, maxlength, "Unable to find offset for %s\n", "CBaseCSGrenadeStartGrenadeThrow");
        return false;
    }

    SH_MANUALHOOK_RECONFIGURE(CBaseCSGrenadeStartGrenadeThrow, grenadeStartGrenadeThrowOffset, 0, 0);
    return true;
}

void Hooks_Cleanup_CBaseCSGrenadeStartGrenadeThrowHook()
{
    for (int grenadeTypeIndex = 0; grenadeTypeIndex < GrenadeType_COUNT; grenadeTypeIndex++)
    {
        if (g_StartGrenadeThrowHookId[grenadeTypeIndex] != 0)
        {
            SH_REMOVE_HOOK_ID(g_StartGrenadeThrowHookId[grenadeTypeIndex]);
            g_StartGrenadeThrowHookId[grenadeTypeIndex] = 0;
        }
    }
}