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

#ifndef JABRONEZ_RECIPIENT_FILTER_H
#define JABRONEZ_RECIPIENT_FILTER_H

#include "smsdk_ext.h"
#include <sh_vector.h>

class Player;

class RecipientFilter : public IRecipientFilter
{
public:
    explicit RecipientFilter(const SourceHook::CVector<Player*>& players, bool isReliable, bool isInitMessage);
    virtual ~RecipientFilter();

    bool IsReliable( void ) const override;
    bool IsInitMessage( void ) const override;
    int GetRecipientCount() const override;
    int GetRecipientIndex(int slot) const override;

private:
    SourceHook::CVector<Player*> _players;
    bool _isReliable;
    bool _isInitMessage;
};

#endif
