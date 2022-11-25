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

#include "player_run_cmd_hook.h"
#include "extension.h"
#include "smsdk_ext.h"
#include "player_manager.h"
#include "player.h"

SH_DECL_MANUALHOOK2_void(PlayerRunCmd, 0, 0, 0, CUserCmd *, IMoveHelper *);
int g_PlayerRunCmdHookId = 0;

void Hook_Callback_PlayerRunCmd(CUserCmd *command, IMoveHelper *moveHelper)
{
    auto playerEntity = META_IFACEPTR(CBaseEntity);

    if (playerEntity == nullptr)
        RETURN_META(MRES_IGNORED);

    auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByBaseEntity(playerEntity);

    if (player == nullptr || !player->IsValid())
        RETURN_META(MRES_IGNORED);

    if (!player->OnRunCmd(command, moveHelper))
        RETURN_META(MRES_SUPERCEDE);

    RETURN_META(MRES_IGNORED);
}

void Hooks_MaybeSetupPlayerRunCmdHook(CBaseEntity *playerEntity)
{
    if (playerEntity == nullptr)
        return;

    if (g_PlayerRunCmdHookId == 0)
    {
        void *playerVtable = *(void **)playerEntity;
        g_PlayerRunCmdHookId = SH_ADD_MANUALDVPHOOK(PlayerRunCmd, playerVtable, SH_STATIC(Hook_Callback_PlayerRunCmd), false);
    }
}

bool Hooks_Init_PlayerRunCmdHook(
        IGameConfig *sdktoolsGameConfig,
        char *error,
        size_t maxlength)
{
    int playerRunCmdOffset;
    if (!sdktoolsGameConfig->GetOffset("PlayerRunCmd", &playerRunCmdOffset))
    {
        snprintf(error, maxlength, "Unable to find offset for %s\n", "PlayerRunCmd");
        return false;
    }

    SH_MANUALHOOK_RECONFIGURE(PlayerRunCmd, playerRunCmdOffset, 0, 0);
    return true;
}

void Hooks_Cleanup_PlayerRunCmdHook()
{
    if (g_PlayerRunCmdHookId != 0)
    {
        SH_REMOVE_HOOK_ID(g_PlayerRunCmdHookId);
        g_PlayerRunCmdHookId = 0;
    }
}