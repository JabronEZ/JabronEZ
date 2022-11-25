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

#include "recipient_filter.h"
#include "player.h"

RecipientFilter::RecipientFilter(const SourceHook::CVector<Player *>& players, bool isReliable, bool isInitMessage)
{
    // FIXME: Should we really be storing the players here, or should we store the user IDs instead?
    //        There might be a case where the recipient filter is dispatched later.
    _players = players;
    _isReliable = isReliable;
    _isInitMessage = isInitMessage;
}

RecipientFilter::~RecipientFilter()
{
    _players.clear();
}

int RecipientFilter::GetRecipientCount() const
{
    return (int)_players.size();
}

int RecipientFilter::GetRecipientIndex(int slot) const
{
    return _players.at(slot)->GetClientIndex();
}

bool RecipientFilter::IsReliable() const
{
    return _isReliable;
}

bool RecipientFilter::IsInitMessage() const
{
    return _isInitMessage;
}

