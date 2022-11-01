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

JabronEZ g_JabronEZ;

SMEXT_LINK(&g_JabronEZ);

IGameConfig *g_GameConf = nullptr;

#ifdef _WIN32
CBaseEntity* (__fastcall *DetourCSmokeGrenadeProjectileCreate_Actual)(
        const Vector& origin,
        const QAngle& angle,
        const Vector& velocity,
        const Vector& angularImpulse,
        void *player,
        int grenadeType) = nullptr;

CBaseEntity* __fastcall DetourCSmokeGrenadeProjectileCreate(
        const Vector& origin,
        const QAngle& angle,
        const Vector& velocity,
        const Vector& angularImpulse,
        void *player,
        int grenadeType)
#else
CBaseEntity* (__cdecl *DetourCSmokeGrenadeProjectileCreate_Actual)(
        const Vector& origin,
        const QAngle& angle,
        const Vector& velocity,
        const Vector& angularImpulse,
        void *player,
        int grenadeType) = nullptr;

CBaseEntity* __cdecl DetourCSmokeGrenadeProjectileCreate(
        const Vector& origin,
        const QAngle& angle,
        const Vector& velocity,
        const Vector& angularImpulse,
        void *player,
        int grenadeType)
#endif
{
    engine->ServerCommand("echo Maybe a smoke is thrown?!\n");
    return DetourCSmokeGrenadeProjectileCreate_Actual(origin, angle, velocity, angularImpulse, player, grenadeType);
}

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

    _hooks = new Hooks(g_pSM->GetScriptingEngine(), g_GameConf);
    _hooks->NewHook(
            "CSmokeGrenadeProjectileCreate",
            (void*)&DetourCSmokeGrenadeProjectileCreate,
            (void**)&DetourCSmokeGrenadeProjectileCreate_Actual);

    if (!_hooks->InstallHooks(error, maxlength))
    {
        return false;
    }

    return true;
}

void JabronEZ::SDK_OnUnload()
{
    if (_hooks != nullptr)
    {
        _hooks->UninstallHooks();

        delete _hooks;
        _hooks = nullptr;
    }
}
