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
#include "weapon_identifiers.h"
#include "smsdk_ext.h"
#include "cbasecsgrenade_start_grenade_throw_hook.h"

SH_DECL_MANUALHOOK1_void(CCSPlayerWeaponEquip, 0, 0, 0, CBaseEntity*);
int g_CCSPlayerWeaponEquipHookId = 0;

void Hook_Callback_CCSPlayerWeaponEquip(CBaseEntity *weaponEntity)
{
    const char *weaponEntityClassName = gamehelpers->GetEntityClassname(weaponEntity);

    GrenadeType grenadeType = GetGrenadeTypeFromWeaponClassName(weaponEntityClassName);

    if (grenadeType != GrenadeType_UNKNOWN)
        Hooks_MaybeSetupCBaseCSGrenadeStartGrenadeThrowHook(weaponEntity, grenadeType);

    RETURN_META(MRES_IGNORED);
}

void Hooks_MaybeSetupCCSPlayerWeaponEquipHook(CBaseEntity *playerEntity)
{
    if (playerEntity == nullptr)
        return;

    if (g_CCSPlayerWeaponEquipHookId == 0)
    {
        void *playerVtable = *(void **)playerEntity;
        g_CCSPlayerWeaponEquipHookId = SH_ADD_MANUALDVPHOOK(CCSPlayerWeaponEquip, playerVtable, SH_STATIC(Hook_Callback_CCSPlayerWeaponEquip), false);
    }
}

bool Hooks_Init_CCSPlayerWeaponEquipHook(
        IGameConfig *gameConfig,
        char *error,
        size_t maxlength)
{
    int csPlayerWeaponEquip;
    if (!gameConfig->GetOffset("CCSPlayerWeaponEquip", &csPlayerWeaponEquip))
    {
        snprintf(error, maxlength, "Unable to find offset for %s\n", "CCSPlayerWeaponEquip");
        return false;
    }

    SH_MANUALHOOK_RECONFIGURE(CCSPlayerWeaponEquip, csPlayerWeaponEquip, 0, 0);
    return true;
}

void Hooks_Cleanup_CCSPlayerWeaponEquipHook()
{
    if (g_CCSPlayerWeaponEquipHookId != 0)
    {
        SH_REMOVE_HOOK_ID(g_CCSPlayerWeaponEquipHookId);
        g_CCSPlayerWeaponEquipHookId = 0;
    }
}