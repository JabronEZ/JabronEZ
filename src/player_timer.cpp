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
#include "player_manager.h"
#include "player_timer.h"


PlayerTimer::PlayerTimer(Player *player, float interval, ITimerSystem *timerSystem)
{
    _timerSystem = timerSystem;
    _timer = timerSystem->CreateTimer(this, interval, nullptr, TIMER_FLAG_NO_MAPCHANGE);
    _userId = player->GetUserId();
    _isInTimerCallback = false;
}

PlayerTimer::~PlayerTimer()
{
    if (_timer != nullptr && _timerSystem != nullptr && !_isInTimerCallback)
    {
        _timerSystem->KillTimer(_timer);
    }

    _timer = nullptr;
}

ResultType PlayerTimer::OnTimer(ITimer *timer, void *data)
{
    _isInTimerCallback = true;

    if (!_isKilled)
        OnPlayerTimer();

    return SourceMod::Pl_Stop;
}

void PlayerTimer::OnTimerEnd(ITimer *timer, void *data)
{
    OnPlayerTimerEnd();

    _timer = nullptr;
    delete this;
}

Player *PlayerTimer::GetPlayer() const
{
    return g_JabronEZ.GetPlayerManager()->GetPlayerByUserId(_userId);
}

void PlayerTimer::KillTimerSafely()
{
    if (_isKilled)
        return;

    _isKilled = true;

    if (!_isInTimerCallback && _timer != nullptr && _timerSystem != nullptr)
    {
        _timerSystem->KillTimer(_timer);
    }

    _timer = nullptr;
}
