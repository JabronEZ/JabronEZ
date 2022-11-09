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

CBaseEntity *Hook_Call_SmokeProjectileCreate(const Vector &origin, const QAngle &angle, const Vector &velocity, const Vector &angularImpulse, void* player, int grenadeItemDefinitionIndex)
{
    return Hook_Call_Internal_ProjectileCreate(
            Hook_Original_SmokeProjectileCreate,
            origin,
            angle,
            velocity,
            angularImpulse,
            player,
            grenadeItemDefinitionIndex);
}

CBaseEntity *Hook_Call_FlashbangProjectileCreate(const Vector &origin, const QAngle &angle, const Vector &velocity, const Vector &angularImpulse, void* player, int grenadeItemDefinitionIndex)
{
    return Hook_Call_Internal_ProjectileCreate(
            Hook_Original_FlashbangProjectileCreate,
            origin,
            angle,
            velocity,
            angularImpulse,
            player,
            grenadeItemDefinitionIndex);
}

CBaseEntity *Hook_Call_MolotovProjectileCreate(const Vector &origin, const QAngle &angle, const Vector &velocity, const Vector &angularImpulse, void* player, int grenadeItemDefinitionIndex)
{
    return Hook_Call_Internal_ProjectileCreate(
            Hook_Original_MolotovProjectileCreate,
            origin,
            angle,
            velocity,
            angularImpulse,
            player,
            grenadeItemDefinitionIndex);
}

CBaseEntity *Hook_Call_DecoyProjectileCreate(const Vector &origin, const QAngle &angle, const Vector &velocity, const Vector &angularImpulse, void* player, int grenadeItemDefinitionIndex)
{
    return Hook_Call_Internal_ProjectileCreate(
            Hook_Original_DecoyProjectileCreate,
            origin,
            angle,
            velocity,
            angularImpulse,
            player,
            grenadeItemDefinitionIndex);
}

CBaseEntity *Hook_Call_HEGrenadeProjectileCreate(const Vector &origin, const QAngle &angle, const Vector &velocity, const Vector &angularImpulse, void* player, int grenadeItemDefinitionIndex)
{
    return Hook_Call_Internal_ProjectileCreate(
            Hook_Original_HEGrenadeProjectileCreate,
            origin,
            angle,
            velocity,
            angularImpulse,
            player,
            grenadeItemDefinitionIndex);
}

