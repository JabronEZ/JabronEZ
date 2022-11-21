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
#include "extension.h"
#include "smsdk_ext.h"
#include "player_manager.h"
#include "player.h"

SH_DECL_MANUALHOOK1(CCSPlayerBumpWeapon, 0, 0, 0, bool, CBaseEntity*);
int g_CCSPlayerBumpWeaponHookId = 0;
int g_CCSPlayerBumpWeaponPostHookId = 0;

bool Hook_Callback_CCSPlayerBumpWeapon(CBaseEntity *weapon)
{
    auto playerEntity = META_IFACEPTR(CBaseEntity);

    auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByBaseEntity(playerEntity);
    player->OnBumpWeapon(weapon);

    RETURN_META_VALUE(MRES_IGNORED, false);
}

bool Hook_Callback_CCSPlayerBumpWeapon_Post(CBaseEntity *weapon)
{
    auto playerEntity = META_IFACEPTR(CBaseEntity);

    auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByBaseEntity(playerEntity);
    player->OnBumpWeaponPost(weapon);

    RETURN_META_VALUE(MRES_IGNORED, false);
}

void Hooks_MaybeSetupCCSPlayerBumpWeapon(CBaseEntity *playerEntity)
{
    if (playerEntity == nullptr)
        return;

    void *playerVtable = *(void **)playerEntity;

    if (g_CCSPlayerBumpWeaponHookId == 0)
        g_CCSPlayerBumpWeaponHookId = SH_ADD_MANUALDVPHOOK(CCSPlayerBumpWeapon, playerVtable, SH_STATIC(Hook_Callback_CCSPlayerBumpWeapon), false);

    if (g_CCSPlayerBumpWeaponPostHookId == 0)
        g_CCSPlayerBumpWeaponPostHookId = SH_ADD_MANUALDVPHOOK(CCSPlayerBumpWeapon, playerVtable, SH_STATIC(Hook_Callback_CCSPlayerBumpWeapon_Post), true);
}

bool Hooks_Init_CCSPlayerBumpWeaponHook(
        IGameConfig *gameConfig,
        char *error,
        size_t maxlength)
{
    int csPlayerBumpWeaponOffset;
    if (!gameConfig->GetOffset("CCSPlayerBumpWeapon", &csPlayerBumpWeaponOffset))
    {
        snprintf(error, maxlength, "Unable to find offset for %s\n", "CCSPlayerBumpWeapon");
        return false;
    }

    SH_MANUALHOOK_RECONFIGURE(CCSPlayerBumpWeapon, csPlayerBumpWeaponOffset, 0, 0);
    return true;
}

void Hooks_Cleanup_CCSPlayerBumpWeaponHook()
{
    if (g_CCSPlayerBumpWeaponHookId != 0)
    {
        SH_REMOVE_HOOK_ID(g_CCSPlayerBumpWeaponHookId);
        g_CCSPlayerBumpWeaponHookId = 0;
    }

    if (g_CCSPlayerBumpWeaponPostHookId != 0)
    {
        SH_REMOVE_HOOK_ID(g_CCSPlayerBumpWeaponPostHookId);
        g_CCSPlayerBumpWeaponPostHookId = 0;
    }
}