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

#include "player.h"
#include "spot.h"
#include "extension.h"
#include "translations.h"
#include "hud_utilities.h"
#include "callables.h"
#include "entity_utilities.h"
#include "console_manager.h"
#include <dt_send.h>
#include <iplayerinfo.h>

Player::Player(int clientIndex, int userId, IGamePlayer *gamePlayer, IGameHelpers *gameHelpers)
{
    _clientIndex = clientIndex;
    _userId = userId;
    _gamePlayer = gamePlayer;
    _gameHelpers = gameHelpers;
    _grenadeSpots.clear();

    for (int grenadeTypeIndex = GrenadeType_FLASH; grenadeTypeIndex < GrenadeType_COUNT; grenadeTypeIndex++)
    {
        auto grenadeType = static_cast<GrenadeType>(grenadeTypeIndex);
        _grenadeMode[grenadeType] = ProjectileMode_ALL;
    }
}

Player::~Player()
{
    _gamePlayer = nullptr;
    _grenadeSpots.clear();
    _gameHelpers = nullptr;
}

ProjectileMode Player::GetProjectileMode(GrenadeType grenadeType) const
{
    if (grenadeType == GrenadeType_UNKNOWN)
        return ProjectileMode_ALL;

    return _grenadeMode[grenadeType];
}

void Player::SetProjectileMode(GrenadeType grenadeType, ProjectileMode projectileMode)
{
    if (grenadeType == GrenadeType_UNKNOWN)
        return;

    _grenadeMode[grenadeType] = projectileMode;
}

void Player::DoAddSpot()
{
    int clientIndex = GetClientIndex();

    if(GetGrenadePlaybackStarted() || GetGrenadeAwaitingDetonation())
    {
        char message[1024];

        g_JabronEZ.GetTranslations()->FormatTranslated(
                message,
                sizeof(message),
                "%T",
                2,
                nullptr,
                "Grenades may not add spot during playback",
                &clientIndex);

        g_JabronEZ.GetHudUtilities()->PrintToChat(this, message);
        return;
    }

    auto origin = GetAbsOrigin();
    auto eyeAngles = GetEyeAngles();

    auto spot = Spot(origin, eyeAngles);
    _grenadeSpots.push_back(spot);

    char message[1024];

    g_JabronEZ.GetTranslations()->FormatTranslated(
            message,
            sizeof(message),
            "%T",
            2,
            nullptr,
            "Grenades spot added",
            &clientIndex);

    g_JabronEZ.GetHudUtilities()->PrintToChat(this, message);
}

void Player::DoRemoveSpot()
{
    auto spotCount = _grenadeSpots.size();
    int clientIndex = GetClientIndex();

    if (GetGrenadePlaybackStarted() || GetGrenadeAwaitingDetonation())
    {
        char message[1024];

        g_JabronEZ.GetTranslations()->FormatTranslated(
                message,
                sizeof(message),
                "%T",
                2,
                nullptr,
                "Grenades may not remove spot during playback",
                &clientIndex);

        g_JabronEZ.GetHudUtilities()->PrintToChat(this, message);
        return;
    }

    size_t closestSpotIndex;
    auto closestSpot = GetClosestGrenadeSpot(&closestSpotIndex);

    if (spotCount == 0 || closestSpotIndex >= spotCount)
    {
        char message[1024];

        g_JabronEZ.GetTranslations()->FormatTranslated(
                message,
                sizeof(message),
                "%T",
                2,
                nullptr,
                "Grenades no spots to remove",
                &clientIndex);

        g_JabronEZ.GetHudUtilities()->PrintToChat(this, message);
        return;
    }

    _grenadeSpots.erase(_grenadeSpots.iterAt(closestSpotIndex));

    if (_grenadeSpots.empty())
        SetGrenadePlaybackEnabled(false);

    char message[1024];

    g_JabronEZ.GetTranslations()->FormatTranslated(
            message,
            sizeof(message),
            "%T",
            2,
            nullptr,
            "Grenades spot removed",
            &clientIndex);

    g_JabronEZ.GetHudUtilities()->PrintToChat(this, message);
}

