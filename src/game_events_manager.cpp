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

#include "game_events_manager.h"
#include "smsdk_ext.h"
#include "extension.h"
#include "player_manager.h"
#include "player.h"
#include "entity_utilities.h"

GameEventsManager::GameEventsManager()
{
    _gameEventsManager = nullptr;
    _gameHelpers = nullptr;
}

bool GameEventsManager::Init(ISmmAPI *ismm, char *error, size_t maxlen)
{
    GET_V_IFACE_CURRENT(GetEngineFactory, _gameEventsManager, IGameEventManager2, INTERFACEVERSION_GAMEEVENTSMANAGER2);
    _gameEventsManager->AddListenerGlobal(this, true);
    return true;
}

GameEventsManager::~GameEventsManager()
{
    if (_gameEventsManager)
        _gameEventsManager->RemoveListener(this);
}

void GameEventsManager::FireGameEvent(IGameEvent *event)
{
    if (event == nullptr)
        return;

    auto eventName = event->GetName();
    auto isFlashbangDetonateEvent = strcmp(eventName, "flashbang_detonate") == 0;
    auto isSmokeDetonateEvent = strcmp(eventName, "smokegrenade_detonate") == 0;
    auto isDecoyStartedEvent = strcmp(eventName, "decoy_started") == 0;
    auto isHEGrenadeDetonateEvent = strcmp(eventName, "hegrenade_detonate") == 0;

    if (!isFlashbangDetonateEvent && !isSmokeDetonateEvent && !isDecoyStartedEvent && !isHEGrenadeDetonateEvent)
        return;

    auto userId = event->GetInt("userid");
    auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByUserId(userId);

    if (player == nullptr)
        return;

    GrenadeType grenadeType = GrenadeType_UNKNOWN;

    if (isFlashbangDetonateEvent)
        grenadeType = GrenadeType_FLASH;
    else if (isSmokeDetonateEvent)
        grenadeType = GrenadeType_SMOKE;
    else if (isDecoyStartedEvent)
        grenadeType = GrenadeType_DECOY;
    else if (isHEGrenadeDetonateEvent)
        grenadeType = GrenadeType_HEGRENADE;

    auto entityId = event->GetInt("entityid");

    if (entityId <= 0)
        return;

    auto projectileReference = _gameHelpers->IndexToReference(entityId);

    if (projectileReference == 0)
        return;

    player->OnGrenadeDetonationEvent(grenadeType, projectileReference);
}

int	GameEventsManager::GetEventDebugID()
{
    return 42;
}