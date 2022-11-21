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

void Hooks_MaybeSetupCCSPlayerWeaponEquip(CBaseEntity *playerEntity);

#ifdef _WIN32
extern CDetour *g_DetourSmokeProjectileCreate;
extern CDetour *g_DetourFlashbangProjectileCreate;
extern CDetour *g_DetourMolotovProjectileCreate;
extern CDetour *g_DetourDecoyProjectileCreate;
extern CDetour *g_DetourHEGrenadeProjectileCreate;

extern void *Hook_Original_SmokeProjectileCreate;
extern void *Hook_Original_FlashbangProjectileCreate;
extern void *Hook_Original_MolotovProjectileCreate;
extern void *Hook_Original_DecoyProjectileCreate;
extern void *Hook_Original_HEGrenadeProjectileCreate;

CBaseEntity *Hook_Call_SmokeProjectileCreate(const Vector &origin, const QAngle &angle, const Vector &velocity, const Vector &angularImpulse, void* player, int grenadeItemDefinitionIndex);
CBaseEntity *Hook_Call_FlashbangProjectileCreate(const Vector &origin, const QAngle &angle, const Vector &velocity, const Vector &angularImpulse, void* player, int grenadeItemDefinitionIndex);
CBaseEntity *Hook_Call_MolotovProjectileCreate(const Vector &origin, const QAngle &angle, const Vector &velocity, const Vector &angularImpulse, void* player, int grenadeItemDefinitionIndex);
CBaseEntity *Hook_Call_DecoyProjectileCreate(const Vector &origin, const QAngle &angle, const Vector &velocity, const Vector &angularImpulse, void* player, int grenadeItemDefinitionIndex);
CBaseEntity *Hook_Call_HEGrenadeProjectileCreate(const Vector &origin, const QAngle &angle, const Vector &velocity, const Vector &angularImpulse, void* player, int grenadeItemDefinitionIndex);

CBaseEntity* __cdecl Hook_Callback_ProjectileCreate(const Vector &velocity, const Vector &angularImpulse, void* player, int grenadeItemDefinitionIndex);
CBaseEntity* Hook_Call_Internal_ProjectileCreate(void *originalAddress, const Vector &origin, const QAngle &angle, const Vector &velocity, const Vector &angularImpulse, void* player, int grenadeItemDefinitionIndex);
#else
JEZ_HOOK_STATIC_DECL6(
        SmokeProjectileCreate,
        CBaseEntity*,
        __cdecl,
        const Vector&,
        origin,
        const QAngle&,
        angle,
        const Vector&,
        velocity,
        const Vector&,
        angularImpulse,
        void*,
        player,
        int,
        grenadeItemDefinitionIndex);

JEZ_HOOK_STATIC_DECL6(
        FlashbangProjectileCreate,
        CBaseEntity*,
        __cdecl,
        const Vector&,
        origin,
        const QAngle&,
        angle,
        const Vector&,
        velocity,
        const Vector&,
        angularImpulse,
        void*,
        player,
        int,
        grenadeItemDefinitionIndex);

JEZ_HOOK_STATIC_DECL6(
        MolotovProjectileCreate,
        CBaseEntity*,
        __cdecl,
        const Vector&,
        origin,
        const QAngle&,
        angle,
        const Vector&,
        velocity,
        const Vector&,
        angularImpulse,
        void*,
        player,
        int,
        grenadeItemDefinitionIndex);

JEZ_HOOK_STATIC_DECL6(
        DecoyProjectileCreate,
        CBaseEntity*,
        __cdecl,
        const Vector&,
        origin,
        const QAngle&,
        angle,
        const Vector&,
        velocity,
        const Vector&,
        angularImpulse,
        void*,
        player,
        int,
        grenadeItemDefinitionIndex);

JEZ_HOOK_STATIC_DECL6(
        HEGrenadeProjectileCreate,
        CBaseEntity*,
        __cdecl,
        const Vector&,
        origin,
        const QAngle&,
        angle,
        const Vector&,
        velocity,
        const Vector&,
        angularImpulse,
        void*,
        player,
        int,
        grenadeItemDefinitionIndex);

#endif

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