Spot Player::GetClosestGrenadeSpot(size_t *outSpotIndex) const
{
    auto origin = GetAbsOrigin();
    auto spotCount = _grenadeSpots.size();
    Spot closestSpot = Spot(Vector(0.0f, 0.0f, 0.0f), QAngle(0.0f, 0.0f, 0.0f));
    float shortestDistance = -1.0f;
    size_t closestSpotIndex = spotCount;

    for(size_t spotIndex = 0; spotIndex < spotCount; spotIndex++)
    {
        auto spot = _grenadeSpots.at(spotIndex);
        auto distance = spot.GetOrigin().DistTo(origin);

        if(shortestDistance < 0 || distance < shortestDistance)
        {
            shortestDistance = distance;
            closestSpot = spot;
            closestSpotIndex = spotIndex;
        }
    }

    if (outSpotIndex != nullptr)
        *outSpotIndex = closestSpotIndex;

    return closestSpot;
}

void Player::DoTogglePlayback()
{
}

void Player::DoFastForward()
{
}

bool Player::IsAlive() const
{
    CBaseEntity *playerEntity = g_JabronEZ.GetEntityUtilities()->GetEntityByIndex(GetClientIndex(), true);

    if (playerEntity == nullptr)
        return false;

    sm_sendprop_info_t sendpropInfo {};
    _gameHelpers->FindSendPropInfo("CBasePlayer", "m_lifeState", &sendpropInfo);

    if (sendpropInfo.prop == nullptr)
        return false;

    uint8_t lifeState = *((uint8_t *)playerEntity + sendpropInfo.actual_offset);
    return lifeState == 0;
}

void Player::RespawnPlayer()
{
    CBaseEntity *playerEntity = g_JabronEZ.GetEntityUtilities()->GetEntityByIndex(GetClientIndex(), true);

    if (playerEntity == nullptr)
        return;

    Callables_Call_CS_RespawnPlayer(playerEntity);
}

void Player::DoSwitchToGrenade()
{
    int clientIndex = GetClientIndex();

    if(!IsAlive())
    {
        char message[1024];

        g_JabronEZ.GetTranslations()->FormatTranslated(
                message,
                sizeof(message),
                "%T",
                2,
                nullptr,
                "Grenades must be alive to do this",
                &clientIndex);

        g_JabronEZ.GetHudUtilities()->PrintToChat(this, message);
        return;
    }

    SwitchToCurrentGrenadeType();

    char message[1024];

    g_JabronEZ.GetTranslations()->FormatTranslated(
            message,
            sizeof(message),
            "%T",
            2,
            nullptr,
            "Grenades switched to grenade",
            &clientIndex);

    g_JabronEZ.GetHudUtilities()->PrintToChat(this, message);
}

void Player::DoToggleGodMode()
{
}

void Player::DoToggleNoClip()
{
}

void Player::DoTogglePlayerMode()
{
}

void Player::DoToggleProjectileMode(GrenadeType grenadeType)
{
    if (grenadeType == GrenadeType_UNKNOWN)
        return;

    auto nextProjectileMode = GetNextProjectileMode(GetProjectileMode(grenadeType));

    char message[1024];
    int clientIndex = GetClientIndex();

    const char *translationPhrase = ChooseStringForGrenadeType(
            grenadeType,
            "Grenades HE grenade mode changed",
            "Grenades molotov mode changed",
            "Grenades incendiary mode changed",
            "Grenades decoy mode changed",
            "Grenades flash mode changed",
            "Grenades smoke mode changed");

    if (translationPhrase == nullptr)
        return;

    g_JabronEZ.GetTranslations()->FormatTranslated(
            message,
            sizeof(message),
            "%T",
            3,
            nullptr,
            translationPhrase,
            &clientIndex,
            g_JabronEZ.GetTranslations()->GetProjectileModeTranslationPhrase(nextProjectileMode));

    g_JabronEZ.GetHudUtilities()->PrintToChat(this, message);
    SetProjectileMode(grenadeType, nextProjectileMode);
}

