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

#include "temporary_entities.h"
#include "temporary_entity_creator.h"
#include <server_class.h>

TemporaryEntities::TemporaryEntities(IGameHelpers *gameHelpers, IVEngineServer *engineServer)
{
    _gameHelpers = gameHelpers;
    _engineServer = engineServer;
}

TemporaryEntities::~TemporaryEntities()
{
    _gameHelpers = nullptr;
    _engineServer = nullptr;
}

bool TemporaryEntities::Init(
        IGameConfig *sdktoolsGameConfig,
        char *error,
        size_t maxlength)
{
    void *cbaseTempEntityAddress;
    int offset;

    if (!sdktoolsGameConfig->GetMemSig("CBaseTempEntity", &cbaseTempEntityAddress) || cbaseTempEntityAddress == nullptr)
    {
        snprintf(error, maxlength, "Unable to find CBaseTempEntity");
        return false;
    }

    if (!sdktoolsGameConfig->GetOffset("s_pTempEntities", &offset))
    {
        snprintf(error, maxlength, "Unable to find m_pTempEntities offset");
        return false;
    }

    _headOfList = **(void ***) ((unsigned char *)cbaseTempEntityAddress + offset);

    if (!sdktoolsGameConfig->GetOffset("GetTEName", &_getTENameOffset))
    {
        snprintf(error, maxlength, "Unable to find GetTEName offset");
        return false;
    }

    if (!sdktoolsGameConfig->GetOffset("GetTENext", &_getTENextOffset))
    {
        snprintf(error, maxlength, "Unable to find GetTENext offset");
        return false;
    }

    void *currentTempEnt = _headOfList;
    while (currentTempEnt != nullptr)
    {
        auto name = *reinterpret_cast<const char **>(((uint8_t*)currentTempEnt + _getTENameOffset));
        auto creator = new TemporaryEntityCreator(currentTempEnt, name, _gameHelpers, _engineServer);
        _creators.push_back(creator);
        currentTempEnt = *reinterpret_cast<void **>(((uint8_t*)currentTempEnt + _getTENextOffset));
    }

    return true;
}

TemporaryEntityCreator *TemporaryEntities::FindByName(const char *name) const
{
    for (auto creator : _creators)
    {
        if (strcmp(name, creator->GetName()) == 0)
        {
            return creator;
        }
    }

    return nullptr;
}
