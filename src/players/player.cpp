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
#include "menu_manager.h"
#include "handle_playback_timer.h"
#include "grenade_trigger_playback_timer.h"
#include "grenade_goto_next_spot_or_finish_timer.h"
#include <dt_send.h>
#include <iplayerinfo.h>
#include "basehandle.h"
#include "vector.h"
#include <shareddefs.h>
#include "usercmd.h"
#include "filesystem.h"
#include "hooks.h"
#include "ccsplayer_slot_occupied_hook.h"
#include "particle_manager.h"
#include "remove_short_smoke_timer.h"

Player::Player(int clientIndex, int userId, IGamePlayer *gamePlayer, IGameHelpers *gameHelpers, ITimerSystem *timerSystem)
{
    _clientIndex = clientIndex;
    _userId = userId;
    _gamePlayer = gamePlayer;
    _gameHelpers = gameHelpers;
    _timerSystem = timerSystem;
    _grenadeSpots.clear();
}

Player::~Player()
{
    if (_grenadeTriggerPlaybackTimer != nullptr)
    {
        _grenadeTriggerPlaybackTimer->KillTimerSafely();
        _grenadeTriggerPlaybackTimer = nullptr;
    }

    if (_grenadeGotoNextSpotOrFinishTimer != nullptr)
    {
        _grenadeGotoNextSpotOrFinishTimer->KillTimerSafely();
        _grenadeGotoNextSpotOrFinishTimer = nullptr;
    }

    if (_grenadeHandlePlaybackTimer != nullptr)
    {
        _grenadeHandlePlaybackTimer->KillTimerSafely();
        _grenadeHandlePlaybackTimer = nullptr;
    }

    _gamePlayer = nullptr;
    _grenadeSpots.clear();
    _gameHelpers = nullptr;
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
    auto spots = GetGrenadeSpots();
    auto spotCount = spots.size();
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

    if (spotCount == 0)
    {
        char message[1024];

        g_JabronEZ.GetTranslations()->FormatTranslated(
                message,
                sizeof(message),
                "%T",
                2,
                nullptr,
                "Grenades must first configure one or more spots",
                &clientIndex);

        g_JabronEZ.GetHudUtilities()->PrintToChat(this, message);
        return;
    }

    auto playbackEnabled = GetGrenadePlaybackEnabled();
    SetGrenadePlaybackEnabled(!playbackEnabled);

    char message[1024];

    g_JabronEZ.GetTranslations()->FormatTranslated(
            message,
            sizeof(message),
            "%T",
            2,
            nullptr,
            playbackEnabled ? "Grenades playback disabled no longer tracking" : "Grenades playback enabled now tracking",
            &clientIndex);

    g_JabronEZ.GetHudUtilities()->PrintToChat(this, message);
}

void Player::DoFastForward()
{
    int clientIndex = GetClientIndex();

    if (GetGrenadePlaybackStarted() || GetGrenadeAwaitingDetonation())
    {
        FinishGrenadeTesterPlayback();

        char message[1024];

        g_JabronEZ.GetTranslations()->FormatTranslated(
                message,
                sizeof(message),
                "%T",
                2,
                nullptr,
                "Grenades playback skipped",
                &clientIndex);

        g_JabronEZ.GetHudUtilities()->PrintToChat(this, message);
    }
    else
    {
        char message[1024];

        g_JabronEZ.GetTranslations()->FormatTranslated(
                message,
                sizeof(message),
                "%T",
                2,
                nullptr,
                "Grenades may only fast forward during playback",
                &clientIndex);

        g_JabronEZ.GetHudUtilities()->PrintToChat(this, message);
    }
}

bool Player::IsAlive() const
{
    CBaseEntity *playerEntity = g_JabronEZ.GetEntityUtilities()->GetEntityByIndex(GetClientIndex(), true);

    if (playerEntity == nullptr)
        return false;

    static unsigned int lifeStateOffset = 0;

    if (lifeStateOffset == 0)
    {
        sm_sendprop_info_t sendpropInfo {};
        _gameHelpers->FindSendPropInfo("CBasePlayer", "m_lifeState", &sendpropInfo);

        if (sendpropInfo.prop == nullptr)
            return false;

        lifeStateOffset = sendpropInfo.actual_offset;
    }

    uint8_t lifeState = *((uint8_t *)playerEntity + lifeStateOffset);
    return lifeState == 0;
}

