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

#include "temporary_entity_creator.h"
#include "callables.h"
#include <server_class.h>

TemporaryEntityCreator::TemporaryEntityCreator(
        void *temporaryEntity,
        const char *name,
        IGameHelpers *gameHelpers,
        IVEngineServer *engineServer)
{
    _temporaryEntity = temporaryEntity;
    snprintf(_name, sizeof(_name), "%s", name);
    _gameHelpers = gameHelpers;
    _engineServer = engineServer;
}

TemporaryEntityCreator::~TemporaryEntityCreator()
{
    _temporaryEntity = nullptr;
}

ServerClass *TemporaryEntityCreator::GetServerClass() const
{
    return Callables_Call_TE_GetServerClass(_temporaryEntity);
}

int TemporaryEntityCreator::FindPropertyOffset(const char *propertyName) const
{
    // FIXME: In the future, it might be worth attempting to cache these in a dictionary.
    sm_sendprop_info_t sendpropInfo {};
    if (!_gameHelpers->FindSendPropInfo(GetServerClass()->GetName(), propertyName, &sendpropInfo))
        return -1;

    return (int)sendpropInfo.actual_offset;
}

void TemporaryEntityCreator::SetPropertyChar(const char *propertyName, char value) const
{
    auto offset = FindPropertyOffset(propertyName);

    if (offset < 0)
        return;

    *(char *)((uint8_t *)_temporaryEntity + offset) = value;
}

void TemporaryEntityCreator::SetPropertyShort(const char *propertyName, short value) const
{
    auto offset = FindPropertyOffset(propertyName);

    if (offset < 0)
        return;

    *(short *)((uint8_t *)_temporaryEntity + offset) = value;
}

void TemporaryEntityCreator::SetPropertyInt(const char *propertyName, int value) const
{
    auto offset = FindPropertyOffset(propertyName);

    if (offset < 0)
        return;

    *(int *)((uint8_t *)_temporaryEntity + offset) = value;
}

void TemporaryEntityCreator::SetPropertyFloat(const char *propertyName, float value) const
{
    auto offset = FindPropertyOffset(propertyName);

    if (offset < 0)
        return;

    *(float *)((uint8_t *)_temporaryEntity + offset) = value;
}

void TemporaryEntityCreator::SetPropertyVector(const char *propertyName, Vector vector) const
{
    auto offset = FindPropertyOffset(propertyName);

    if (offset < 0)
        return;

    auto *vectorProperty = (Vector *)((uint8_t *)_temporaryEntity + offset);
    vectorProperty->x = vector.x;
    vectorProperty->y = vector.y;
    vectorProperty->z = vector.z;
}

void TemporaryEntityCreator::SetPropertyFloatArray(const char *propertyName, const float *array, size_t size) const
{
    auto offset = FindPropertyOffset(propertyName);

    if (offset < 0)
        return;

    auto *base = (float *)((uint8_t *)_temporaryEntity + offset);

    for (size_t index = 0; index < size; index++)
    {
        base[index] = array[index];
    }
}

void TemporaryEntityCreator::Send(IRecipientFilter &filter, float delay) const
{
    auto serverClass = GetServerClass();

    _engineServer->PlaybackTempEntity(
            filter,
            delay,
            _temporaryEntity,
            serverClass->m_pTable,
            serverClass->m_ClassID);
}
