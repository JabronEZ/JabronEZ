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
#include "smsdk_ext.h"
#include "entity_utilities.h"
#include "grenade_throw_tickrate.h"

SH_DECL_MANUALHOOK0_void(CMolotovProjectileDetonate, 0, 0, 0);
int g_MolotovProjectileDetonateHookId = 0;

SH_DECL_MANUALHOOK2_void(PlayerRunCmd, 0, 0, 0, CUserCmd *, IMoveHelper *);
int g_PlayerRunCmdHookId = 0;

SH_DECL_MANUALHOOK1(CCSPlayerBumpWeapon, 0, 0, 0, bool, CBaseEntity*);
int g_CCSPlayerBumpWeaponHookId = 0;
int g_CCSPlayerBumpWeaponPostHookId = 0;

SH_DECL_MANUALHOOK1(CCSPlayerSlotOccupied, 0, 0, 0, bool, CBaseEntity*);
int g_CCSPlayerSlotOccupiedHookId = 0;

SH_DECL_MANUALHOOK1_void(CCSPlayerWeaponEquip, 0, 0, 0, CBaseEntity*);
int g_CCSPlayerWeaponEquipHookId = 0;

SH_DECL_MANUALHOOK0_void(CBaseCSGrenadeStartGrenadeThrow, 0, 0, 0);
int g_StartGrenadeThrowHookId[GrenadeType_COUNT] = { 0, 0, 0, 0, 0, 0 };

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

void Hook_Callback_CMolotovProjectileDetonate()
{
    auto projectileEntity = META_IFACEPTR(CBaseEntity);
    auto isIncendiary = g_JabronEZ.GetEntityUtilities()->IsIncendiaryGrenade(projectileEntity);
    auto throwerEntity = g_JabronEZ.GetEntityUtilities()->GetProjectileThrower(projectileEntity);

    if (throwerEntity != nullptr)
    {
        auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByBaseEntity(throwerEntity);

        if (player != nullptr)
            player->OnGrenadeDetonationEvent(isIncendiary ? GrenadeType_INCENDIARY : GrenadeType_MOLOTOV, gamehelpers->EntityToReference(projectileEntity));
    }

    RETURN_META(MRES_IGNORED);
}

void MaybeSetupCMolotovProjectileDetonateHook(CBaseEntity *molotovProjectile)
{
    if (molotovProjectile == nullptr)
        return;

    if (g_MolotovProjectileDetonateHookId == 0)
    {
        void *molotovVtable = *(void**)molotovProjectile;
        g_MolotovProjectileDetonateHookId = SH_ADD_MANUALDVPHOOK(CMolotovProjectileDetonate, molotovVtable, SH_STATIC(Hook_Callback_CMolotovProjectileDetonate), false);
    }
}

void Hook_Callback_PlayerRunCmd(CUserCmd *command, IMoveHelper *moveHelper)
{
    auto playerEntity = META_IFACEPTR(CBaseEntity);

    if (playerEntity == nullptr)
        RETURN_META(MRES_IGNORED);

    auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByBaseEntity(playerEntity);

    if (player == nullptr)
        RETURN_META(MRES_IGNORED);

    if (!player->OnRunCmd(command, moveHelper))
        RETURN_META(MRES_SUPERCEDE);

    RETURN_META(MRES_IGNORED);
}

void Hooks_MaybeSetupPlayerRunCmd(CBaseEntity *playerEntity)
{
    if (playerEntity == nullptr)
        return;

    if (g_PlayerRunCmdHookId == 0)
    {
        void *playerVtable = *(void **)playerEntity;
        g_PlayerRunCmdHookId = SH_ADD_MANUALDVPHOOK(PlayerRunCmd, playerVtable, SH_STATIC(Hook_Callback_PlayerRunCmd), false);
    }
}

bool Hook_Callback_CCSPlayerBumpWeapon(CBaseEntity *weapon)
{
    auto playerEntity = META_IFACEPTR(CBaseEntity);

    auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByBaseEntity(playerEntity);
    player->OnBumpWeapon(weapon);

    RETURN_META_VALUE(MRES_IGNORED, false);
}

bool Hook_Callback_CCSPlayerBumpWeapon_Post(CBaseEntity *weapon)
{
    auto playerEntity = META_IFACEPTR(CBaseEntity);

    auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByBaseEntity(playerEntity);
    player->OnBumpWeaponPost(weapon);

    RETURN_META_VALUE(MRES_IGNORED, false);
}

