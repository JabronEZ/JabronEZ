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

#ifndef JABRONEZ_GAME_EVENTS_MANAGER_H
#define JABRONEZ_GAME_EVENTS_MANAGER_H

#include "smsdk_ext.h"
#include <igameevents.h>

class ICvar;
class ConCommandBase;

class GameEventsManager : public IGameEventListener2
{
public:
    explicit GameEventsManager();
    ~GameEventsManager() override;

    bool Init(ISmmAPI *ismm, char *error, size_t maxlength);

    void FireGameEvent(IGameEvent *event) override;
    int	GetEventDebugID() override;

    void SetGameHelpers(IGameHelpers *pHelpers) { _gameHelpers = gamehelpers; }

private:
    IGameEventManager2 *_gameEventsManager { nullptr };
    IGameHelpers *_gameHelpers { nullptr };
};

#endif
