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
#include "remove_short_smoke_timer.h"
#include "entity_utilities.h"

RemoveShortSmokeTimer::RemoveShortSmokeTimer(CBaseEntity *projectileEntity, ITimerSystem *timerSystem, IGameHelpers *gameHelpers)
    : EntityTimer(projectileEntity, 3.5f, timerSystem, gameHelpers)
{
}

RemoveShortSmokeTimer::~RemoveShortSmokeTimer() = default;

void RemoveShortSmokeTimer::OnSimpleTimer()
{
    auto entity = GetEntity();

    if (entity == nullptr)
        return;

    g_JabronEZ.GetEntityUtilities()->KillEntity(entity);
}

void RemoveShortSmokeTimer::OnSimpleTimerEnd()
{
}