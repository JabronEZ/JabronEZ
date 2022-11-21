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

#include "ccsplayer_slot_occupied_hook.h"
#include "extension.h"
#include "smsdk_ext.h"
#include "player_manager.h"
#include "player.h"

SH_DECL_MANUALHOOK1(CCSPlayerSlotOccupied, 0, 0, 0, bool, CBaseEntity*);
int g_CCSPlayerSlotOccupiedHookId = 0;

bool Hook_Callback_CCSPlayerSlotOccupied(CBaseEntity *weapon)
{
    auto playerEntity = META_IFACEPTR(CBaseEntity);

    auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByBaseEntity(playerEntity);
    auto checkOccupiedResult = player->OnCheckSlotOccupied(weapon);

    if (checkOccupiedResult == CheckSlotOccupiedResult::UseOriginal)
        RETURN_META_VALUE(MRES_IGNORED, false);

    RETURN_META_VALUE(MRES_SUPERCEDE, checkOccupiedResult == CheckSlotOccupiedResult::Occupied);
}

void Hooks_MaybeSetupCCSPlayerSlotOccupiedHook(CBaseEntity *playerEntity)
{
    if (playerEntity == nullptr)
        return;

    if (g_CCSPlayerSlotOccupiedHookId == 0)
    {
        void *playerVtable = *(void **)playerEntity;
        g_CCSPlayerSlotOccupiedHookId = SH_ADD_MANUALDVPHOOK(CCSPlayerSlotOccupied, playerVtable, SH_STATIC(Hook_Callback_CCSPlayerSlotOccupied), false);
    }
}

bool Hooks_Init_CCSPlayerSlotOccupiedHook(
        IGameConfig *gameConfig,
        char *error,
        size_t maxlength)
{
    int csPlayerSlotOccupiedOffset;
    if (!gameConfig->GetOffset("CCSPlayerSlotOccupied", &csPlayerSlotOccupiedOffset))
    {
        snprintf(error, maxlength, "Unable to find offset for %s\n", "CCSPlayerSlotOccupied");
        return false;
    }

    SH_MANUALHOOK_RECONFIGURE(CCSPlayerSlotOccupied, csPlayerSlotOccupiedOffset, 0, 0);
    return true;
}

void Hooks_Cleanup_CCSPlayerSlotOccupiedHook()
{
    if (g_CCSPlayerSlotOccupiedHookId != 0)
    {
        SH_REMOVE_HOOK_ID(g_CCSPlayerSlotOccupiedHookId);
        g_CCSPlayerSlotOccupiedHookId = 0;
    }
}