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

#include "spot.h"

Spot::Spot()
{
    _origin = Vector(0, 0, 0);
    _eyeAngles = QAngle(0, 0, 0);
}

Spot::Spot(Vector origin, QAngle eyeAngles)
{
    _origin = origin;
    _eyeAngles = eyeAngles;
}

Spot::~Spot() = default;

Spot::Spot(const Spot &otherSpot)
{
    _origin = otherSpot._origin;
    _eyeAngles = otherSpot._eyeAngles;
}