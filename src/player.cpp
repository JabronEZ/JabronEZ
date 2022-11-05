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
#include "hooks.h"
#include "entity_utilities.h"

Player::Player(int clientIndex, int userId, IGamePlayer *gamePlayer)
{
    _clientIndex = clientIndex;
    _userId = userId;
    _gamePlayer = gamePlayer;
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
}

void Player::DoRemoveSpot()
{
}

void Player::DoTogglePlayback()
{
}

void Player::DoFastForward()
{
}

void Player::DoSwitchToGrenade()
{
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

    const char *translationPhrase = g_JabronEZ.GetTranslations()->ChooseTranslationPhraseForGrenadeType(
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
}

CBaseEntity *Player::GiveNamedItem(const char *entityName) const
{
    return Hook_Call_CCSPlayerGiveNamedItem(
            g_JabronEZ.GetEntityUtilities()->GetEntityByIndex(GetClientIndex(), true),
            entityName,
            0,
            nullptr,
            false,
            nullptr);
}
