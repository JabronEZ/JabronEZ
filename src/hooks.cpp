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

#include "hooks.h"
#include <CDetour/detours.h>
#include "extension.h"
#include "player_manager.h"
#include "player.h"
#include "weapon_identifiers.h"

void TriggerOnProjectileCreated(
        void *playerEntity,
        const Vector &origin,
        const QAngle &angle,
        const Vector &velocity,
        const Vector &angularImpulse,
        int grenadeItemDefinitionIndex)
{
    auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByBaseEntity(reinterpret_cast<CBaseEntity*>(playerEntity));

    if (player != nullptr)
        player->OnProjectileCreated(origin, angle, velocity, angularImpulse, GetGrenadeTypeFromItemDefinitionIndex((ItemDefinitionIndex)grenadeItemDefinitionIndex));
}

JEZ_HOOK_STATIC_DEF6(
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
        playerEntity,
        int,
        grenadeType)
{
    TriggerOnProjectileCreated(playerEntity, origin, angle, velocity, angularImpulse, grenadeType);
    return Hook_Call_SmokeProjectileCreate(origin, angle, velocity, angularImpulse, playerEntity, grenadeType);
}

JEZ_HOOK_STATIC_DEF6(
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
        grenadeType)
{
    TriggerOnProjectileCreated(player, origin, angle, velocity, angularImpulse, grenadeType);
    return Hook_Call_FlashbangProjectileCreate(origin, angle, velocity, angularImpulse, player, grenadeType);
}

JEZ_HOOK_STATIC_DEF6(
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
        grenadeType)
{
    TriggerOnProjectileCreated(player, origin, angle, velocity, angularImpulse, grenadeType);
    return Hook_Call_MolotovProjectileCreate(origin, angle, velocity, angularImpulse, player, grenadeType);
}

JEZ_HOOK_STATIC_DEF6(
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
        grenadeType)
{
    TriggerOnProjectileCreated(player, origin, angle, velocity, angularImpulse, grenadeType);
    return Hook_Call_DecoyProjectileCreate(origin, angle, velocity, angularImpulse, player, grenadeType);
}

JEZ_HOOK_STATIC_DEF6(
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
        grenadeType)
{
    TriggerOnProjectileCreated(player, origin, angle, velocity, angularImpulse, grenadeType);
    return Hook_Call_HEGrenadeProjectileCreate(origin, angle, velocity, angularImpulse, player, grenadeType);
}

bool Hooks_Init(
        ISourcePawnEngine *sourcePawnEngine,
        IGameConfig *gameConfig,
        char *error,
        size_t maxlength)
{
    CDetourManager::Init(sourcePawnEngine, gameConfig);

    JEZ_HOOK_STATIC_CREATE(SmokeProjectileCreate, "CSmokeGrenadeProjectileCreate");
    JEZ_HOOK_STATIC_CREATE(FlashbangProjectileCreate, "CFlashbangProjectileCreate");
    JEZ_HOOK_STATIC_CREATE(MolotovProjectileCreate, "CMolotovProjectileCreate");
    JEZ_HOOK_STATIC_CREATE(DecoyProjectileCreate, "CDecoyProjectileCreate");
    JEZ_HOOK_STATIC_CREATE(HEGrenadeProjectileCreate, "CHEGrenadeProjectileCreate");

    return true;
}

void Hooks_Cleanup()
{
    JEZ_HOOK_CLEANUP(SmokeProjectileCreate);
    JEZ_HOOK_CLEANUP(FlashbangProjectileCreate);
    JEZ_HOOK_CLEANUP(MolotovProjectileCreate);
    JEZ_HOOK_CLEANUP(DecoyProjectileCreate);
    JEZ_HOOK_CLEANUP(HEGrenadeProjectileCreate);
}