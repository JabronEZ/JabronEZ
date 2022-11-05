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

CBaseEntity *EntityUtilities::GetEntityByIndex(int entityIndex, bool isPlayer)
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