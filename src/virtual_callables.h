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

#ifndef JABRONEZ_VIRTUAL_CALLABLES_H
#define JABRONEZ_VIRTUAL_CALLABLES_H

#include "smsdk_ext.h"
#include <sh_vector.h>
#include "virtual_callables_macros.h"

bool Virtual_Callables_Init(
        IGameConfig *gameConfig,
        IGameConfig *sdktoolsGameConfig,
        char *error,
        size_t maxlength);

void Virtual_Callables_Cleanup();

JEZ_VIRTUAL_CALLABLES_DECL0(
        GetEyeAngles,
        CBaseEntity,
        const QAngle&);

JEZ_VIRTUAL_CALLABLES_DECL1(
        CBasePlayerRemovePlayerItem,
        CBaseEntity,
        bool,
        CBaseEntity*,
        item);

JEZ_VIRTUAL_CALLABLES_DECL5(
        CCSPlayerGiveNamedItem,
        CBaseEntity,
        CBaseEntity*,
        const char *,
        entityName,
        int,
        subType,
        void *,
        econ,
        bool,
        unk,
        Vector*,
        vec);

#endif