CBaseEntity* Hook_Call_Internal_ProjectileCreate(
        void *originalAddress,
        const Vector &origin,
        const QAngle &angle,
        const Vector &velocity,
        const Vector &angularImpulse,
        void* player,
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
        push player;
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

CBaseEntity* Hook_Callback_FullStackProjectileCreate(
        Vector *origin,
        QAngle *angle,
        const Vector &velocity,
        const Vector &angularImpulse,
        void* player,
        int grenadeItemDefinitionIndex)
{
    Vector originLocal = Vector(origin->x, origin->y, origin->z);
    QAngle angleLocal = QAngle(angle->x, angle->y, angle->z);

    auto grenadeType = GetGrenadeTypeFromItemDefinitionIndex((ItemDefinitionIndex)grenadeItemDefinitionIndex);
    TriggerOnProjectileCreated(player, originLocal, angleLocal, velocity, angularImpulse, grenadeItemDefinitionIndex);

    switch (grenadeType)
    {
        case GrenadeType_FLASH:
            return Hook_Call_FlashbangProjectileCreate(originLocal, angleLocal, velocity, angularImpulse, player, grenadeItemDefinitionIndex);
        case GrenadeType_SMOKE:
            return Hook_Call_SmokeProjectileCreate(originLocal, angleLocal, velocity, angularImpulse, player, grenadeItemDefinitionIndex);
        case GrenadeType_DECOY:
            return Hook_Call_DecoyProjectileCreate(originLocal, angleLocal, velocity, angularImpulse, player, grenadeItemDefinitionIndex);
        case GrenadeType_HEGRENADE:
            return Hook_Call_HEGrenadeProjectileCreate(originLocal, angleLocal, velocity, angularImpulse, player, grenadeItemDefinitionIndex);
        case GrenadeType_MOLOTOV:
        case GrenadeType_INCENDIARY:
            return Hook_Call_MolotovProjectileCreate(originLocal, angleLocal, velocity, angularImpulse, player, grenadeItemDefinitionIndex);
        default:
            return nullptr;
    }
}

CBaseEntity* __cdecl Hook_Callback_ProjectileCreate(const Vector &velocity, const Vector &angularImpulse, void* player, int grenadeItemDefinitionIndex)
{
    // Do not attempt to combine this and the function above, for MSVC will be rude and attempt to mess with ECX and EDX.
    static Vector *origin;
    static QAngle *angle;

    __asm
    {
        mov origin, ecx;
        mov angle, edx;
    }

    return Hook_Callback_FullStackProjectileCreate(
            origin,
            angle,
            velocity,
            angularImpulse,
            player,
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
        void*,
        playerEntity,
        int,
        grenadeItemDefinitionIndex)
{
    TriggerOnProjectileCreated(playerEntity, origin, angle, velocity, angularImpulse, grenadeItemDefinitionIndex);
    return Hook_Call_SmokeProjectileCreate(origin, angle, velocity, angularImpulse, playerEntity, grenadeItemDefinitionIndex);
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
        grenadeItemDefinitionIndex)
{
    TriggerOnProjectileCreated(player, origin, angle, velocity, angularImpulse, grenadeItemDefinitionIndex);
    return Hook_Call_FlashbangProjectileCreate(origin, angle, velocity, angularImpulse, player, grenadeItemDefinitionIndex);
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
        grenadeItemDefinitionIndex)
{
    TriggerOnProjectileCreated(player, origin, angle, velocity, angularImpulse, grenadeItemDefinitionIndex);
    return Hook_Call_MolotovProjectileCreate(origin, angle, velocity, angularImpulse, player, grenadeItemDefinitionIndex);
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
        grenadeItemDefinitionIndex)
{
    TriggerOnProjectileCreated(player, origin, angle, velocity, angularImpulse, grenadeItemDefinitionIndex);
    return Hook_Call_DecoyProjectileCreate(origin, angle, velocity, angularImpulse, player, grenadeItemDefinitionIndex);
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
        grenadeItemDefinitionIndex)
{
    TriggerOnProjectileCreated(player, origin, angle, velocity, angularImpulse, grenadeItemDefinitionIndex);
    return Hook_Call_HEGrenadeProjectileCreate(origin, angle, velocity, angularImpulse, player, grenadeItemDefinitionIndex);
}
#endif

bool Hooks_Init(
        ISourcePawnEngine *sourcePawnEngine,
        IGameConfig *gameConfig,
        char *error,
        size_t maxlength)
{
    CDetourManager::Init(sourcePawnEngine, gameConfig);

#ifdef _WIN32
    JEZ_HOOK_STATIC_CREATE_EX(SmokeProjectileCreate, ProjectileCreate, SmokeProjectileCreate, "CSmokeGrenadeProjectileCreate");
    JEZ_HOOK_STATIC_CREATE_EX(FlashbangProjectileCreate, ProjectileCreate, FlashbangProjectileCreate, "CFlashbangProjectileCreate");
    JEZ_HOOK_STATIC_CREATE_EX(MolotovProjectileCreate, ProjectileCreate, MolotovProjectileCreate, "CMolotovProjectileCreate");
    JEZ_HOOK_STATIC_CREATE_EX(DecoyProjectileCreate, ProjectileCreate, DecoyProjectileCreate, "CDecoyProjectileCreate");
    JEZ_HOOK_STATIC_CREATE_EX(HEGrenadeProjectileCreate, ProjectileCreate, HEGrenadeProjectileCreate,"CHEGrenadeProjectileCreate");
#else
    JEZ_HOOK_STATIC_CREATE(SmokeProjectileCreate, "CSmokeGrenadeProjectileCreate");
    JEZ_HOOK_STATIC_CREATE(FlashbangProjectileCreate, "CFlashbangProjectileCreate");
    JEZ_HOOK_STATIC_CREATE(MolotovProjectileCreate, "CMolotovProjectileCreate");
    JEZ_HOOK_STATIC_CREATE(DecoyProjectileCreate, "CDecoyProjectileCreate");
    JEZ_HOOK_STATIC_CREATE(HEGrenadeProjectileCreate, "CHEGrenadeProjectileCreate");
#endif

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