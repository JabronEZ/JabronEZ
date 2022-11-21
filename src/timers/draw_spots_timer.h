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

#ifndef JABRONEZ_DRAW_SPOTS_TIMER_H
#define JABRONEZ_DRAW_SPOTS_TIMER_H

#include "smsdk_ext.h"
#include <sh_vector.h>
#include "weapon_identifiers.h"

class Player;

class DrawSpotsTimer : public ITimedEvent {
public:
    explicit DrawSpotsTimer(ITimerSystem *timerSystem, IVEngineServer *engineServer);
    ~DrawSpotsTimer();

    void Init();
    ResultType OnTimer(ITimer *timer, void *data) override;
    void OnTimerEnd(ITimer *timer, void *data) override;

    void DrawLine(Player *player, Vector point1, Vector point2, Color color) const;
    void DrawRectangle(Player *player, Vector point1, Vector point2, Color color) const;

private:
    ITimerSystem *_timerSystem { nullptr };
    IVEngineServer *_engineServer { nullptr };
    ITimer *_timer { nullptr };
    int _laserBeamSprite { -1 };
};

#endif
