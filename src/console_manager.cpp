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

#include "console_manager.h"
#include "smsdk_ext.h"
#include "hooks.h"
#include "extension.h"

ConsoleManager::ConsoleManager()
{
    _cvarInterface = nullptr;
    _rethrowLastSmokeCommand = nullptr;
}

bool ConsoleManager::RegisterConCommandBase(ConCommandBase *conCommandBase)
{
    return META_REGCVAR(conCommandBase);
}

bool ConsoleManager::Init(ISmmAPI *ismm, char *error, size_t maxlen)
{
    // In order for us to be able to register console commands and variables, we must set g_pCVar, call ConVar_Register,
    // and execute META_REGCVAR inside RegisterConCommandBase. Missing any of these things will cause new commands to fail
    // without any particular warning/error.
    // For more information, see: https://wiki.alliedmods.net/Porting_To_Orange_Box
    GET_V_IFACE_CURRENT(GetEngineFactory, _cvarInterface, ICvar, CVAR_INTERFACE_VERSION);
    g_pCVar = _cvarInterface;
    ConVar_Register(0, this);

    _rethrowLastSmokeCommand = new ConCommand(
            "sm_jez_rethrow_last_smoke",
            RethrowLastSmokeCallback,
            "Test help",
            0);

    return true;
}

ConsoleManager::~ConsoleManager()
{
    if (_rethrowLastSmokeCommand != nullptr && _cvarInterface != nullptr)
    {
        _cvarInterface->UnregisterConCommand(_rethrowLastSmokeCommand);
    }

    _rethrowLastSmokeCommand = nullptr;
    _cvarInterface = nullptr;
}

void ConsoleManager::RethrowLastSmokeCallback(const CCommand &command)
{
    void *basePlayer = nullptr;
    int playerIndex = 1;

    if (playerIndex > 0)
    {
        cell_t reference = gamehelpers->IndexToReference(playerIndex);
        basePlayer = gamehelpers->ReferenceToEntity(reference);
    }

    Hook_Call_SmokeProjectileCreate(
            lastOrigin,
            lastAngle,
            lastVelocity,
            lastAngularImpulse,
            basePlayer,
            45);
}
