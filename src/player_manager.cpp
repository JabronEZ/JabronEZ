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

PlayerManager::PlayerManager()
{
    _players.clear();

    playerhelpers->AddClientListener(this);
}

PlayerManager::~PlayerManager()
{
    playerhelpers->RemoveClientListener(this);

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

    auto gamePlayer = playerhelpers->GetGamePlayer(clientIndex);
    auto player = new Player(clientIndex, gamePlayer->GetUserId(), gamePlayer);

    META_CONPRINTF("Welcome, %s\n", gamePlayer->GetName());

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
    {
        return;
    }

    META_CONPRINTF("Goodbye, %s\n", player->GetGamePlayer()->GetName());

    CleanupPlayer(player);
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