void Player::DoToggleGrenadeType()
{
    int clientIndex = GetClientIndex();

    if (GetGrenadePlaybackStarted() || GetGrenadeAwaitingDetonation())
    {
        char message[1024];

        g_JabronEZ.GetTranslations()->FormatTranslated(
                message,
                sizeof(message),
                "%T",
                2,
                nullptr,
                "Grenades may not change this during playback",
                &clientIndex);

        g_JabronEZ.GetHudUtilities()->PrintToChat(this, message);
        return;
    }

    auto nextGrenadeType = GetNextGrenadeType(GetGrenadeType());
    SetGrenadeType(nextGrenadeType);

    // This logic deals with the fact that a player can only hold a molotov OR incendiary grenade.
    // If they want to switch to a molotov, but they already have an incendiary grenade in their inventory,
    // simply remove the incendiary grenade and give them a molotov (and vice versa).
    auto molotovWeapon = FindWeapon("weapon_molotov");
    auto incendiaryWeapon = FindWeapon("weapon_incgrenade");

    if (nextGrenadeType == GrenadeType_MOLOTOV && molotovWeapon == nullptr)
    {
        if (incendiaryWeapon != nullptr)
            RemoveWeapon(incendiaryWeapon);

        GiveNamedItem("weapon_molotov");
    }
    else if (nextGrenadeType == GrenadeType_INCENDIARY && incendiaryWeapon == nullptr)
    {
        if (molotovWeapon != nullptr)
            RemoveWeapon(molotovWeapon);

        GiveNamedItem("weapon_incgrenade");
    }

    char message[1024];

    g_JabronEZ.GetTranslations()->FormatTranslated(
            message,
            sizeof(message),
            "%T",
            3,
            nullptr,
            "Grenades type changed",
            &clientIndex,
            g_JabronEZ.GetTranslations()->GetGrenadeTranslationPhrase(nextGrenadeType));

    g_JabronEZ.GetHudUtilities()->PrintToChat(this, message);

    SwitchToCurrentGrenadeType();
}

CBaseEntity *Player::GiveNamedItem(const char *entityName) const
{
    return Virtual_Callables_Call_CCSPlayerGiveNamedItem(
            g_JabronEZ.GetEntityUtilities()->GetEntityByIndex(GetClientIndex(), true),
            entityName,
            0,
            nullptr,
            false,
            nullptr);
}

CBaseEntity *Player::FindWeapon(const char *entityName) const
{
    CBaseEntity *playerEntity = g_JabronEZ.GetEntityUtilities()->GetEntityByIndex(GetClientIndex(), true);

    if (playerEntity == nullptr)
        return nullptr;

    sm_sendprop_info_t sendpropInfo {};
    _gameHelpers->FindSendPropInfo("CCSPlayer", "m_hMyWeapons", &sendpropInfo);

    if (sendpropInfo.prop == nullptr)
        return nullptr;

    SendTable *dataTable = sendpropInfo.prop->GetDataTable();

    if (dataTable == nullptr)
        return nullptr;

    size_t offset = sendpropInfo.actual_offset;
    size_t numberOfProps = dataTable->GetNumProps();

    for (size_t propIndex = 0; propIndex < numberOfProps; propIndex++)
    {
        auto property = dataTable->GetProp((int)propIndex);
        auto propertyActualOffset = offset + property->GetOffset();

        auto *baseHandle = (CBaseHandle *)((uint8_t *)playerEntity + propertyActualOffset);

        if (baseHandle == nullptr)
            continue;

        CBaseEntity *handleEntity = _gameHelpers->ReferenceToEntity(baseHandle->GetEntryIndex());

        if (handleEntity == nullptr)
            continue;

        auto weaponClassName = _gameHelpers->GetEntityClassname(handleEntity);

        if (strcmp(weaponClassName, entityName) == 0)
            return handleEntity;
    }

    return nullptr;
}

void Player::RemoveWeapon(CBaseEntity *weaponEntity) const
{
    Virtual_Callables_Call_CBasePlayerRemovePlayerItem(
            g_JabronEZ.GetEntityUtilities()->GetEntityByIndex(GetClientIndex(), true),
            weaponEntity);
}

void Player::SwitchToCurrentGrenadeType() const
{
    char useCommand[512];
    snprintf(
            useCommand,
            sizeof(useCommand),
            "use %s\n",
            ChooseStringForGrenadeType(
                    GetGrenadeType(),
                    "weapon_hegrenade",
                    "weapon_molotov",
                    "weapon_incgrenade",
                    "weapon_decoy",
                    "weapon_flashbang",
                    "weapon_smokegrenade"));

    g_JabronEZ.GetConsoleManager()->SendClientCommand(GetGamePlayer()->GetEdict(), useCommand);
}

Vector Player::GetAbsOrigin() const
{
    IPlayerInfo *playerInfo = GetGamePlayer()->GetPlayerInfo();

    if (playerInfo == nullptr)
        return { 0.0f, 0.0f, 0.0f };

    return playerInfo->GetAbsOrigin();
}

QAngle Player::GetEyeAngles() const
{
    CBaseEntity *playerEntity = g_JabronEZ.GetEntityUtilities()->GetEntityByIndex(GetClientIndex(), true);

    if (playerEntity == nullptr)
        return { 0.0f, 0.0f, 0.0f };

    return Virtual_Callables_Call_GetEyeAngles(playerEntity);
}