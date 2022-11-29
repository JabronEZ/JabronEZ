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

#include "projectile_create_hooks.h"
#include "extension.h"
#include "player_manager.h"
#include "player.h"
#include "weapon_identifiers.h"
#include "cmolotov_projectile_detonate_hook.h"
#include <CDetour/detours.h>

CBaseEntity *Hook_Callback_ProjectileCreate(
        const Vector &origin,
        const QAngle &angle,
        const Vector &velocity,
        const Vector &angularImpulse,
        CBaseEntity *playerEntity,
        int grenadeItemDefinitionIndex)
{
    auto grenadeType = GetGrenadeTypeFromItemDefinitionIndex((ItemDefinitionIndex)grenadeItemDefinitionIndex);
    auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByBaseEntity(playerEntity);
    auto validPlayer = player != nullptr && player->IsValid();

    if (validPlayer)
        player->OnProjectileCreate(origin, angle, velocity, angularImpulse, grenadeType);

    CBaseEntity *projectileEntity = nullptr;

    switch (grenadeType)
    {
        case GrenadeType_FLASH:
            projectileEntity = Hook_Call_FlashbangProjectileCreate(origin, angle, velocity, angularImpulse, playerEntity, grenadeItemDefinitionIndex);
            break;
        case GrenadeType_SMOKE:
            projectileEntity = Hook_Call_SmokeProjectileCreate(origin, angle, velocity, angularImpulse, playerEntity, grenadeItemDefinitionIndex);
            break;
        case GrenadeType_DECOY:
            projectileEntity = Hook_Call_DecoyProjectileCreate(origin, angle, velocity, angularImpulse, playerEntity, grenadeItemDefinitionIndex);
            break;
        case GrenadeType_HEGRENADE:
            projectileEntity = Hook_Call_HEGrenadeProjectileCreate(origin, angle, velocity, angularImpulse, playerEntity, grenadeItemDefinitionIndex);
            break;
        case GrenadeType_MOLOTOV:
        case GrenadeType_INCENDIARY:
            projectileEntity = Hook_Call_MolotovProjectileCreate(origin, angle, velocity, angularImpulse, playerEntity, grenadeItemDefinitionIndex);
            Hooks_MaybeSetupCMolotovProjectileDetonateHook(projectileEntity);
            break;
        default:
            break;
    }

    if (validPlayer && projectileEntity != nullptr)
        player->OnProjectileCreated(origin, angle, velocity, angularImpulse, grenadeType, projectileEntity);

    return projectileEntity;
}

#ifdef _WIN32
CDetour *g_DetourSmokeProjectileCreate = nullptr;
CDetour *g_DetourFlashbangProjectileCreate = nullptr;
CDetour *g_DetourMolotovProjectileCreate = nullptr;
CDetour *g_DetourDecoyProjectileCreate = nullptr;
CDetour *g_DetourHEGrenadeProjectileCreate = nullptr;

void *Hook_Original_SmokeProjectileCreate = nullptr;
void *Hook_Original_FlashbangProjectileCreate = nullptr;
void *Hook_Original_MolotovProjectileCreate = nullptr;
void *Hook_Original_DecoyProjectileCreate = nullptr;
void *Hook_Original_HEGrenadeProjectileCreate = nullptr;

CBaseEntity *Hook_Call_SmokeProjectileCreate(const Vector &origin, const QAngle &angle, const Vector &velocity, const Vector &angularImpulse, CBaseEntity* playerEntity, int grenadeItemDefinitionIndex)
{
    return Hook_Call_Internal_ProjectileCreate(
            Hook_Original_SmokeProjectileCreate,
            origin,
            angle,
            velocity,
            angularImpulse,
            playerEntity,
            grenadeItemDefinitionIndex);
}

CBaseEntity *Hook_Call_FlashbangProjectileCreate(const Vector &origin, const QAngle &angle, const Vector &velocity, const Vector &angularImpulse, CBaseEntity* playerEntity, int grenadeItemDefinitionIndex)
{
    return Hook_Call_Internal_ProjectileCreate(
            Hook_Original_FlashbangProjectileCreate,
            origin,
            angle,
            velocity,
            angularImpulse,
            playerEntity,
            grenadeItemDefinitionIndex);
}

CBaseEntity *Hook_Call_MolotovProjectileCreate(const Vector &origin, const QAngle &angle, const Vector &velocity, const Vector &angularImpulse, CBaseEntity *playerEntity, int grenadeItemDefinitionIndex)
{
    return Hook_Call_Internal_ProjectileCreate(
            Hook_Original_MolotovProjectileCreate,
            origin,
            angle,
            velocity,
            angularImpulse,
            playerEntity,
            grenadeItemDefinitionIndex);
}

CBaseEntity *Hook_Call_DecoyProjectileCreate(const Vector &origin, const QAngle &angle, const Vector &velocity, const Vector &angularImpulse, CBaseEntity *playerEntity, int grenadeItemDefinitionIndex)
{
    return Hook_Call_Internal_ProjectileCreate(
            Hook_Original_DecoyProjectileCreate,
            origin,
            angle,
            velocity,
            angularImpulse,
            playerEntity,
            grenadeItemDefinitionIndex);
}

CBaseEntity *Hook_Call_HEGrenadeProjectileCreate(const Vector &origin, const QAngle &angle, const Vector &velocity, const Vector &angularImpulse, CBaseEntity *playerEntity, int grenadeItemDefinitionIndex)
{
    return Hook_Call_Internal_ProjectileCreate(
            Hook_Original_HEGrenadeProjectileCreate,
            origin,
            angle,
            velocity,
            angularImpulse,
            playerEntity,
            grenadeItemDefinitionIndex);
}

