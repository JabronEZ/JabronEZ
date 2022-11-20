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
#include "simple_timer.h"

SimpleTimer::SimpleTimer(float interval, ITimerSystem *timerSystem, IGameHelpers *gameHelpers)
{
    _timerSystem = timerSystem;
    _gameHelpers = gameHelpers;
    _timer = timerSystem->CreateTimer(this, interval, nullptr, TIMER_FLAG_NO_MAPCHANGE);
    _isInTimerCallback = false;
}

SimpleTimer::~SimpleTimer()
{
    if (_timer != nullptr && _timerSystem != nullptr && !_isInTimerCallback)
    {
        _timerSystem->KillTimer(_timer);
    }

    _timer = nullptr;
}

ResultType SimpleTimer::OnTimer(ITimer *timer, void *data)
{
    _isInTimerCallback = true;

    if (!_isKilled)
        OnSimpleTimer();

    return SourceMod::Pl_Stop;
}

void SimpleTimer::OnTimerEnd(ITimer *timer, void *data)
{
    OnSimpleTimerEnd();

    _timer = nullptr;
    delete this;
}

void SimpleTimer::KillTimerSafely()
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
