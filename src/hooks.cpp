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
        player,
        int,
        grenadeType)
{
    META_CONPRINTF(
            "Smoke thrown: [%f %f %f] [%f %f %f] [%f %f %f] [%f %f %f] %p %d\n",
            origin.x,
            origin.y,
            origin.z,
            angle.x,
            angle.y,
            angle.z,
            velocity.x,
            velocity.y,
            velocity.z,
            angularImpulse.x,
            angularImpulse.y,
            angularImpulse.z,
            player,
            grenadeType);

    lastOrigin = origin;
    lastAngle = angle;
    lastVelocity = velocity;
    lastAngularImpulse = angularImpulse;

    return Hook_Call_SmokeProjectileCreate(origin, angle, velocity, angularImpulse, player, grenadeType);
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
    META_CONPRINTF(
            "Flashbang thrown: [%f %f %f] [%f %f %f] [%f %f %f] [%f %f %f] %p %d\n",
            origin.x,
            origin.y,
            origin.z,
            angle.x,
            angle.y,
            angle.z,
            velocity.x,
            velocity.y,
            velocity.z,
            angularImpulse.x,
            angularImpulse.y,
            angularImpulse.z,
            player,
            grenadeType);

    lastOrigin = origin;
    lastAngle = angle;
    lastVelocity = velocity;
    lastAngularImpulse = angularImpulse;

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
    META_CONPRINTF(
            "Molotov thrown: [%f %f %f] [%f %f %f] [%f %f %f] [%f %f %f] %p %d\n",
            origin.x,
            origin.y,
            origin.z,
            angle.x,
            angle.y,
            angle.z,
            velocity.x,
            velocity.y,
            velocity.z,
            angularImpulse.x,
            angularImpulse.y,
            angularImpulse.z,
            player,
            grenadeType);

    lastOrigin = origin;
    lastAngle = angle;
    lastVelocity = velocity;
    lastAngularImpulse = angularImpulse;

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
    META_CONPRINTF(
            "Decoy thrown: [%f %f %f] [%f %f %f] [%f %f %f] [%f %f %f] %p %d\n",
            origin.x,
            origin.y,
            origin.z,
            angle.x,
            angle.y,
            angle.z,
            velocity.x,
            velocity.y,
            velocity.z,
            angularImpulse.x,
            angularImpulse.y,
            angularImpulse.z,
            player,
            grenadeType);

    lastOrigin = origin;
    lastAngle = angle;
    lastVelocity = velocity;
    lastAngularImpulse = angularImpulse;

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
    META_CONPRINTF(
            "HE grenade thrown: [%f %f %f] [%f %f %f] [%f %f %f] [%f %f %f] %p %d\n",
            origin.x,
            origin.y,
            origin.z,
            angle.x,
            angle.y,
            angle.z,
            velocity.x,
            velocity.y,
            velocity.z,
            angularImpulse.x,
            angularImpulse.y,
            angularImpulse.z,
            player,
            grenadeType);

    lastOrigin = origin;
    lastAngle = angle;
    lastVelocity = velocity;
    lastAngularImpulse = angularImpulse;

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