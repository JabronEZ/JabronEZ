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

#include "sdk_variant_t.h"
#include "smsdk_ext.h"

SDKVariantT::SDKVariantT() // NOLINT(cppcoreguidelines-pro-type-member-init)
{
    _fieldType = FIELD_VOID;
    _intValue = 0; // This looks wrong, but this is actually how they initialize it in the SDK.
}

bool SDKVariantT::GetBoolValue() const
{
    if (_fieldType == FIELD_BOOLEAN)
        return _boolValue;

    return false;
}

string_t SDKVariantT::GetStringValue() const
{
    if (_fieldType == FIELD_STRING)
        return _stringValue;

    return NULL_STRING;
}

int SDKVariantT::GetIntValue() const
{
    if (_fieldType == FIELD_INTEGER)
        return _intValue;

    return 0;
}

float SDKVariantT::GetFloatValue() const
{
    if (_fieldType == FIELD_FLOAT)
        return _floatValue;

    return 0;
}

Vector SDKVariantT::GetVectorValue() const
{
    if (_fieldType == FIELD_VECTOR || _fieldType == FIELD_POSITION_VECTOR)
        return { _vectorValue[0], _vectorValue[1], _vectorValue[2] };

    return { 0.0f, 0.0f, 0.0f };
}

color32 SDKVariantT::GetColorValue() const
{
    // For some reason, in the SDK they don't actually check _fieldType here.
    return _colorValue;
}

void SDKVariantT::SetBoolValue(bool value)
{
    _boolValue = value;
    _fieldType = FIELD_BOOLEAN;
}

void SDKVariantT::SetStringValue(string_t value)
{
    _stringValue = value;
    _fieldType = FIELD_STRING;
}

void SDKVariantT::SetIntValue(int value)
{
    _intValue = value;
    _fieldType = FIELD_INTEGER;
}

void SDKVariantT::SetFloatValue(float value)
{
    _floatValue = value;
    _fieldType = FIELD_FLOAT;
}

void SDKVariantT::SetVector3DValue(Vector value)
{
    _fieldType = FIELD_VECTOR;
    _vectorValue[0] = value.x;
    _vectorValue[1] = value.y;
    _vectorValue[2] = value.z;
}

void SDKVariantT::SetVectorPosition3DValue(Vector value)
{
    _fieldType = FIELD_POSITION_VECTOR;
    _vectorValue[0] = value.x;
    _vectorValue[1] = value.y;
    _vectorValue[2] = value.z;
}

void SDKVariantT::SetColorValue(color32 value)
{
    _fieldType = FIELD_COLOR32;
    _colorValue = value;
}

// This is pretty much directly copied from the SDK, hence the silencing of warnings.
void SDKVariantT::Set(fieldtype_t fieldType, void *value)
{
    _fieldType = fieldType;

    switch ( fieldType )
    {
        case FIELD_BOOLEAN:
            _boolValue = *((bool *)value);
            break;
        case FIELD_CHARACTER:
            _intValue = *((char *)value); // NOLINT(cert-str34-c)
            break;
        case FIELD_SHORT:
            _intValue = *((short *)value);
            break;
        case FIELD_INTEGER:
            _intValue = *((int *)value);
            break;
        case FIELD_STRING:
            _stringValue = *((string_t *)value);
            break;
        case FIELD_FLOAT:
            _floatValue = *((float *)value);
            break;
        case FIELD_COLOR32:
            _colorValue = *((color32 *)value);
            break;

        case FIELD_VECTOR:
        case FIELD_POSITION_VECTOR:
        {
            _vectorValue[0] = ((float *)value)[0];
            _vectorValue[1] = ((float *)value)[1];
            _vectorValue[2] = ((float *)value)[2];
            break;
        }

        case FIELD_EHANDLE:
            _handleIndexOrPointer = *((uint32_t *)value);
            break;

        case FIELD_CLASSPTR:
            _handleIndexOrPointer = (uint32_t )*((CBaseEntity **)value);
            break;

        case FIELD_VOID:
        default:
            _intValue = 0;
            fieldType = FIELD_VOID;
            break;
    }
}
uint32_t SDKVariantT::GetHandleIndexOrPointer() const
{
    if (_fieldType == FIELD_EHANDLE || _fieldType == FIELD_CLASSPTR)
        return _handleIndexOrPointer;

    return 0;
}

fieldtype_t SDKVariantT::GetFieldType() const
{
    return _fieldType;
}
