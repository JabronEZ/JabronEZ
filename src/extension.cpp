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
#include "hooks.h"
#include "console_manager.h"
#include "menus.h"
#include "player_manager.h"
#include "translations.h"
#include "hud_utilities.h"
#include "entity_utilities.h"

JabronEZ g_JabronEZ;

SMEXT_LINK(&g_JabronEZ);
Vector lastOrigin;
QAngle lastAngle;
Vector lastVelocity;
Vector lastAngularImpulse;

IGameConfig *g_GameConf = nullptr;

bool JabronEZ::SDK_OnLoad(char *error, size_t maxlength, bool late)
{
    if (strcmp(g_pSM->GetGameFolderName(), "csgo") != 0)
    {
        snprintf(error, maxlength, "JabronEZ is only compatible with CS:GO");
        return false;
    }

    sharesys->AddDependency(myself, "bintools.ext", true, true);

    char confError[256] = "Unknown error occurred";

    if (!gameconfs->LoadGameConfigFile("jabronez.games", &g_GameConf, confError, sizeof(confError)))
    {
        if (error)
        {
            snprintf(error, maxlength, "Could not read jabronez.games: %s", confError);
        }

        return false;
    }

    if (!Hooks_Init(
            g_pSM->GetScriptingEngine(),
            g_GameConf,
            error,
            maxlength))
    {
        return false;
    }

    _menus = new Menus();
    _playerManager = new PlayerManager();
    _translations = new Translations(translator);
    _hudUtilities = new HudUtilities(gamehelpers);
    _entityUtilities = new EntityUtilities(gamehelpers, playerhelpers);

    if (!_translations->Init(error, maxlength))
    {
        return false;
    }

    return true;
}

void JabronEZ::SDK_OnUnload()
{
    Hooks_Cleanup();

    if (_consoleManager != nullptr)
    {
        delete _consoleManager;
        _consoleManager = nullptr;
    }

    if (_menus != nullptr)
    {
        delete _menus;
        _menus = nullptr;
    }

    if (_playerManager != nullptr)
    {
        delete _playerManager;
        _playerManager = nullptr;
    }

    if (_translations != nullptr)
    {
        delete _translations;
        _translations = nullptr;
    }

    if (_hudUtilities != nullptr)
    {
        delete _hudUtilities;
        _hudUtilities = nullptr;
    }

    if (_entityUtilities != nullptr)
    {
        delete _entityUtilities;
        _entityUtilities = nullptr;
    }
}

bool JabronEZ::SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlength, bool late)
{
    _consoleManager = new ConsoleManager();

    if (!_consoleManager->Init(ismm, error, maxlength))
    {
        return false;
    }

    return true;
}
