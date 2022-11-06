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

#ifndef JABRONEZ_TEMPORARY_ENTITIES_H
#define JABRONEZ_TEMPORARY_ENTITIES_H

#include "smsdk_ext.h"
#include <sh_vector.h>

class TemporaryEntityCreator;

class TemporaryEntities
{
public:
    explicit TemporaryEntities(IGameHelpers *gameHelpers, IVEngineServer *engineServer);
    ~TemporaryEntities();

public:
    bool Init(IGameConfig *sdktoolsGameConfig, char *error, size_t maxlength);
    TemporaryEntityCreator *FindByName(const char *name) const;

private:
    IGameHelpers *_gameHelpers { nullptr };
    IVEngineServer *_engineServer { nullptr };

    void *_headOfList { nullptr };
    int _getTENameOffset { -1 };
    int _getTENextOffset { -1 };

    SourceHook::CVector<TemporaryEntityCreator*> _creators;
};

#endif
