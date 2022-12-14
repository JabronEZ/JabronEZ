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
class ConCommandBase;

class ConsoleManager : public IConCommandBaseAccessor
{
public:
    explicit ConsoleManager(IVEngineServer *engineServer);
    ~ConsoleManager();

    bool Init(ISmmAPI *ismm, char *error, size_t maxlength);
    bool RegisterConCommandBase(ConCommandBase *conCommandBase) override;

    void OnClientCommand(edict_t *edict, const CCommand &args);
    void OnSayCommand(const CCommand &command);
    void SetCommandClient(int clientSlot);

    void LoadConfiguration();
    void SendClientCommand(edict_t *edict, const char *command);

private:
    ICvar *_cvarInterface { nullptr };
    IServerGameClients *_serverClients { nullptr };
    IVEngineServer *_engineServer { nullptr };
    IServerPluginHelpers *_pluginHelpers { nullptr };
    int _onClientCommandHookId { 0 };
    int _sayCommandHookId { 0 };
    int _teamSayCommandHookId { 0 };
    int _setCommandClientHookId { 0 };
    int _lastCommandClient { -1 };
};

#endif
