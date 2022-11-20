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

#include "entity_utilities.h"
#include "smsdk_ext.h"
#include "callables.h"

EntityUtilities::EntityUtilities(
        IGameHelpers *gameHelpers,
        IPlayerManager *smPlayerManager)
{
    _gameHelpers = gameHelpers;
    _smPlayerManager = smPlayerManager;
}

EntityUtilities::~EntityUtilities()
{
    _gameHelpers = nullptr;
}

bool EntityUtilities::IsIncendiaryGrenade(CBaseEntity *entity) const
{
    if (entity == nullptr)
        return false;

    static unsigned int isIncGrenadeOffset = 0;

    if (isIncGrenadeOffset == 0)
    {
        sm_sendprop_info_t sendpropInfo {};
        _gameHelpers->FindSendPropInfo("CMolotovProjectile", "m_bIsIncGrenade", &sendpropInfo);

        if (sendpropInfo.prop == nullptr)
            return false;

        isIncGrenadeOffset = sendpropInfo.actual_offset;
    }

    uint8_t isIncendiary = *((uint8_t *)entity + isIncGrenadeOffset);
    return isIncendiary != 0;
}

CBaseEntity *EntityUtilities::GetEntityFromHandle(CBaseHandle *handle) const
{
    if (handle == nullptr)
        return nullptr;

    return _gameHelpers->ReferenceToEntity(handle->GetEntryIndex());
}

CBaseEntity *EntityUtilities::GetProjectileThrower(CBaseEntity *entity) const
{
    if (entity == nullptr)
        return nullptr;

    static unsigned int throwerOffset = 0;
    static unsigned int ownerOffset = 0;

    if (throwerOffset == 0)
    {
        sm_sendprop_info_t throwerSendpropInfo {};
        _gameHelpers->FindSendPropInfo("CBaseCSGrenadeProjectile", "m_hThrower", &throwerSendpropInfo);

        if (throwerSendpropInfo.prop != nullptr)
            throwerOffset = throwerSendpropInfo.actual_offset;
    }

    if (ownerOffset == 0)
    {
        sm_sendprop_info_t ownerSendpropInfo {};
        _gameHelpers->FindSendPropInfo("CBaseEntity", "m_hOwnerEntity", &ownerSendpropInfo);

        if (ownerSendpropInfo.prop != nullptr)
            ownerOffset = ownerSendpropInfo.actual_offset;
    }

    if (throwerOffset != 0)
    {
        auto *throwerHandle = (CBaseHandle *)((uint8_t *)entity + throwerOffset);

        if (throwerHandle != nullptr)
        {
            auto throwerEntity = GetEntityFromHandle(throwerHandle);

            if (throwerEntity != nullptr)
                return throwerEntity;
        }
    }

    if (ownerOffset != 0)
    {
        auto *ownerHandle = (CBaseHandle *)((uint8_t *)entity + ownerOffset);

        if (ownerHandle != nullptr)
        {
            auto ownerEntity = GetEntityFromHandle(ownerHandle);

            if (ownerEntity != nullptr)
                return ownerEntity;
        }
    }

    return nullptr;
}

CBaseEntity *EntityUtilities::GetWeaponOwner(CBaseEntity *entity) const
{
    if (entity == nullptr)
        return nullptr;

    static unsigned int ownerOffset = 0;

    if (ownerOffset == 0)
    {
        sm_sendprop_info_t sendpropInfo {};
        _gameHelpers->FindSendPropInfo("CBaseEntity", "m_hOwnerEntity", &sendpropInfo);

        if (sendpropInfo.prop == nullptr)
            return nullptr;

        ownerOffset = sendpropInfo.actual_offset;
    }

    auto *ownerHandle = (CBaseHandle *)((uint8_t *)entity + ownerOffset);

    if (ownerHandle == nullptr)
        return nullptr;

    return GetEntityFromHandle(ownerHandle);
}

CBaseEntity *EntityUtilities::GetEntityByIndex(int entityIndex, bool isPlayer) const
{
    edict_t *edict = _gameHelpers->EdictOfIndex(entityIndex);

    if (edict == nullptr || edict->IsFree())
        return nullptr;

    if (entityIndex > 0 && entityIndex <= _smPlayerManager->GetMaxClients())
    {
        IGamePlayer *gamePlayer = playerhelpers->GetGamePlayer(edict);
        if (gamePlayer == nullptr || !gamePlayer->IsConnected())
            return nullptr;
    }
    else if (isPlayer)
        return nullptr;

    IServerUnknown *serverUnknown;
    if ((serverUnknown = edict->GetUnknown()) == nullptr)
        return nullptr;

    return serverUnknown->GetBaseEntity();
}

int EntityUtilities::GetIndexByEntity(CBaseEntity *entity) const
{
    auto reference = _gameHelpers->EntityToReference(entity);

    if (reference == 0)
        return 0;

    return _gameHelpers->ReferenceToIndex(reference);
}

CBaseEntity *EntityUtilities::FindEntityInListByClassName(const SourceHook::CVector<CBaseEntity *>& searchList, const char *className) const
{
    for (auto entity : searchList)
    {
        if (entity == nullptr)
            continue;

        auto entityClassName = _gameHelpers->GetEntityClassname(entity);

        if (entityClassName == nullptr)
            continue;

        if (strcmp(className, entityClassName) == 0)
            return entity;
    }

    return nullptr;
}

void EntityUtilities::KillEntity(CBaseEntity *entity)
{
    if (entity == nullptr)
        return;

    SDKVariantT voidVariant;
    Callables_Call_AcceptEntityInput(entity, "kill", nullptr, nullptr, &voidVariant, 0);
}

Vector EntityUtilities::GetEntityAbsOrigin(CBaseEntity *entity) const
{
    if (entity == nullptr)
        return { 0, 0, 0 };

    static unsigned int absOriginOffset = 0;

    if (absOriginOffset == 0)
    {
        sm_sendprop_info_t sendpropInfo {};
        _gameHelpers->FindSendPropInfo("CBaseEntity", "m_vecOrigin", &sendpropInfo);

        if (sendpropInfo.prop == nullptr)
            return { 0, 0, 0 };

        absOriginOffset = sendpropInfo.actual_offset;
    }

    auto *originVector = (Vector *)((uint8_t *)entity + absOriginOffset);

    return { originVector->x, originVector->y, originVector->z };
}