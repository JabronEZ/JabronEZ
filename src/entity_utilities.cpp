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

    sm_sendprop_info_t sendpropInfo {};
    _gameHelpers->FindSendPropInfo("CMolotovProjectile", "m_bIsIncGrenade", &sendpropInfo);

    if (sendpropInfo.prop == nullptr)
        return false;

    uint8_t isIncendiary = *((uint8_t *)entity + sendpropInfo.actual_offset);
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

    sm_sendprop_info_t throwerSendpropInfo {};
    _gameHelpers->FindSendPropInfo("CBaseCSGrenadeProjectile", "m_hThrower", &throwerSendpropInfo);

    sm_sendprop_info_t ownerSendpropInfo {};
    _gameHelpers->FindSendPropInfo("CBaseEntity", "m_hOwnerEntity", &ownerSendpropInfo);

    if (throwerSendpropInfo.prop != nullptr)
    {
        auto *throwerHandle = (CBaseHandle *)((uint8_t *)entity + throwerSendpropInfo.actual_offset);

        if (throwerHandle != nullptr)
        {
            auto throwerEntity = GetEntityFromHandle(throwerHandle);

            if (throwerEntity != nullptr)
                return throwerEntity;
        }
    }

    if (ownerSendpropInfo.prop != nullptr)
    {
        auto *ownerHandle = (CBaseHandle *)((uint8_t *)entity + ownerSendpropInfo.actual_offset);

        if (ownerHandle != nullptr)
        {
            auto ownerEntity = GetEntityFromHandle(ownerHandle);

            if (ownerEntity != nullptr)
                return ownerEntity;
        }
    }

    return nullptr;
}

CBaseEntity *EntityUtilities::GetEntityByIndex(int entityIndex, bool isPlayer) const
{
    edict_t *edict = _gameHelpers->EdictOfIndex(entityIndex);

    if (edict == nullptr || edict->IsFree())
    {
        return nullptr;
    }

    if (entityIndex > 0 && entityIndex <= _smPlayerManager->GetMaxClients())
    {
        IGamePlayer *gamePlayer = playerhelpers->GetGamePlayer(edict);
        if (gamePlayer == nullptr || !gamePlayer->IsConnected())
        {
            return nullptr;
        }
    }
    else if (isPlayer)
    {
        return nullptr;
    }

    IServerUnknown *serverUnknown;
    if ((serverUnknown = edict->GetUnknown()) == nullptr)
    {
        return nullptr;
    }

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