void Hooks_MaybeSetupCCSPlayerBumpWeapon(CBaseEntity *playerEntity)
{
    if (playerEntity == nullptr)
        return;

    void *playerVtable = *(void **)playerEntity;

    if (g_CCSPlayerBumpWeaponHookId == 0)
        g_CCSPlayerBumpWeaponHookId = SH_ADD_MANUALDVPHOOK(CCSPlayerBumpWeapon, playerVtable, SH_STATIC(Hook_Callback_CCSPlayerBumpWeapon), false);

    if (g_CCSPlayerBumpWeaponPostHookId == 0)
        g_CCSPlayerBumpWeaponPostHookId = SH_ADD_MANUALDVPHOOK(CCSPlayerBumpWeapon, playerVtable, SH_STATIC(Hook_Callback_CCSPlayerBumpWeapon_Post), true);
}

bool Hook_Callback_CCSPlayerSlotOccupied(CBaseEntity *weapon)
{
    auto playerEntity = META_IFACEPTR(CBaseEntity);

    auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByBaseEntity(playerEntity);
    auto checkOccupiedResult = player->OnCheckSlotOccupied(weapon);

    if (checkOccupiedResult == CheckSlotOccupiedResult::UseOriginal)
        RETURN_META_VALUE(MRES_IGNORED, false);

    RETURN_META_VALUE(MRES_SUPERCEDE, checkOccupiedResult == CheckSlotOccupiedResult::Occupied);
}

void Hooks_MaybeSetupCCSPlayerSlotOccupied(CBaseEntity *playerEntity)
{
    if (playerEntity == nullptr)
        return;

    if (g_CCSPlayerSlotOccupiedHookId == 0)
    {
        void *playerVtable = *(void **)playerEntity;
        g_CCSPlayerSlotOccupiedHookId = SH_ADD_MANUALDVPHOOK(CCSPlayerSlotOccupied, playerVtable, SH_STATIC(Hook_Callback_CCSPlayerSlotOccupied), false);
    }
}

void Hook_Callback_CBaseCSGrenadeStartGrenadeThrow()
{
    auto weaponEntity = META_IFACEPTR(CBaseEntity);
    auto playerEntity = g_JabronEZ.GetEntityUtilities()->GetWeaponOwner(weaponEntity);

    if (playerEntity == nullptr)
        RETURN_META(MRES_IGNORED);

    auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByBaseEntity(playerEntity);

    if (player == nullptr || player->GetGrenadeThrowTickRate() != GrenadeThrowTickRate_64)
        RETURN_META(MRES_IGNORED);

    static unsigned int throwTimeOffset = 0;

    if (throwTimeOffset == 0)
    {
        sm_sendprop_info_t sendpropInfo {};
        gamehelpers->FindSendPropInfo("CBaseCSGrenade", "m_fThrowTime", &sendpropInfo);

        if (sendpropInfo.prop == nullptr)
            RETURN_META(MRES_IGNORED);

        throwTimeOffset = sendpropInfo.actual_offset;
    }

    float newValue = g_JabronEZ.GetGlobalVars()->curtime + 0.109375f;
    *(float*)((uint8_t *)weaponEntity + throwTimeOffset) = newValue;

    RETURN_META(MRES_IGNORED);
}

void Hooks_MaybeSetupCBaseCSGrenadeStartGrenadeThrow(CBaseEntity *weaponEntity, GrenadeType grenadeType)
{
    if (weaponEntity == nullptr)
        return;

    int grenadeTypeIndex = (int)grenadeType;
    if (g_StartGrenadeThrowHookId[grenadeTypeIndex] == 0)
    {
        void *weaponVtable = *(void **)weaponEntity;
        g_StartGrenadeThrowHookId[grenadeTypeIndex] = SH_ADD_MANUALDVPHOOK(CBaseCSGrenadeStartGrenadeThrow, weaponVtable, SH_STATIC(Hook_Callback_CBaseCSGrenadeStartGrenadeThrow), true);
    }
}

void Hook_Callback_CCSPlayerWeaponEquip(CBaseEntity *weaponEntity)
{
    const char *weaponEntityClassName = gamehelpers->GetEntityClassname(weaponEntity);

    GrenadeType grenadeType = GetGrenadeTypeFromWeaponClassName(weaponEntityClassName);

    if (grenadeType != GrenadeType_UNKNOWN)
        Hooks_MaybeSetupCBaseCSGrenadeStartGrenadeThrow(weaponEntity, grenadeType);

    RETURN_META(MRES_IGNORED);
}