CBaseEntity* Hook_Call_Internal_ProjectileCreate(
        void *originalAddress,
        const Vector &origin,
        const QAngle &angle,
        const Vector &velocity,
        const Vector &angularImpulse,
        CBaseEntity *playerEntity,
        int grenadeItemDefinitionIndex)
{
    static void *oldECX = nullptr;
    static void *oldEDX = nullptr;
    static void *returnValue = nullptr;

    __asm
    {
        mov oldECX, ecx;
        mov oldEDX, edx;
        mov ecx, origin;
        mov edx, angle;
        push grenadeItemDefinitionIndex;
        push playerEntity;
        push angularImpulse;
        push velocity;
        call originalAddress;
        mov returnValue, eax;
        add esp, 16
        mov ecx, oldECX;
        mov edx, oldEDX;
    }

    return reinterpret_cast<CBaseEntity*>(returnValue);
}

CBaseEntity* Hook_Callback_Win32ProjectileCreateInner(
        Vector *origin,
        QAngle *angle,
        const Vector &velocity,
        const Vector &angularImpulse,
        CBaseEntity *playerEntity,
        int grenadeItemDefinitionIndex)
{
    Vector originLocal = Vector(origin->x, origin->y, origin->z);
    QAngle angleLocal = QAngle(angle->x, angle->y, angle->z);

    return Hook_Callback_ProjectileCreate(originLocal, angleLocal, velocity, angularImpulse, playerEntity, grenadeItemDefinitionIndex);
}

CBaseEntity* __cdecl Hook_Callback_Win32ProjectileCreate(const Vector &velocity, const Vector &angularImpulse, CBaseEntity *playerEntity, int grenadeItemDefinitionIndex)
{
    // Do not attempt to combine this and the function above, for MSVC will be rude and attempt to mess with ECX and EDX.
    static Vector *origin;
    static QAngle *angle;

    __asm
    {
        mov origin, ecx;
        mov angle, edx;
    }

    return Hook_Callback_Win32ProjectileCreateInner(
            origin,
            angle,
            velocity,
            angularImpulse,
            playerEntity,
            grenadeItemDefinitionIndex);
}

#else
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
        CBaseEntity*,
        playerEntity,
        int,
        grenadeItemDefinitionIndex)
{
    return Hook_Callback_ProjectileCreate(origin, angle, velocity, angularImpulse, playerEntity, grenadeItemDefinitionIndex);
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
        CBaseEntity*,
        playerEntity,
        int,
        grenadeItemDefinitionIndex)
{
    return Hook_Callback_ProjectileCreate(origin, angle, velocity, angularImpulse, playerEntity, grenadeItemDefinitionIndex);
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
        CBaseEntity*,
        playerEntity,
        int,
        grenadeItemDefinitionIndex)
{
    return Hook_Callback_ProjectileCreate(origin, angle, velocity, angularImpulse, playerEntity, grenadeItemDefinitionIndex);
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
        CBaseEntity*,
        playerEntity,
        int,
        grenadeItemDefinitionIndex)
{
    return Hook_Callback_ProjectileCreate(origin, angle, velocity, angularImpulse, playerEntity, grenadeItemDefinitionIndex);
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
        CBaseEntity*,
        playerEntity,
        int,
        grenadeItemDefinitionIndex)
{
    return Hook_Callback_ProjectileCreate(origin, angle, velocity, angularImpulse, playerEntity, grenadeItemDefinitionIndex);
}
#endif

bool Hooks_Init_ProjectileCreateHooks(
        char *error,
        size_t maxlength)
{
#ifdef _WIN32
    JEZ_HOOK_STATIC_CREATE_EX(SmokeProjectileCreate, Win32ProjectileCreate, SmokeProjectileCreate, "CSmokeGrenadeProjectileCreate");
    JEZ_HOOK_STATIC_CREATE_EX(FlashbangProjectileCreate, Win32ProjectileCreate, FlashbangProjectileCreate, "CFlashbangProjectileCreate");
    JEZ_HOOK_STATIC_CREATE_EX(MolotovProjectileCreate, Win32ProjectileCreate, MolotovProjectileCreate, "CMolotovProjectileCreate");
    JEZ_HOOK_STATIC_CREATE_EX(DecoyProjectileCreate, Win32ProjectileCreate, DecoyProjectileCreate, "CDecoyProjectileCreate");
    JEZ_HOOK_STATIC_CREATE_EX(HEGrenadeProjectileCreate, Win32ProjectileCreate, HEGrenadeProjectileCreate,"CHEGrenadeProjectileCreate");
#else
    JEZ_HOOK_STATIC_CREATE(SmokeProjectileCreate, "CSmokeGrenadeProjectileCreate");
    JEZ_HOOK_STATIC_CREATE(FlashbangProjectileCreate, "CFlashbangProjectileCreate");
    JEZ_HOOK_STATIC_CREATE(MolotovProjectileCreate, "CMolotovProjectileCreate");
    JEZ_HOOK_STATIC_CREATE(DecoyProjectileCreate, "CDecoyProjectileCreate");
    JEZ_HOOK_STATIC_CREATE(HEGrenadeProjectileCreate, "CHEGrenadeProjectileCreate");
#endif

    return true;
}

void Hooks_Cleanup_ProjectileCreateHooks()
{
    JEZ_HOOK_CLEANUP(SmokeProjectileCreate);
    JEZ_HOOK_CLEANUP(FlashbangProjectileCreate);
    JEZ_HOOK_CLEANUP(MolotovProjectileCreate);
    JEZ_HOOK_CLEANUP(DecoyProjectileCreate);
    JEZ_HOOK_CLEANUP(HEGrenadeProjectileCreate);
}