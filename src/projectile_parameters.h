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

#ifndef JABRONEZ_PROJECTILE_PARAMETERS_H
#define JABRONEZ_PROJECTILE_PARAMETERS_H

#include "smsdk_ext.h"

class ProjectileParameters
{
public:
    explicit ProjectileParameters(Vector origin, QAngle angle, Vector velocity, Vector angularImpulse);
    ~ProjectileParameters();
    ProjectileParameters(const ProjectileParameters &otherProjectileParameters);

    Vector GetOrigin() const { return _origin; }
    QAngle GetAngle() const { return _angle; }
    Vector GetVelocity() const { return _velocity; }
    Vector GetAngularImpulse() const { return _angularImpulse; }

private:
    Vector _origin;
    QAngle _angle;
    Vector _velocity;
    Vector _angularImpulse;
};

#endif
