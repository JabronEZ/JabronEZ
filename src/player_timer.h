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

#ifndef JABRONEZ_PLAYER_TIMER_H
#define JABRONEZ_PLAYER_TIMER_H

#include "smsdk_ext.h"
#include "simple_timer.h"

class Player;

class PlayerTimer : public SimpleTimer {
public:
    explicit PlayerTimer(Player *player, float interval, ITimerSystem *timerSystem, IGameHelpers *gameHelpers);
    Player *GetPlayer() const;

private:
    // Timers specific to a player should never assume that the underlying `Player *` is still connected!
    int _userId { -1 };
};

#endif
