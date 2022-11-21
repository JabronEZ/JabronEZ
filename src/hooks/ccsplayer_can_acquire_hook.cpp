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

#include "ccsplayer_can_acquire_hook.h"
#include "extension.h"
#include <CDetour/detours.h>
#include "player_manager.h"
#include "player.h"

JEZ_HOOK_MEMBER_DEF3(
        CCSPlayerCanAcquire,
        CBaseEntity,
        int,
        void*,
        econItemView,
        int,
        acquireType,
        void *,
        unk)
{
    auto self = reinterpret_cast<CBaseEntity*>(this);

    auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByBaseEntity(self);
    auto originalResult = Hook_Call_CCSPlayerCanAcquire(self, econItemView, acquireType, unk);

    if (player == nullptr)
        return originalResult;

    return player->OnCanAcquire(econItemView, acquireType, originalResult);
}

bool Hooks_Init_CCSPlayerCanAcquireHook(
        char *error,
        size_t maxlength)
{
    JEZ_HOOK_MEMBER_CREATE(CCSPlayerCanAcquire, "CCSPlayerCanAcquire");
    return true;
}

void Hooks_Cleanup_CCSPlayerCanAcquireHook()
{
    JEZ_HOOK_CLEANUP(CCSPlayerCanAcquire);
}