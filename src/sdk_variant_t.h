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

#ifndef JABRONEZ_SDK_VARIANT_T_H
#define JABRONEZ_SDK_VARIANT_T_H

#include "smsdk_ext.h"

class SDKVariantT
{
public:
    explicit SDKVariantT();

    bool GetBoolValue() const;
    string_t GetStringValue() const;
    int GetIntValue() const;
    float GetFloatValue() const;
    Vector GetVectorValue() const;
    color32 GetColorValue() const;
    uint32_t GetHandleIndexOrPointer() const;
    fieldtype_t GetFieldType() const;

    void SetBoolValue(bool value);
    void SetStringValue(string_t value);
    void SetIntValue(int value);
    void SetFloatValue(float value);
    void SetVector3DValue(Vector value);
    void SetVectorPosition3DValue(Vector value);
    void SetColorValue(color32 value);

    void Set(fieldtype_t fieldType, void *value);

private:
    // The order of all these fields matters, so we can match the SDKs versions roughly.
    union
    {
        bool _boolValue;
        string_t _stringValue;
        int _intValue;
        float _floatValue;
        float _vectorValue[3];
        color32 _colorValue;
    };

    uint32_t _handleIndexOrPointer;
    fieldtype_t _fieldType;
};

#endif
