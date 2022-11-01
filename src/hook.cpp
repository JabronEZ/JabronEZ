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

#include "hook.h"
#include <CDetour/detours.h>

Hook::Hook(
    const char *functionName,
    void *callbackFunction,
    void **trampolineFunction)
{
    strncpy(_functionName, functionName, sizeof(_functionName));
    _callbackFunction = callbackFunction;
    _trampolineFunction = trampolineFunction;
    _isInstalled = false;
    _detour = nullptr;
}

Hook::~Hook()
{
    if (!_isInstalled || _detour == nullptr)
    {
        return;
    }

    _detour->Destroy();
    _detour = nullptr;
}

bool Hook::Install(char *error, size_t maxlength)
{
    if (_isInstalled)
    {
        return true;
    }

    _detour = CDetourManager::CreateDetour(
            _callbackFunction,
            _trampolineFunction,
            _functionName);

    if (_detour != nullptr)
    {
        _detour->EnableDetour();
        _isInstalled = true;
        return true;
    }

    if (error)
    {
        snprintf(error, maxlength, "Unable to find address of %s for detour", _functionName);
    }

    return false;
}

void Hook::Uninstall()
{
    if (!_isInstalled || _detour == nullptr)
    {
        return;
    }

    _detour->DisableDetour();
}

bool Hook::IsInstalled() const
{
    return _isInstalled;
}