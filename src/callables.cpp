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

#include "callables.h"
#include "extension.h"

JEZ_CALLABLES_MEMBER_OFFSET_DEF0(
        GetEyeAngles,
        CBaseEntity,
        const QAngle &);

JEZ_CALLABLES_MEMBER_OFFSET_DEF0(
        TE_GetServerClass,
        void,
        ServerClass *);

JEZ_CALLABLES_MEMBER_OFFSET_DEF1(
        CBasePlayerRemovePlayerItem,
        CBaseEntity,
        bool,
        CBaseEntity *,
        item);

JEZ_CALLABLES_MEMBER_OFFSET_DEF5(
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

JEZ_CALLABLES_MEMBER_SIG_DEF0_VOID(
        CS_RespawnPlayer,
        CBaseEntity);

JEZ_CALLABLES_MEMBER_OFFSET_DEF4_VOID(
        Teleport,
        CBaseEntity,
        const Vector *,
        newPosition,
        const QAngle *,
        newAngles,
        const Vector *,
        newVelocity,
        bool,
        useSlowHighAccuracyContacts);

JEZ_CALLABLES_MEMBER_SIG_DEF0(
        CEconItemViewGetCCSWeaponData,
        void,
        void*);

bool Callables_Init(
        IGameConfig *gameConfig,
        IGameConfig *sdktoolsGameConfig,
        IGameConfig *cstrikeGameConfig,
        char *error,
        size_t maxlength)
{
    JEZ_CALLABLE_MEMBER_OFFSET_CREATE(GetEyeAngles, "EyeAngles", sdktoolsGameConfig);
    JEZ_CALLABLE_MEMBER_OFFSET_CREATE(CBasePlayerRemovePlayerItem, "RemovePlayerItem", sdktoolsGameConfig);
    JEZ_CALLABLE_MEMBER_OFFSET_CREATE(CCSPlayerGiveNamedItem, "GiveNamedItem", sdktoolsGameConfig);
    JEZ_CALLABLE_MEMBER_OFFSET_CREATE(TE_GetServerClass, "TE_GetServerClass", sdktoolsGameConfig);
    JEZ_CALLABLE_MEMBER_SIG_CREATE(CS_RespawnPlayer, "RoundRespawn", cstrikeGameConfig);
    JEZ_CALLABLE_MEMBER_OFFSET_CREATE(Teleport, "Teleport", sdktoolsGameConfig);
    JEZ_CALLABLE_MEMBER_SIG_CREATE(CEconItemViewGetCCSWeaponData, "CEconItemViewGetCCSWeaponData", gameConfig);

    return true;
}

void Callables_Cleanup()
{
}