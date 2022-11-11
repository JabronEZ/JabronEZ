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

#ifndef JABRONEZ_ENTITY_UTILITIES_H
#define JABRONEZ_ENTITY_UTILITIES_H

#include "smsdk_ext.h"
#include <sh_vector.h>

class Player;

class EntityUtilities {
public:
    explicit EntityUtilities(
            IGameHelpers *gameHelpers,
            IPlayerManager *smPlayerManager);
    ~EntityUtilities();

    CBaseEntity *GetEntityByIndex(int entityIndex, bool isPlayer) const;
    bool IsIncendiaryGrenade(CBaseEntity *entity) const;
    CBaseEntity *GetProjectileThrower(CBaseEntity *entity) const;
    CBaseEntity *GetEntityFromHandle(CBaseHandle *handle) const;
    int GetIndexByEntity(CBaseEntity *entity) const;
    CBaseEntity *FindEntityInListByClassName(const SourceHook::CVector<CBaseEntity *>& searchList, const char *className) const;

private:
    IGameHelpers *_gameHelpers { nullptr };
    IPlayerManager *_smPlayerManager { nullptr };
};

#endif
