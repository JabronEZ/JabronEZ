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

#ifndef JABRONEZ_HOOKS_H
#define JABRONEZ_HOOKS_H

#include "smsdk_ext.h"
#include <sh_vector.h>
#include "hooks_macros.h"
#include "weapon_identifiers.h"

class CDetour;

// The reason that all these methods are static is due to the nature of detouring methods.
// Primarily, the callbacks for hooks can not capture additional arguments/context.
// Since we already have this limitation, we can prevent the need for passing around context by just embracing global scope here.
bool Hooks_Init(
        ISourcePawnEngine *sourcePawnEngine,
        IGameConfig *gameConfig,
        IGameConfig *sdktoolsGameConfig,
        char *error,
        size_t maxlength);

void Hooks_Cleanup();

JEZ_HOOK_MEMBER_DECL3(
        CCSPlayerCanAcquire,
        CBaseEntity,
        int,
        void *,
        econItemView,
        int,
        acquireType,
        void *,
        item);

JEZ_HOOK_MEMBER_DECL3_VOID(
        CCSPlayerCSWeaponDrop,
        CBaseEntity,
        CBaseEntity *,
        weapon,
        bool,
        unk1,
        bool,
        unk2);

JEZ_HOOK_MEMBER_DECL0_VOID(
        CSmokeGrenadeProjectileDetonate,
        CBaseEntity);

#endif