void Player::RespawnPlayer() const
{
    CBaseEntity *playerEntity = g_JabronEZ.GetEntityUtilities()->GetEntityByIndex(GetClientIndex(), true);

    if (playerEntity == nullptr)
        return;

    Callables_Call_CS_RespawnPlayer(playerEntity);
}

void Player::DoToggleGrenadeThrowTickRate()
{
    SetGrenadeThrowTickRate(GetGrenadeThrowTickRate() == GrenadeThrowTickRate_64
                                    ? GrenadeThrowTickRate_128
                                    : GrenadeThrowTickRate_64);

    int clientIndex = GetClientIndex();

    char message[1024];

    g_JabronEZ.GetTranslations()->FormatTranslated(
            message,
            sizeof(message),
            "%T",
            3,
            nullptr,
            "Grenades grenade throw tickrate changed",
            &clientIndex,
            g_JabronEZ.GetTranslations()->GetGrenadeThrowTickRateTranslationPhrase(GetGrenadeThrowTickRate()));

    g_JabronEZ.GetHudUtilities()->PrintToChat(this, message);
}

void Player::DoToggleShortGrenades()
{
    SetShortGrenades(!GetShortGrenades());

    int clientIndex = GetClientIndex();

    char message[1024];

    g_JabronEZ.GetTranslations()->FormatTranslated(
            message,
            sizeof(message),
            "%T",
            3,
            nullptr,
            "Grenades short grenades changed",
            &clientIndex,
            g_JabronEZ.GetTranslations()->GetShortGrenadesTranslationPhrase(GetShortGrenades()));

    g_JabronEZ.GetHudUtilities()->PrintToChat(this, message);
}

void Player::DoToggleGodMode()
{
}

void Player::DoToggleNoClip()
{
    auto newNoClip = !GetNoClip();
    SetNoClip(newNoClip);

    int clientIndex = GetClientIndex();

    char message[1024];

    g_JabronEZ.GetTranslations()->FormatTranslated(
            message,
            sizeof(message),
            "%T",
            2,
            nullptr,
            newNoClip ? "Grenades noclip enabled" : "Grenades noclip disabled",
            &clientIndex);

    g_JabronEZ.GetHudUtilities()->PrintToChat(this, message);
}

