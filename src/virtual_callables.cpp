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

#include "virtual_callables.h"
#include "extension.h"

JEZ_VIRTUAL_CALLABLES_DEF0(
        GetEyeAngles,
        CBaseEntity,
        const QAngle &);

JEZ_VIRTUAL_CALLABLES_DEF1(
        CBasePlayerRemovePlayerItem,
        CBaseEntity,
        bool,
        CBaseEntity *,
        item);

JEZ_VIRTUAL_CALLABLES_DEF5(
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

bool Virtual_Callables_Init(
        IGameConfig *gameConfig,
        IGameConfig *sdktoolsGameConfig,
        char *error,
        size_t maxlength)
{
    JEZ_VIRTUAL_CALLABLE_CREATE(GetEyeAngles, "EyeAngles", sdktoolsGameConfig);
    JEZ_VIRTUAL_CALLABLE_CREATE(CBasePlayerRemovePlayerItem, "RemovePlayerItem", sdktoolsGameConfig);
    JEZ_VIRTUAL_CALLABLE_CREATE(CCSPlayerGiveNamedItem, "GiveNamedItem", sdktoolsGameConfig);

    return true;
}

void Virtual_Callables_Cleanup()
{
}