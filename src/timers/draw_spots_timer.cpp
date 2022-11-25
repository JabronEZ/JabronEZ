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
#include "temporary_entities.h"
#include "temporary_entity_creator.h"
#include "recipient_filter.h"

DrawSpotsTimer::DrawSpotsTimer(ITimerSystem *timerSystem, IVEngineServer *engineServer)
{
    _timerSystem = timerSystem;
    _engineServer = engineServer;
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
    _laserBeamSprite = _engineServer->PrecacheModel("materials/sprites/laserbeam.vmt", false);

    _timer = _timerSystem->CreateTimer(
            this,
            0.5f,
            nullptr,
            TIMER_FLAG_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
}

ResultType DrawSpotsTimer::OnTimer(ITimer *timer, void *data)
{
    auto players = g_JabronEZ.GetPlayerManager()->GetPlayers();
    auto playerCount = players.size();
    auto yellowColor = Color(255, 255, 0, 200);
    auto whiteColor = Color(255, 255, 255, 200);

    for(size_t playerIndex = 0; playerIndex < playerCount; playerIndex++)
    {
        auto player = players.at(playerIndex);

        if(player == nullptr || !player->IsValid())
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
}

void DrawSpotsTimer::DrawLine(Player *player, Vector point1, Vector point2, Color color) const
{
    auto beamPoints = g_JabronEZ.GetTemporaryEntities()->FindByName("BeamPoints");
    beamPoints->SetPropertyVector("m_vecStartPoint", point1);
    beamPoints->SetPropertyVector("m_vecEndPoint", point2);
    beamPoints->SetPropertyInt("m_nModelIndex", _laserBeamSprite);
    beamPoints->SetPropertyInt("m_nHaloIndex", _laserBeamSprite);
    beamPoints->SetPropertyInt("m_nStartFrame", 0);
    beamPoints->SetPropertyInt("m_nFrameRate", 0);
    beamPoints->SetPropertyFloat("m_fLife", 0.5f);
    beamPoints->SetPropertyFloat("m_fWidth", 5.0f);
    beamPoints->SetPropertyFloat("m_fEndWidth", 5.0f);
    beamPoints->SetPropertyFloat("m_fAmplitude", 0.0f);
    beamPoints->SetPropertyChar("r", (char)color.r());
    beamPoints->SetPropertyChar("g", (char)color.g());
    beamPoints->SetPropertyChar("b", (char)color.b());
    beamPoints->SetPropertyChar("a", (char)color.a());
    beamPoints->SetPropertyInt("m_nSpeed", 0);
    beamPoints->SetPropertyInt("m_nFadeLength", 0);

    SourceHook::CVector<Player*> recipients;
    recipients.clear();
    recipients.push_back(player);

    RecipientFilter filter = RecipientFilter(recipients, false, false);
    beamPoints->Send(filter, 0.0f);
}

void DrawSpotsTimer::DrawRectangle(Player *player, Vector point1, Vector point2, Color color) const
{
    // Here is a visualization of each of the points on the rectangle:
    //         H               G
    //          +-------------+
    //         /|            /|
    //        / |           / |
    //       /  |          /  |
    //    F +-------------+ B |
    //      |   |         |   |
    //      |   +---------|---+
    //      |  / E        |  /  C
    //      | /           | /
    //      |/            |/
    //      +-------------+
    //    D                A
    auto pointA = Vector(point1.x, point1.y, point1.z);
    auto pointH = Vector(point2.x, point2.y, point2.z);
    auto pointB = Vector(point1.x, point1.y, point2.z);
    auto pointC = Vector(point1.x, point2.y, point1.z);
    auto pointD = Vector(point2.x, point1.y, point1.z);
    auto pointE = Vector(point2.x, point2.y, point1.z);
    auto pointF = Vector(point2.x, point1.y, point2.z);
    auto pointG = Vector(point1.x, point2.y, point2.z);

    DrawLine(player, pointA, pointB, color);
    DrawLine(player, pointA, pointC, color);
    DrawLine(player, pointA, pointD, color);
    DrawLine(player, pointH, pointE, color);
    DrawLine(player, pointH, pointF, color);
    DrawLine(player, pointH, pointG, color);
    DrawLine(player, pointC, pointE, color);
    DrawLine(player, pointG, pointC, color);
    DrawLine(player, pointG, pointB, color);
    DrawLine(player, pointF, pointB, color);
    DrawLine(player, pointF, pointD, color);
    DrawLine(player, pointD, pointE, color);
}