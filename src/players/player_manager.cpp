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

#include "player_manager.h"
#include "smsdk_ext.h"
#include "player.h"
#include "entity_utilities.h"
#include "extension.h"

PlayerManager::PlayerManager(IPlayerManager *smPlayerManager, IGameHelpers *gameHelpers, ITimerSystem *timerSystem)
{
    _smPlayerManager = smPlayerManager;
    _gameHelpers = gameHelpers;
    _timerSystem = timerSystem;
    _players.clear();

    _smPlayerManager->AddClientListener(this);
}

PlayerManager::~PlayerManager()
{
    _smPlayerManager->RemoveClientListener(this);
    _gameHelpers = nullptr;

    // We do not use CleanupPlayer() here since it would break our iterators (ie: removing in the middle of a loop).
    for (auto player : _players)
    {
        delete player;
    }

    _players.clear();
}

void PlayerManager::OnClientConnected(int clientIndex)
{
    if (clientIndex < 1)
    {
        return;
    }

    auto gamePlayer = _smPlayerManager->GetGamePlayer(clientIndex);
    auto player = new Player(clientIndex, gamePlayer->GetUserId(), gamePlayer, _gameHelpers, _timerSystem);

    _players.push_back(player);
}

void PlayerManager::OnClientDisconnecting(int clientIndex)
{
    if (clientIndex < 1)
    {
        return;
    }

    auto player = GetPlayerByClientIndex(clientIndex);

    if (player == nullptr)
        return;

    CleanupPlayer(player);
}

Player *PlayerManager::GetPlayerByBaseEntity(CBaseEntity *entity) const
{
    // TODO: This can definitely be optimized.
    if (entity == nullptr)
        return nullptr;

    auto maxClient = _smPlayerManager->GetMaxClients();

    for (auto clientIndex = 1; clientIndex <= maxClient; clientIndex++)
    {
        auto playerEntity = g_JabronEZ.GetEntityUtilities()->GetEntityByIndex(clientIndex, true);

        if (entity == playerEntity)
            return GetPlayerByClientIndex(clientIndex);
    }

    return nullptr;
}

Player *PlayerManager::GetPlayerByClientIndex(int clientIndex) const
{
    for (auto player : _players)
    {
        if (player->GetClientIndex() == clientIndex)
        {
            return player;
        }
    }

    return nullptr;
}

Player *PlayerManager::GetPlayerByUserId(int userId) const
{
    for (auto player : _players)
    {
        if (player->GetUserId() == userId)
        {
            return player;
        }
    }

    return nullptr;
}

void PlayerManager::CleanupPlayer(Player *player)
{
    for (size_t playerIndex = 0; playerIndex < _players.size(); playerIndex++)
    {
        auto checkPlayer = _players.at(playerIndex);

        if (checkPlayer == player)
        {
            auto iter = _players.iterAt(playerIndex);
            _players.erase(iter);
            break;
        }
    }

    delete player;
}

Player *PlayerManager::GetPlayerByEdict(edict_t *edict) const
{
    for (auto player : _players)
    {
        if (player->GetGamePlayer()->GetEdict() == edict)
        {
            return player;
        }
    }

    return nullptr;
}

const SourceHook::CVector<Player *> &PlayerManager::GetPlayers() const
{
    return _players;
}

SourceHook::CVector<Player *> PlayerManager::GetValidPlayers() const
{
    SourceHook::CVector<Player *> validPlayers;
    validPlayers.clear();

    for (auto player : _players)
    {
        if (player->IsValid())
        {
            validPlayers.push_back(player);
        }
    }

    return validPlayers;
}