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

#ifndef JABRONEZ_CALLABLES_H
#define JABRONEZ_CALLABLES_H

#include "smsdk_ext.h"
#include <sh_vector.h>
#include "callables_macros.h"

bool Callables_Init(
        IGameConfig *gameConfig,
        IGameConfig *sdktoolsGameConfig,
        IGameConfig *cstrikeGameConfig,
        char *error,
        size_t maxlength);

void Callables_Cleanup();

JEZ_CALLABLES_MEMBER_OFFSET_DECL0(
        GetEyeAngles,
        CBaseEntity,
        const QAngle&);

JEZ_CALLABLES_MEMBER_OFFSET_DECL1(
        CBasePlayerRemovePlayerItem,
        CBaseEntity,
        bool,
        CBaseEntity*,
        item);

JEZ_CALLABLES_MEMBER_OFFSET_DECL0(
        TE_GetServerClass,
        void,
        ServerClass *);

JEZ_CALLABLES_MEMBER_OFFSET_DECL5(
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

JEZ_CALLABLES_MEMBER_SIG_DECL0_VOID(
        CS_RespawnPlayer,
        CBaseEntity);

JEZ_CALLABLES_MEMBER_OFFSET_DECL4_VOID(
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

JEZ_CALLABLES_MEMBER_SIG_DECL0(
        CEconItemViewGetCCSWeaponData,
        void,
        void*);

#endif
