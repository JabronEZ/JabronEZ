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

#include "functions.h"
#include "extension.h"
#include "hooks.h"

Functions::Functions(IGameConfig *gameConfig)
{
    _gameConfig = gameConfig;
    _funcCSmokeGrenadeProjectileCreate = nullptr;
}

Functions::~Functions()
{
    _gameConfig = nullptr;
    _funcCSmokeGrenadeProjectileCreate = nullptr;
}

bool Functions::Init(char *error, size_t maxlength)
{
    void *smokeGrenadeProjectileCreateAddress = FindFunction("CSmokeGrenadeProjectileCreate", error, maxlength);

    if (smokeGrenadeProjectileCreateAddress == nullptr)
    {
        return false;
    }

    _funcCSmokeGrenadeProjectileCreate = (ProjectileCreatePrototype)smokeGrenadeProjectileCreateAddress;
    return true;
}

void *Functions::FindFunction(const char *functionName, char *error, size_t maxlength)
{
    void *address = nullptr;

    if (!_gameConfig->GetMemSig(functionName, &address) || address == nullptr)
    {
        if (error)
        {
            snprintf(error, maxlength, "Could not locate function: %s", functionName);
        }

        return nullptr;
    }

    return address;
}

CBaseEntity *Functions::CreateSmokeGrenadeProjectile(
        const Vector &origin,
        const QAngle &angle,
        const Vector &velocity,
        const Vector &angularImpulse,
        int playerIndex) {
    if (_funcCSmokeGrenadeProjectileCreate == nullptr)
    {
        return nullptr;
    }

    void *basePlayer = nullptr;

    if (playerIndex > 0)
    {
        cell_t reference = gamehelpers->IndexToReference(playerIndex);
        basePlayer = gamehelpers->ReferenceToEntity(reference);
    }

    g_JabronEZ.GetHooks()->NotifyCreatingSmokeGrenadeProjectile(true);
    CBaseEntity *result = _funcCSmokeGrenadeProjectileCreate(origin, angle, velocity, angularImpulse, basePlayer, 45);
    g_JabronEZ.GetHooks()->NotifyCreatingSmokeGrenadeProjectile(false);
    return result;
}
