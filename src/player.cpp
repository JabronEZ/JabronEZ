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

Player::Player(int clientIndex, int userId, IGamePlayer *gamePlayer)
{
    _clientIndex = clientIndex;
    _userId = userId;
    _gamePlayer = gamePlayer;
    _grenadeSpots.clear();
}

Player::~Player()
{
    _gamePlayer = nullptr;
    _grenadeSpots.clear();
}

ProjectileMode Player::GetProjectileMode(CSWeaponID grenadeType) const
{
    switch (grenadeType)
    {
        case CSWeapon_FLASHBANG:
            return GetGrenadeFlashMode();
        case CSWeapon_SMOKEGRENADE:
            return GetGrenadeSmokeMode();
        case CSWeapon_DECOY:
            return GetGrenadeDecoyMode();
        case CSWeapon_MOLOTOV:
            return GetGrenadeMolotovMode();
        case CSWeapon_INCGRENADE:
            return GetGrenadeIncendiaryMode();
        case CSWeapon_HEGRENADE:
            return GetGrenadeHEGrenadeMode();
    }

    return ProjectileMode_ALL;
}
