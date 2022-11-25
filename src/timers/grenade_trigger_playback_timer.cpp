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

#include "extension.h"
#include "grenade_trigger_playback_timer.h"
#include "player.h"

GrenadeTriggerPlaybackTimer::GrenadeTriggerPlaybackTimer(Player *player, float postDetonationDelay, ITimerSystem *timerSystem, IGameHelpers *gameHelpers)
    : PlayerTimer(player, postDetonationDelay, timerSystem, gameHelpers)
{
}

GrenadeTriggerPlaybackTimer::~GrenadeTriggerPlaybackTimer()
{
    auto player = GetPlayer();

    if (player != nullptr && player->IsValid())
        player->SetGrenadeTriggerPlaybackTimer(nullptr);
}

void GrenadeTriggerPlaybackTimer::OnSimpleTimer()
{
    auto player = GetPlayer();

    if (player == nullptr || !player->IsValid())
        return;

    player->StartGrenadeTesterPlayback();
}

void GrenadeTriggerPlaybackTimer::OnSimpleTimerEnd()
{
}