CBaseEntity *Player::GiveNamedItem(const char *entityName) const
{
    return Callables_Call_CCSPlayerGiveNamedItem(
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

    static unsigned int myWeaponsOffset = 0;
    static unsigned int myWeaponsDataTableSize = 0;

    if (myWeaponsOffset == 0)
    {
        sm_sendprop_info_t sendpropInfo {};
        _gameHelpers->FindSendPropInfo("CCSPlayer", "m_hMyWeapons", &sendpropInfo);

        if (sendpropInfo.prop == nullptr)
            return nullptr;

        SendTable *dataTable = sendpropInfo.prop->GetDataTable();

        if (dataTable == nullptr)
            return nullptr;

        myWeaponsOffset = sendpropInfo.actual_offset;
        myWeaponsDataTableSize = dataTable->GetNumProps();
    }

    for (size_t propIndex = 0; propIndex < myWeaponsDataTableSize; propIndex++)
    {
        auto *baseHandle = (CBaseHandle *)((uint8_t *)playerEntity + myWeaponsOffset + (propIndex * 4));

        if (baseHandle == nullptr)
            continue;

        CBaseEntity *handleEntity = g_JabronEZ.GetEntityUtilities()->GetEntityFromHandle(baseHandle);

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
    Callables_Call_CBasePlayerRemovePlayerItem(
            g_JabronEZ.GetEntityUtilities()->GetEntityByIndex(GetClientIndex(), true),
            weaponEntity);
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

    return Callables_Call_GetEyeAngles(playerEntity);
}

void Player::OnProjectileCreate(const Vector &origin, const QAngle &angle, const Vector &velocity, const Vector &angularImpulse, GrenadeType grenadeType)
{
    if(GetGrenadePlaybackStarted() || !GetGrenadePlaybackEnabled() || GetGrenadeAwaitingDetonation())
        return;

    SetGrenadeType(grenadeType);
    SetGrenadeAwaitingDetonation(true);
    RefreshGrenadesMenu();

    SetProjectileParameters(ProjectileParameters(origin, angle, velocity, angularImpulse));
    SetGrenadeThrowerSpot(Spot(GetAbsOrigin(), GetEyeAngles()));
}

void Player::OnProjectileCreated(
        const Vector &origin,
        const QAngle &angle,
        const Vector &velocity,
        const Vector &angularImpulse,
        GrenadeType grenadeType,
        CBaseEntity *projectileEntity)
{
}

bool Player::OnRunCmd(CUserCmd *command, IMoveHelper *moveHelper)
{
    if (command->weaponselect == 0)
        return true;

    auto weaponEntity = g_JabronEZ.GetEntityUtilities()->GetEntityByIndex(command->weaponselect, false);

    if (weaponEntity == nullptr)
        return true;

    auto weaponClassName = _gameHelpers->GetEntityClassname(weaponEntity);

    auto currentActiveWeapon = GetActiveWeapon();

    if (currentActiveWeapon == nullptr)
        return true;

    auto activeWeaponClassName = _gameHelpers->GetEntityClassname(currentActiveWeapon);
    auto allWeapons = GetAllWeapons();

    if (HandleSwitchGrenade(command, weaponClassName, activeWeaponClassName, allWeapons)
        || HandleSwitchPrimaryWeapon(command, weaponClassName, activeWeaponClassName, allWeapons)
        || HandleSwitchSecondaryWeapon(command, weaponClassName, activeWeaponClassName, allWeapons))
        return true;

    return true;
}

bool Player::HandleSwitchGrenade(
        CUserCmd *command,
        const char *weaponClassName,
        const char *activeWeaponClassName,
        const SourceHook::CVector<CBaseEntity*>& allWeapons)
{
    if (strcmp(weaponClassName, "weapon_incgrenade") == 0 && strcmp(activeWeaponClassName, "weapon_decoy") == 0)
    {
        auto molotovEntity = g_JabronEZ.GetEntityUtilities()->FindEntityInListByClassName(allWeapons, "weapon_molotov");

        if (molotovEntity != nullptr)
            command->weaponselect = g_JabronEZ.GetEntityUtilities()->GetIndexByEntity(molotovEntity);

        return true;
    }

    if (strcmp(weaponClassName, "weapon_hegrenade") == 0 && strcmp(activeWeaponClassName, "weapon_molotov") == 0)
    {
        auto incendiaryEntity = g_JabronEZ.GetEntityUtilities()->FindEntityInListByClassName(allWeapons, "weapon_incgrenade");

        if (incendiaryEntity != nullptr)
            command->weaponselect = g_JabronEZ.GetEntityUtilities()->GetIndexByEntity(incendiaryEntity);

        return true;
    }

    if (strcmp(weaponClassName, "weapon_decoy") == 0 && strcmp(activeWeaponClassName, "weapon_incgrenade") == 0)
    {
        auto incendiaryEntity = g_JabronEZ.GetEntityUtilities()->FindEntityInListByClassName(allWeapons, "weapon_molotov");

        if (incendiaryEntity != nullptr)
            command->weaponselect = g_JabronEZ.GetEntityUtilities()->GetIndexByEntity(incendiaryEntity);

        return true;
    }

    return false;
}

bool Player::HandleSwitchPrimaryWeapon(
        CUserCmd *command,
        const char *weaponClassName,
        const char *activeWeaponClassName,
        const SourceHook::CVector<CBaseEntity*>& allWeapons)
{
    bool isAttemptingToSwitchToPrimaryWeapon = g_JabronEZ.GetEntityUtilities()->IsPrimaryWeaponClassName(weaponClassName);
    bool isAlreadyHoldingPrimaryWeapon = g_JabronEZ.GetEntityUtilities()->IsPrimaryWeaponClassName(activeWeaponClassName);

    if (!isAttemptingToSwitchToPrimaryWeapon || !isAlreadyHoldingPrimaryWeapon)
        return false;

    SourceHook::CVector<CBaseEntity*> primaryWeapons;
    primaryWeapons.clear();

    size_t currentWeaponIndex = 0;
    for (auto weaponEntity : allWeapons)
    {
        if (weaponEntity == nullptr)
            continue;

        const char *checkWeaponClassName = _gameHelpers->GetEntityClassname(weaponEntity);

        if (g_JabronEZ.GetEntityUtilities()->IsPrimaryWeaponClassName(checkWeaponClassName))
        {
            size_t nextIndex = primaryWeapons.size();
            primaryWeapons.push_back(weaponEntity);

            if (strcmp(checkWeaponClassName, activeWeaponClassName) == 0)
                currentWeaponIndex = nextIndex;
        }
    }

    if (primaryWeapons.size() <= 1)
        return false;

    // Select the next primary weapon index, wrapping around if we hit the end.
    auto nextWeaponIndex = currentWeaponIndex + 1;

    if (nextWeaponIndex >= primaryWeapons.size())
        nextWeaponIndex = 0;

    auto desiredWeaponEntity = primaryWeapons.at(nextWeaponIndex);
    command->weaponselect = g_JabronEZ.GetEntityUtilities()->GetIndexByEntity(desiredWeaponEntity);
    return true;
}

bool Player::HandleSwitchSecondaryWeapon(
        CUserCmd *command,
        const char *weaponClassName,
        const char *activeWeaponClassName,
        const SourceHook::CVector<CBaseEntity*>& allWeapons)
{
    bool isAttemptingToSwitchToSecondaryWeapon = g_JabronEZ.GetEntityUtilities()->IsSecondaryWeaponClassName(weaponClassName);
    bool isAlreadyHoldingSecondaryWeapon = g_JabronEZ.GetEntityUtilities()->IsSecondaryWeaponClassName(activeWeaponClassName);
    
    if (!isAttemptingToSwitchToSecondaryWeapon || !isAlreadyHoldingSecondaryWeapon)
        return false;

    SourceHook::CVector<CBaseEntity*> secondaryWeapons;
    secondaryWeapons.clear();

    size_t currentWeaponIndex = 0;
    for (auto weaponEntity : allWeapons)
    {
        if (weaponEntity == nullptr)
            continue;

        const char *checkWeaponClassName = _gameHelpers->GetEntityClassname(weaponEntity);

        if (g_JabronEZ.GetEntityUtilities()->IsSecondaryWeaponClassName(checkWeaponClassName))
        {
            size_t nextIndex = secondaryWeapons.size();
            secondaryWeapons.push_back(weaponEntity);

            if (strcmp(checkWeaponClassName, activeWeaponClassName) == 0)
                currentWeaponIndex = nextIndex;
        }
    }

    if (secondaryWeapons.size() <= 1)
        return false;

    // Select the next secondary weapon index, wrapping around if we hit the end.
    auto nextWeaponIndex = currentWeaponIndex + 1;

    if (nextWeaponIndex >= secondaryWeapons.size())
        nextWeaponIndex = 0;

    auto desiredWeaponEntity = secondaryWeapons.at(nextWeaponIndex);
    command->weaponselect = g_JabronEZ.GetEntityUtilities()->GetIndexByEntity(desiredWeaponEntity);
    return true;
}

void Player::FinishGrenadeTesterPlayback(bool restorePosition)
{
    if (_grenadeTriggerPlaybackTimer != nullptr)
    {
        _grenadeTriggerPlaybackTimer->KillTimerSafely();
        _grenadeTriggerPlaybackTimer = nullptr;
    }

    if (_grenadeGotoNextSpotOrFinishTimer != nullptr)
    {
        _grenadeGotoNextSpotOrFinishTimer->KillTimerSafely();
        _grenadeGotoNextSpotOrFinishTimer = nullptr;
    }

    if (_grenadeHandlePlaybackTimer != nullptr)
    {
        _grenadeHandlePlaybackTimer->KillTimerSafely();
        _grenadeHandlePlaybackTimer = nullptr;
    }

    SetGrenadeCurrentSpotIndex(0);
    SetGrenadePlaybackStarted(false);
    SetGrenadeAwaitingDetonation(false);

    RefreshGrenadesMenu();

    if(!restorePosition)
        return;

    if(!IsAlive())
        RespawnPlayer();

    auto playerEntity = g_JabronEZ.GetEntityUtilities()->GetEntityByIndex(GetClientIndex(), true);

    if (playerEntity == nullptr)
        return;

    auto throwerSpot = GetGrenadeThrowerSpot();

    auto origin = throwerSpot.GetOrigin();
    auto eyeAngles = throwerSpot.GetEyeAngles();
    auto velocity = Vector(0.0f, 0.0f, 0.0f);

    Callables_Call_Teleport(playerEntity, &origin, &eyeAngles, &velocity, true);
}

void Player::StartGrenadeTesterPlayback()
{
    SetGrenadeCurrentSpotIndex(0);
    SetGrenadePlaybackStarting(false);
    SetGrenadePlaybackStarted(true);

    HandleGrenadeTesterPlayback();
}

void Player::RefreshGrenadesMenu()
{
    if (!IsGrenadeMenuOpen())
        return;

    auto pageNumber = GetGrenadeMenuPage();

    if(pageNumber == 0)
        pageNumber = 1;

    g_JabronEZ.GetMenuManager()->OpenMenu(this, pageNumber);
}

void Player::HandleGrenadeTesterPlayback()
{
    CBaseEntity *playerEntity = g_JabronEZ.GetEntityUtilities()->GetEntityByIndex(GetClientIndex(), true);

    if (playerEntity == nullptr)
        return;

    SetGrenadeTossedGrenade(0);

    if (!IsOnActualTeam())
        return;

    if (!IsAlive())
        RespawnPlayer();

    RefreshGrenadesMenu();

    auto spotIndex = GetGrenadeCurrentSpotIndex();
    auto spots = GetGrenadeSpots();
    auto spot = spots.at(spotIndex);

    auto spotOrigin = spot.GetOrigin();
    auto spotAngles = spot.GetEyeAngles();
    auto spotVelocity = Vector(0.0f, 0.0f, 0.0f);
    Callables_Call_Teleport(playerEntity, &spotOrigin, &spotAngles, &spotVelocity, true);

    auto existingTimer = GetGrenadeHandlePlaybackTimer();

    if (existingTimer != nullptr)
    {
        existingTimer->KillTimerSafely();
        SetGrenadeHandlePlaybackTimer(nullptr);
    }

    SetGrenadeHandlePlaybackTimer(new HandlePlaybackTimer(this, _timerSystem, _gameHelpers));
}

bool Player::IsOnActualTeam() const
{
    auto gamePlayer = GetGamePlayer();

    if (gamePlayer == nullptr)
        return false;

    auto playerInfo = gamePlayer->GetPlayerInfo();

    if (playerInfo == nullptr)
        return false;

    auto teamIndex = playerInfo->GetTeamIndex();

    return teamIndex == 2 || teamIndex == 3;
}

float GetDelayPostDetonation(GrenadeType grenadeType)
{
    if(grenadeType == GrenadeType_MOLOTOV || grenadeType == GrenadeType_INCENDIARY)
        return 9.0f;

    if(grenadeType == GrenadeType_DECOY)
        return 7.0f;

    return 3.0f;
}

void Player::OnGrenadeDetonationEvent(GrenadeType grenadeType, cell_t projectileReference)
{
    // FIXME: We have a hook for `CSmokeGrenadeProjectileDetonate`, does it make sense to move this logic there to prevent needing to jump back and forth?
    if (grenadeType == GrenadeType_SMOKE && GetShortGrenades())
    {
        auto projectileEntity = _gameHelpers->ReferenceToEntity(projectileReference);
        if (projectileEntity != nullptr)
        {
            auto projectileOrigin = g_JabronEZ.GetEntityUtilities()->GetEntityAbsOrigin(projectileEntity);
            g_JabronEZ.GetParticleManager()->CreateShortSmoke(projectileOrigin, QAngle(0, 0, 0));
            new RemoveShortSmokeTimer(projectileEntity, _timerSystem, _gameHelpers);
        }
    }

    if (!GetGrenadePlaybackEnabled())
        return;

    auto postDetonationDelay = GetDelayPostDetonation(grenadeType);

    if (GetGrenadeAwaitingDetonation())
    {
        auto selectedGrenadeType = GetGrenadeType();

        if (selectedGrenadeType != grenadeType)
            return;

        SetGrenadeAwaitingDetonation(false);
        SetGrenadePlaybackStarting(true);

        // if(p_GrenadeType == CSWeapon_SMOKEGRENADE)
        //     MarkShortSmoke(p_ProjectileIndex);
        //
        // if(p_GrenadeType == CSWeapon_DECOY)
        //     MarkShortDecoy(p_ProjectileIndex);

        auto existingTriggerPlaybackTimer = GetGrenadeTriggerPlaybackTimer();

        if (existingTriggerPlaybackTimer != nullptr)
        {
            existingTriggerPlaybackTimer->KillTimerSafely();
            SetGrenadeTriggerPlaybackTimer(nullptr);
        }

        SetGrenadeTriggerPlaybackTimer(new GrenadeTriggerPlaybackTimer(this, postDetonationDelay, _timerSystem, _gameHelpers));
        return;
    }

    if (GetGrenadePlaybackStarted())
    {
        auto tossedGrenadeReference = GetGrenadeTossedGrenade();

        if (tossedGrenadeReference != projectileReference)
            return;

        SetGrenadeTossedGrenade(0);

        auto existingGotoNextSpotTimer = GetGrenadeGotoNextSpotOrFinishTimer();

        if (existingGotoNextSpotTimer != nullptr)
        {
            existingGotoNextSpotTimer->KillTimerSafely();
            SetGrenadeGotoNextSpotOrFinishTimer(nullptr);
        }

        SetGrenadeGotoNextSpotOrFinishTimer(new GrenadeGotoNextSpotOrFinishTimer(this, postDetonationDelay, _timerSystem, _gameHelpers));
        return;
    }
}

void Player::GotoNextSpotOrFinishPlayback()
{
    if (!IsOnActualTeam())
        return;

    auto currentSpotIndex = GetGrenadeCurrentSpotIndex();
    auto spots = GetGrenadeSpots();
    auto spotCount = spots.size();

    if (currentSpotIndex + 1 >= spotCount)
        FinishGrenadeTesterPlayback();
    else
    {
        SetGrenadeCurrentSpotIndex(currentSpotIndex + 1);
        HandleGrenadeTesterPlayback();
    }
}

int Player::OnCanAcquire(void *econItemView, int type, int originalResult)
{
    void *weaponData = Callables_Call_CEconItemViewGetCCSWeaponData(econItemView);

    if (weaponData == nullptr)
        return originalResult;

    auto className = *(const char**)((uint8_t*)weaponData + Callables_Offset_CCSWeaponDataClassName);
    auto isFireGrenade = strcmp(className, "weapon_molotov") == 0 || strcmp(className, "weapon_incgrenade") == 0;

    // Allow players to pick up a molotov/incendiary even if they already have the other one.
    if (isFireGrenade && originalResult == 2)
        return 0;

    return originalResult;
}

void Player::OnBumpWeapon(CBaseEntity *weaponEntity)
{
    _isBumpingWeapon = true;
}

void Player::OnBumpWeaponPost(CBaseEntity *weaponEntity)
{
    _isBumpingWeapon = false;
}

CheckSlotOccupiedResult Player::OnCheckSlotOccupied(CBaseEntity *weaponEntity) const
{
    if (_isBumpingWeapon)
        return CheckSlotOccupiedResult::NotOccupied;

    return CheckSlotOccupiedResult::UseOriginal;
}

bool Player::OnDropWeapon(CBaseEntity *weaponEntity) const
{
    if (_isBumpingWeapon)
        return false;

    return true;
}

void Player::GiveAllGrenades() const
{
    GiveNamedItem("weapon_flashbang");
    GiveNamedItem("weapon_molotov");
    GiveNamedItem("weapon_incgrenade");
    GiveNamedItem("weapon_decoy");
    GiveNamedItem("weapon_smokegrenade");
    GiveNamedItem("weapon_hegrenade");
}

CBaseEntity *Player::GetEntity() const
{
    return g_JabronEZ.GetEntityUtilities()->GetEntityByIndex(GetClientIndex(), true);
}

CBaseEntity *Player::GetActiveWeapon() const
{
    auto playerEntity = GetEntity();

    if (playerEntity == nullptr)
        return nullptr;

    static unsigned int activeWeaponOffset = 0;

    if (activeWeaponOffset == 0)
    {
        sm_sendprop_info_t sendpropInfo {};
        _gameHelpers->FindSendPropInfo("CCSPlayer", "m_hActiveWeapon", &sendpropInfo);

        if (sendpropInfo.prop == nullptr)
            return nullptr;

        activeWeaponOffset = sendpropInfo.actual_offset;
    }

    auto *weaponHandle = (CBaseHandle *)((uint8_t *)playerEntity + activeWeaponOffset);
    return g_JabronEZ.GetEntityUtilities()->GetEntityFromHandle(weaponHandle);
}

SourceHook::CVector<CBaseEntity *> Player::GetAllWeapons() const
{
    auto playerEntity = GetEntity();

    if (playerEntity == nullptr)
        return {};

    static unsigned int myWeaponsOffset = 0;
    static unsigned int myWeaponsDataTableSize = 0;

    if (myWeaponsOffset == 0)
    {
        sm_sendprop_info_t sendpropInfo{};
        _gameHelpers->FindSendPropInfo("CCSPlayer", "m_hMyWeapons", &sendpropInfo);

        if (sendpropInfo.prop == nullptr)
            return {};

        auto dataTable = sendpropInfo.prop->GetDataTable();

        if (dataTable == nullptr)
            return {};

        myWeaponsOffset = sendpropInfo.actual_offset;
        myWeaponsDataTableSize = dataTable->GetNumProps();
    }

    auto result = SourceHook::CVector<CBaseEntity *>();

    for (size_t dataTableIndex = 0; dataTableIndex < myWeaponsDataTableSize; dataTableIndex++)
    {
        auto *weaponHandle = (CBaseHandle *)((uint8_t *)playerEntity + myWeaponsOffset + (dataTableIndex * 4));

        if (weaponHandle == nullptr)
            continue;

        CBaseEntity *weaponEntity = g_JabronEZ.GetEntityUtilities()->GetEntityFromHandle(weaponHandle);

        if (weaponEntity == nullptr)
            continue;

        result.push_back(weaponEntity);
    }

    return result;
}

unsigned int Player::GetMoveTypeOffset() const
{
    static unsigned int moveTypeOffset = 0;

    if (moveTypeOffset != 0)
        return moveTypeOffset;

    auto playerEntity = GetEntity();

    if (playerEntity == nullptr)
        return 0;

    auto dataMap = _gameHelpers->GetDataMap(playerEntity);

    if (dataMap == nullptr)
        return 0;

    sm_datatable_info_t dataTableInfo;

    if (!_gameHelpers->FindDataMapInfo(dataMap, "m_MoveType", &dataTableInfo))
        return 0;

    moveTypeOffset = dataTableInfo.actual_offset;
    return moveTypeOffset;
}

bool Player::GetNoClip() const
{
    if (!IsAlive())
        return false;

    auto playerEntity = GetEntity();

    if (playerEntity == nullptr)
        return false;

    unsigned int moveTypeOffset = GetMoveTypeOffset();

    if (moveTypeOffset == 0)
        return false;

    auto currentMoveType = *((uint8_t*)playerEntity + moveTypeOffset);
    return currentMoveType == MOVETYPE_NOCLIP;
}

void Player::SetNoClip(bool noClip)
{
    if (!IsAlive())
        return;

    unsigned int moveTypeOffset = GetMoveTypeOffset();

    if (moveTypeOffset == 0)
        return;

    auto playerEntity = GetEntity();

    if (playerEntity == nullptr)
        return;

    if (noClip)
    {
        *((uint8_t*)playerEntity + moveTypeOffset) = MOVETYPE_NOCLIP;
        return;
    }

    auto currentMoveType = *((uint8_t*)playerEntity + moveTypeOffset);

    if (currentMoveType == MOVETYPE_NOCLIP)
        *((uint8_t*)playerEntity + moveTypeOffset) = MOVETYPE_WALK;
}

bool Player::IsValid() const
{
    if (_gamePlayer == nullptr)
        return false;

    if (!_gamePlayer->IsConnected()
        || !_gamePlayer->IsInGame()
        || _gamePlayer->IsFakeClient()
        || _gamePlayer->IsSourceTV()
        || _gamePlayer->IsReplay())
        return false;

    return true;
}
