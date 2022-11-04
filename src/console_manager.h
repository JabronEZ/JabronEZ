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

#ifndef JABRONEZ_CONSOLE_MANAGER_H
#define JABRONEZ_CONSOLE_MANAGER_H

#include "smsdk_ext.h"

class ICvar;
class ConCommand;
class ConCommandBase;

class ConsoleManager : public IConCommandBaseAccessor
{
public:
    ConsoleManager();
    ~ConsoleManager();

    bool Init(ISmmAPI *ismm, char *error, size_t maxlength);
    bool RegisterConCommandBase(ConCommandBase *conCommandBase) override;

    static void RethrowLastSmokeCallback(const CCommand &command);

    void OnClientCommand(edict_t *edict, const CCommand &args);

private:
    ICvar *_cvarInterface { nullptr };
    IServerGameClients *_serverClients { nullptr };
    ConCommand *_rethrowLastSmokeCommand { nullptr };
};

#endif
