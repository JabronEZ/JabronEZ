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

#ifndef JABRONEZ_PROJECTILE_MODE_H
#define JABRONEZ_PROJECTILE_MODE_H

enum ProjectileMode
{
    ProjectileMode_ALL = 0, // allow anyone to flash me
    ProjectileMode_NONE, // don't allow anyone to flash me
    ProjectileMode_SELF, // only allow us to flash ourselves,
    ProjectileMode_GROUP,
    ProjectileMode_COUNT
};

ProjectileMode GetNextProjectileMode(ProjectileMode projectileMode);

#endif