void Hooks_MaybeSetupCCSPlayerWeaponEquip(CBaseEntity *playerEntity)
{
    if (playerEntity == nullptr)
        return;

    if (g_CCSPlayerWeaponEquipHookId == 0)
    {
        void *playerVtable = *(void **)playerEntity;
        g_CCSPlayerWeaponEquipHookId = SH_ADD_MANUALDVPHOOK(CCSPlayerWeaponEquip, playerVtable, SH_STATIC(Hook_Callback_CCSPlayerWeaponEquip), false);
    }
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

    CBaseEntity *molotovProjectile;

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
            molotovProjectile = Hook_Call_MolotovProjectileCreate(originLocal, angleLocal, velocity, angularImpulse, player, grenadeItemDefinitionIndex);
            MaybeSetupCMolotovProjectileDetonateHook(molotovProjectile);
            return molotovProjectile;
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
    auto molotovProjectile = Hook_Call_MolotovProjectileCreate(origin, angle, velocity, angularImpulse, player, grenadeItemDefinitionIndex);

    MaybeSetupCMolotovProjectileDetonateHook(molotovProjectile);

    return molotovProjectile;
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

JEZ_HOOK_MEMBER_DEF3(
        CCSPlayerCanAcquire,
        CBaseEntity,
        int,
        void*,
        econItemView,
        int,
        acquireType,
        void *,
        unk)
{
    auto self = reinterpret_cast<CBaseEntity*>(this);

    auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByBaseEntity(self);
    auto originalResult = Hook_Call_CCSPlayerCanAcquire(self, econItemView, acquireType, unk);

    if (player == nullptr)
        return originalResult;

    return player->OnCanAcquire(econItemView, acquireType, originalResult);
}

JEZ_HOOK_MEMBER_DEF3_VOID(
        CCSPlayerCSWeaponDrop,
        CBaseEntity,
        CBaseEntity*,
        weapon,
        bool,
        unk1,
        bool,
        unk2)
{
    auto self = reinterpret_cast<CBaseEntity*>(this);

    auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByBaseEntity(self);

    if (player == nullptr)
    {
        Hook_Call_CCSPlayerCSWeaponDrop(self, weapon, unk1, unk2);
        return;
    }

    if (!player->OnDropWeapon(weapon))
        return;

    Hook_Call_CCSPlayerCSWeaponDrop(self, weapon, unk1, unk2);
}

JEZ_HOOK_MEMBER_DEF0_VOID(
        CSmokeGrenadeProjectileDetonate,
        CBaseEntity)
{
    auto projectileEntity = reinterpret_cast<CBaseEntity*>(this);
    Hook_Call_CSmokeGrenadeProjectileDetonate(projectileEntity);

    auto throwerEntity = g_JabronEZ.GetEntityUtilities()->GetProjectileThrower(projectileEntity);

    if (throwerEntity != nullptr)
    {
        auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByBaseEntity(throwerEntity);

        if (player != nullptr && player->GetShortGrenades())
        {
            static unsigned int smokeEffectTickBeginOffset = 0;

            if (smokeEffectTickBeginOffset == 0)
            {
                sm_sendprop_info_t sendpropInfo {};
                gamehelpers->FindSendPropInfo("CSmokeGrenadeProjectile", "m_nSmokeEffectTickBegin", &sendpropInfo);

                if (sendpropInfo.prop == nullptr)
                    return;

                smokeEffectTickBeginOffset = sendpropInfo.actual_offset;
            }

            static unsigned int didSmokeEffectOffset = 0;

            if (didSmokeEffectOffset == 0)
            {
                sm_sendprop_info_t sendpropInfo {};
                gamehelpers->FindSendPropInfo("CSmokeGrenadeProjectile", "m_bDidSmokeEffect", &sendpropInfo);

                if (sendpropInfo.prop == nullptr)
                    return;

                didSmokeEffectOffset = sendpropInfo.actual_offset;
            }

            // Setting these values right after the detonate function will cause the original smoke particle to never appear.
            *(bool*)((uint8_t*)projectileEntity + didSmokeEffectOffset) = false;
            *(uint32_t*)((uint8_t*)projectileEntity + smokeEffectTickBeginOffset) = 0;
        }
    }
}

bool Hooks_Init(
        ISourcePawnEngine *sourcePawnEngine,
        IGameConfig *gameConfig,
        IGameConfig *sdktoolsGameConfig,
        char *error,
        size_t maxlength)
{
    CDetourManager::Init(sourcePawnEngine, gameConfig);

    int molotovProjectileDetonateOffset;
    if (!gameConfig->GetOffset("CMolotovProjectileDetonate", &molotovProjectileDetonateOffset))
    {
        snprintf(error, maxlength, "Unable to find offset for %s\n", "CMolotovProjectileDetonate");
        return false;
    }

    SH_MANUALHOOK_RECONFIGURE(CMolotovProjectileDetonate, molotovProjectileDetonateOffset, 0, 0);

    int playerRunCmdOffset;
    if (!sdktoolsGameConfig->GetOffset("PlayerRunCmd", &playerRunCmdOffset))
    {
        snprintf(error, maxlength, "Unable to find offset for %s\n", "PlayerRunCmd");
        return false;
    }

    SH_MANUALHOOK_RECONFIGURE(PlayerRunCmd, playerRunCmdOffset, 0, 0);

    int csPlayerBumpWeaponOffset;
    if (!gameConfig->GetOffset("CCSPlayerBumpWeapon", &csPlayerBumpWeaponOffset))
    {
        snprintf(error, maxlength, "Unable to find offset for %s\n", "CCSPlayerBumpWeapon");
        return false;
    }

    SH_MANUALHOOK_RECONFIGURE(CCSPlayerBumpWeapon, csPlayerBumpWeaponOffset, 0, 0);

    int csPlayerSlotOccupiedOffset;
    if (!gameConfig->GetOffset("CCSPlayerSlotOccupied", &csPlayerSlotOccupiedOffset))
    {
        snprintf(error, maxlength, "Unable to find offset for %s\n", "CCSPlayerSlotOccupied");
        return false;
    }

    SH_MANUALHOOK_RECONFIGURE(CCSPlayerSlotOccupied, csPlayerSlotOccupiedOffset, 0, 0);

    int csPlayerWeaponEquip;
    if (!gameConfig->GetOffset("CCSPlayerWeaponEquip", &csPlayerWeaponEquip))
    {
        snprintf(error, maxlength, "Unable to find offset for %s\n", "CCSPlayerWeaponEquip");
        return false;
    }

    SH_MANUALHOOK_RECONFIGURE(CCSPlayerWeaponEquip, csPlayerWeaponEquip, 0, 0);

    int grenadeStartGrenadeThrowOffset;
    if (!gameConfig->GetOffset("CBaseCSGrenadeStartGrenadeThrow", &grenadeStartGrenadeThrowOffset))
    {
        snprintf(error, maxlength, "Unable to find offset for %s\n", "CBaseCSGrenadeStartGrenadeThrow");
        return false;
    }

    SH_MANUALHOOK_RECONFIGURE(CBaseCSGrenadeStartGrenadeThrow, grenadeStartGrenadeThrowOffset, 0, 0);

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

    JEZ_HOOK_MEMBER_CREATE(CCSPlayerCanAcquire, "CCSPlayerCanAcquire");
    JEZ_HOOK_MEMBER_CREATE(CCSPlayerCSWeaponDrop, "CCSPlayerCSWeaponDrop");
    JEZ_HOOK_MEMBER_CREATE(CSmokeGrenadeProjectileDetonate, "CSmokeGrenadeProjectileDetonate");

    return true;
}

void Hooks_Cleanup()
{
    JEZ_HOOK_CLEANUP(SmokeProjectileCreate);
    JEZ_HOOK_CLEANUP(FlashbangProjectileCreate);
    JEZ_HOOK_CLEANUP(MolotovProjectileCreate);
    JEZ_HOOK_CLEANUP(DecoyProjectileCreate);
    JEZ_HOOK_CLEANUP(HEGrenadeProjectileCreate);
    JEZ_HOOK_CLEANUP(CCSPlayerCanAcquire);
    JEZ_HOOK_CLEANUP(CCSPlayerCSWeaponDrop);
    JEZ_HOOK_CLEANUP(CSmokeGrenadeProjectileDetonate);

    if (g_MolotovProjectileDetonateHookId != 0)
    {
        SH_REMOVE_HOOK_ID(g_MolotovProjectileDetonateHookId);
        g_MolotovProjectileDetonateHookId = 0;
    }

    if (g_PlayerRunCmdHookId != 0)
    {
        SH_REMOVE_HOOK_ID(g_PlayerRunCmdHookId);
        g_PlayerRunCmdHookId = 0;
    }

    if (g_CCSPlayerBumpWeaponHookId != 0)
    {
        SH_REMOVE_HOOK_ID(g_CCSPlayerBumpWeaponHookId);
        g_CCSPlayerBumpWeaponHookId = 0;
    }

    if (g_CCSPlayerBumpWeaponPostHookId != 0)
    {
        SH_REMOVE_HOOK_ID(g_CCSPlayerBumpWeaponPostHookId);
        g_CCSPlayerBumpWeaponPostHookId = 0;
    }

    for (int grenadeTypeIndex = 0; grenadeTypeIndex < GrenadeType_COUNT; grenadeTypeIndex++)
    {
        if (g_StartGrenadeThrowHookId[grenadeTypeIndex] != 0)
        {
            SH_REMOVE_HOOK_ID(g_StartGrenadeThrowHookId[grenadeTypeIndex]);
            g_StartGrenadeThrowHookId[grenadeTypeIndex] = 0;
        }
    }
}