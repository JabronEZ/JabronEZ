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

#include "hud_utilities.h"
#include "smsdk_ext.h"
#include "player.h"

HudUtilities::HudUtilities(IGameHelpers *gameHelpers)
{
    _gameHelpers = gameHelpers;
}

HudUtilities::~HudUtilities()
{
    _gameHelpers = nullptr;
}

void HudUtilities::PrintToChat(Player *player, const char *message)
{
    if (player == nullptr || !player->IsValid())
        return;

    _gameHelpers->TextMsg(player->GetClientIndex(), TEXTMSG_DEST_CHAT, message);
}