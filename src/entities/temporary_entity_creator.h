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

#ifndef JABRONEZ_TEMPORARY_ENTITY_CREATOR_H
#define JABRONEZ_TEMPORARY_ENTITY_CREATOR_H

#include "smsdk_ext.h"

class TemporaryEntityCreator
{
public:
    explicit TemporaryEntityCreator(
            void *temporaryEntity,
            const char *name,
            IGameHelpers *gameHelpers,
            IVEngineServer *engineServer);
    ~TemporaryEntityCreator();

    const char *GetName() { return _name; }
    ServerClass *GetServerClass() const;
    void SetPropertyChar(const char *propertyName, char value) const;
    void SetPropertyShort(const char *propertyName, short value) const;
    void SetPropertyInt(const char *propertyName, int value) const;
    void SetPropertyFloat(const char *propertyName, float value) const;
    void SetPropertyVector(const char *propertyName, Vector vector) const;
    void SetPropertyFloatArray(const char *propertyName, const float *array, size_t size) const;
    void Send(IRecipientFilter &filter, float delay) const;

private:
    int FindPropertyOffset(const char *propertyName) const;

private:
    char _name[1024] { 0 };
    void *_temporaryEntity { nullptr };
    IGameHelpers *_gameHelpers { nullptr };
    IVEngineServer *_engineServer { nullptr };
};

#endif
