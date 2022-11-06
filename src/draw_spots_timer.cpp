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

#include "draw_spots_timer.h"
#include "extension.h"
#include "player_manager.h"
#include "player.h"

DrawSpotsTimer::DrawSpotsTimer(ITimerSystem *timerSystem)
{
    _timerSystem = timerSystem;
    _timer = nullptr;
}

DrawSpotsTimer::~DrawSpotsTimer()
{
    if (_timer != nullptr && _timerSystem != nullptr)
        _timerSystem->KillTimer(_timer);

    _timer = nullptr;
}

void DrawSpotsTimer::Init()
{
    META_CONPRINTF("DrawSpotsTimer::Init()\n");

    _timer = _timerSystem->CreateTimer(
            this,
            0.5f,
            nullptr,
            TIMER_FLAG_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
}

ResultType DrawSpotsTimer::OnTimer(ITimer *timer, void *data)
{
    META_CONPRINTF("DrawSpotsTimer::OnTimer()\n");

    auto players = g_JabronEZ.GetPlayerManager()->GetPlayers();
    auto playerCount = players.size();
    auto yellowColor = Color(255, 255, 0, 200);
    auto whiteColor = Color(255, 255, 255, 200);

    for(size_t playerIndex = 0; playerIndex < playerCount; playerIndex++)
    {
        auto player = players.at(playerIndex);
        auto gamePlayer = player->GetGamePlayer();

        if(!gamePlayer->IsConnected() || !gamePlayer->IsInGame())
            continue;

        auto spots = player->GetGrenadeSpots();
        auto spotCount = spots.size();
        size_t closestSpotIndex;
        auto closestSpot = player->GetClosestGrenadeSpot(&closestSpotIndex);

        for(size_t spotIndex = 0; spotIndex < spotCount; spotIndex++)
        {
            auto spot = spots.at(spotIndex);
            auto spotOrigin = spot.GetOrigin();

            // Client bounding box point 1
            auto point1 = Vector(
                    spotOrigin.x - 16.0f,
                    spotOrigin.y - 16.0f,
                    spotOrigin.z);

            // Client bounding box point 2
            auto point2 = Vector(
                    spotOrigin.x + 16.0f,
                    spotOrigin.y + 16.0f,
                    spotOrigin.z + 32.0f);

            auto spotColor = spotIndex == closestSpotIndex
                ? yellowColor
                : whiteColor;

            DrawRectangle(player, point1, point2, spotColor);
        }
    }

    return SourceMod::Pl_Continue;
}

void DrawSpotsTimer::OnTimerEnd(ITimer *timer, void *data)
{
    META_CONPRINTF("DrawSpotsTimer::OnTimerEnd()\n");
}

void DrawSpotsTimer::DrawRectangle(Player *player, Vector point1, Vector point2, Color color) const
{
    META_CONPRINTF("DrawSpotsTimer::DrawRectangle() %p [%f %f %f] [%f %f %f] [%d %d %d %d]\n", player, point1.x, point1.y, point1.z, point2.x, point2.y, point2.z, color.r(), color.g(), color.b(), color.a());
}