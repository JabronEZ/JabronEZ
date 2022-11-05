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

#ifndef JABRONEZ_WEAPON_IDENTIFIERS_H
#define JABRONEZ_WEAPON_IDENTIFIERS_H

enum CSWeaponID
{
    CSWeapon_NONE = 0,
    CSWeapon_P228,
    CSWeapon_GLOCK,
    CSWeapon_SCOUT,
    CSWeapon_HEGRENADE,
    CSWeapon_XM1014,
    CSWeapon_C4,
    CSWeapon_MAC10,
    CSWeapon_AUG,
    CSWeapon_SMOKEGRENADE,
    CSWeapon_ELITE,
    CSWeapon_FIVESEVEN,
    CSWeapon_UMP45,
    CSWeapon_SG550,
    CSWeapon_GALIL,
    CSWeapon_FAMAS,
    CSWeapon_USP,
    CSWeapon_AWP,
    CSWeapon_MP5NAVY,
    CSWeapon_M249,
    CSWeapon_M3,
    CSWeapon_M4A1,
    CSWeapon_TMP,
    CSWeapon_G3SG1,
    CSWeapon_FLASHBANG,
    CSWeapon_DEAGLE,
    CSWeapon_SG552,
    CSWeapon_AK47,
    CSWeapon_KNIFE,
    CSWeapon_P90,
    CSWeapon_SHIELD,
    CSWeapon_KEVLAR,
    CSWeapon_ASSAULTSUIT,
    CSWeapon_NIGHTVISION, //Anything below is CS:GO ONLY
    CSWeapon_GALILAR,
    CSWeapon_BIZON,
    CSWeapon_MAG7,
    CSWeapon_NEGEV,
    CSWeapon_SAWEDOFF,
    CSWeapon_TEC9,
    CSWeapon_TASER,
    CSWeapon_HKP2000,
    CSWeapon_MP7,
    CSWeapon_MP9,
    CSWeapon_NOVA,
    CSWeapon_P250,
    CSWeapon_SCAR17,
    CSWeapon_SCAR20,
    CSWeapon_SG556,
    CSWeapon_SSG08,
    CSWeapon_KNIFE_GG,
    CSWeapon_MOLOTOV,
    CSWeapon_DECOY,
    CSWeapon_INCGRENADE,
    CSWeapon_DEFUSER,
    CSWeapon_HEAVYASSAULTSUIT,
    //The rest are actual item definition indexes for CS:GO
    CSWeapon_CUTTERS = 56,
    CSWeapon_HEALTHSHOT = 57,
    CSWeapon_KNIFE_T = 59,
    CSWeapon_M4A1_SILENCER = 60,
    CSWeapon_USP_SILENCER = 61,
    CSWeapon_CZ75A = 63,
    CSWeapon_REVOLVER = 64,
    CSWeapon_TAGGRENADE = 68,
    CSWeapon_FISTS = 69,
    CSWeapon_BREACHCHARGE = 70,
    CSWeapon_TABLET = 72,
    CSWeapon_MELEE = 74,
    CSWeapon_AXE = 75,
    CSWeapon_HAMMER = 76,
    CSWeapon_SPANNER = 78,
    CSWeapon_KNIFE_GHOST = 80,
    CSWeapon_FIREBOMB = 81,
    CSWeapon_DIVERSION = 82,
    CSWeapon_FRAGGRENADE = 83,
    CSWeapon_SNOWBALL = 84,
    CSWeapon_BUMPMINE = 85,
    CSWeapon_MAX_WEAPONS_NO_KNIFES, // Max without the knife item defs, useful when treating all knives as a regular knife.
    CSWeapon_BAYONET = 500,
    CSWeapon_KNIFE_CLASSIC = 503,
    CSWeapon_KNIFE_FLIP = 505,
    CSWeapon_KNIFE_GUT = 506,
    CSWeapon_KNIFE_KARAMBIT = 507,
    CSWeapon_KNIFE_M9_BAYONET = 508,
    CSWeapon_KNIFE_TATICAL = 509,
    CSWeapon_KNIFE_FALCHION = 512,
    CSWeapon_KNIFE_SURVIVAL_BOWIE = 514,
    CSWeapon_KNIFE_BUTTERFLY = 515,
    CSWeapon_KNIFE_PUSH = 516,
    CSWeapon_KNIFE_CORD = 517,
    CSWeapon_KNIFE_CANIS = 518,
    CSWeapon_KNIFE_URSUS = 519,
    CSWeapon_KNIFE_GYPSY_JACKKNIFE = 520,
    CSWeapon_KNIFE_OUTDOOR = 521,
    CSWeapon_KNIFE_STILETTO = 522,
    CSWeapon_KNIFE_WIDOWMAKER = 523,
    CSWeapon_KNIFE_SKELETON = 525,
    CSWeapon_MAX_WEAPONS //THIS MUST BE LAST, EASY WAY TO CREATE LOOPS. When looping, do CS_IsValidWeaponID(i), to check.
};

enum ItemDefinitionIndex
{
    ItemDefinitionIndex_UNKNOWN = -1,
    ItemDefinitionIndex_FLASH = 43,
    ItemDefinitionIndex_HEGRENADE = 44,
    ItemDefinitionIndex_SMOKE = 45,
    ItemDefinitionIndex_MOLOTOV = 46,
    ItemDefinitionIndex_DECOY = 47,
    ItemDefinitionIndex_INCENDIARY = 48,
};

enum GrenadeType
{
    GrenadeType_UNKNOWN = -1,
    GrenadeType_FLASH,
    GrenadeType_HEGRENADE,
    GrenadeType_SMOKE,
    GrenadeType_MOLOTOV,
    GrenadeType_DECOY,
    GrenadeType_INCENDIARY,
    GrenadeType_COUNT
};

CSWeaponID GetCSWeaponIDFromItemDefinitionIndex(ItemDefinitionIndex itemDefinitionIndex);
ItemDefinitionIndex GetItemDefinitionIndexFromCSWeaponID(CSWeaponID csWeaponId);
GrenadeType GetGrenadeTypeFromCSWeaponID(CSWeaponID csWeaponId);
ItemDefinitionIndex GetItemDefinitionIndexFromGrenadeType(GrenadeType grenadeType);
GrenadeType GetNextGrenadeType(GrenadeType grenadeType);

const char *ChooseStringForGrenadeType(
        GrenadeType grenadeType,
        const char *heGrenadeString,
        const char *molotovString,
        const char *incendiaryString,
        const char *decoyString,
        const char *flashbangString,
        const char *smokeString);

#endif
