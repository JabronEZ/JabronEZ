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

#ifndef _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_

#include "smsdk_ext.h"

class ConsoleManager;
class MenuManager;
class PlayerManager;
class Translations;
class HudUtilities;
class EntityUtilities;

class JabronEZ : public SDKExtension
{
public:
    virtual bool SDK_OnLoad(char *error, size_t maxlength, bool late);
    virtual void SDK_OnUnload();

    virtual bool SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlength, bool late);

    MenuManager *GetMenuManager() const { return _menuManager; }
    PlayerManager *GetPlayerManager() const { return _playerManager; }
    Translations *GetTranslations() const { return _translations; }
    HudUtilities *GetHudUtilities() const { return _hudUtilities; }
    EntityUtilities *GetEntityUtilities() const { return _entityUtilities; }

private:
    ConsoleManager *_consoleManager { nullptr };
    MenuManager *_menuManager { nullptr };
    PlayerManager *_playerManager { nullptr };
    Translations *_translations { nullptr };
    HudUtilities *_hudUtilities { nullptr };
    EntityUtilities *_entityUtilities { nullptr };
};

extern JabronEZ g_JabronEZ;

#endif