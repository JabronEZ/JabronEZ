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

#include "weapon_identifiers.h"

CSWeaponID GetCSWeaponIDFromItemDefinitionIndex(ItemDefinitionIndex itemDefinitionIndex)
{
    switch (itemDefinitionIndex)
    {
        case ItemDefinitionIndex_DECOY:
            return CSWeapon_DECOY;
        case ItemDefinitionIndex_FLASH:
            return CSWeapon_FLASHBANG;
        case ItemDefinitionIndex_MOLOTOV:
            return CSWeapon_MOLOTOV;
        case ItemDefinitionIndex_INCENDIARY:
            return CSWeapon_INCGRENADE;
        case ItemDefinitionIndex_SMOKE:
            return CSWeapon_SMOKEGRENADE;
        case ItemDefinitionIndex_HEGRENADE:
            return CSWeapon_HEGRENADE;
    }

    return CSWeapon_NONE;
}

ItemDefinitionIndex GetItemDefinitionIndexFromCSWeaponID(CSWeaponID csWeaponId)
{
    switch (csWeaponId)
    {
        case CSWeapon_DECOY:
            return ItemDefinitionIndex_DECOY;
        case CSWeapon_FLASHBANG:
            return ItemDefinitionIndex_FLASH;
        case CSWeapon_MOLOTOV:
            return ItemDefinitionIndex_MOLOTOV;
        case CSWeapon_INCGRENADE:
            return ItemDefinitionIndex_INCENDIARY;
        case CSWeapon_SMOKEGRENADE:
            return ItemDefinitionIndex_SMOKE;
        case CSWeapon_HEGRENADE:
            return ItemDefinitionIndex_HEGRENADE;
    }

    return ItemDefinitionIndex_UNKNOWN;
}

GrenadeType GetGrenadeTypeFromCSWeaponID(CSWeaponID csWeaponId)
{
    switch (csWeaponId)
    {
        case CSWeapon_DECOY:
            return GrenadeType_DECOY;
        case CSWeapon_FLASHBANG:
            return GrenadeType_FLASH;
        case CSWeapon_MOLOTOV:
            return GrenadeType_MOLOTOV;
        case CSWeapon_INCGRENADE:
            return GrenadeType_INCENDIARY;
        case CSWeapon_SMOKEGRENADE:
            return GrenadeType_SMOKE;
        case CSWeapon_HEGRENADE:
            return GrenadeType_HEGRENADE;
    }

    return GrenadeType_UNKNOWN;
}

ItemDefinitionIndex GetItemDefinitionIndexFromGrenadeType(GrenadeType grenadeType)
{
    switch (grenadeType)
    {
        case GrenadeType_DECOY:
            return ItemDefinitionIndex_DECOY;
        case GrenadeType_FLASH:
            return ItemDefinitionIndex_FLASH;
        case GrenadeType_MOLOTOV:
            return ItemDefinitionIndex_MOLOTOV;
        case GrenadeType_INCENDIARY:
            return ItemDefinitionIndex_INCENDIARY;
        case GrenadeType_SMOKE:
            return ItemDefinitionIndex_SMOKE;
        case GrenadeType_HEGRENADE:
            return ItemDefinitionIndex_HEGRENADE;
    }

    return ItemDefinitionIndex_UNKNOWN;
}

GrenadeType GetNextGrenadeType(GrenadeType grenadeType)
{
    switch (grenadeType)
    {
        case GrenadeType_FLASH:
            return GrenadeType_SMOKE;

        case GrenadeType_SMOKE:
            return GrenadeType_HEGRENADE;

        case GrenadeType_HEGRENADE:
            return GrenadeType_DECOY;

        case GrenadeType_DECOY:
            return GrenadeType_MOLOTOV;

        case GrenadeType_MOLOTOV:
            return GrenadeType_INCENDIARY;
    }

    return GrenadeType_FLASH;
}


const char *ChooseStringForGrenadeType(
        GrenadeType grenadeType,
        const char *heGrenadeString,
        const char *molotovString,
        const char *incendiaryString,
        const char *decoyString,
        const char *flashbangString,
        const char *smokeString)
{
    switch (grenadeType)
    {
        case GrenadeType_INCENDIARY:
            return incendiaryString;

        case GrenadeType_DECOY:
            return decoyString;

        case GrenadeType_MOLOTOV:
            return molotovString;

        case GrenadeType_HEGRENADE:
            return heGrenadeString;

        case GrenadeType_SMOKE:
            return smokeString;

        case GrenadeType_FLASH:
            return flashbangString;
    }

    return nullptr;
}
