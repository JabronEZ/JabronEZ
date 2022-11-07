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

#ifndef JABRONEZ_GRENADE_TRIGGER_PLAYBACK_TIMER_H
#define JABRONEZ_GRENADE_TRIGGER_PLAYBACK_TIMER_H

#include "smsdk_ext.h"
#include "player_timer.h"

class Player;

class GrenadeTriggerPlaybackTimer : public PlayerTimer {
public:
    explicit GrenadeTriggerPlaybackTimer(Player *player, float postDetonationDelay, ITimerSystem *timerSystem);
    ~GrenadeTriggerPlaybackTimer() override;

    void OnPlayerTimer() override;
    void OnPlayerTimerEnd() override;

};

#endif
