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

#ifndef JABRONEZ_CCSPLAYER_SLOT_OCCUPIED_HOOK_H
#define JABRONEZ_CCSPLAYER_SLOT_OCCUPIED_HOOK_H

#include "smsdk_ext.h"

enum class CheckSlotOccupiedResult
{
    UseOriginal,
    NotOccupied,
    Occupied
};

void Hooks_MaybeSetupCCSPlayerSlotOccupiedHook(CBaseEntity *playerEntity);
bool Hooks_Init_CCSPlayerSlotOccupiedHook(
        IGameConfig *gameConfig,
        char *error,
        size_t maxlength);
void Hooks_Cleanup_CCSPlayerSlotOccupiedHook();

#endif
