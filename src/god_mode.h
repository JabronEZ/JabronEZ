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

#ifndef JABRONEZ_GOD_MODE_H
#define JABRONEZ_GOD_MODE_H

enum GodMode
{
    GodMode_OFF = 0, // godmode off
    GodMode_ON, // godmode on
    GodMode_SELFONLY, // only take damage from self/world
    GodMode_NADEONLY, // only take damage from grenades
    GodMode_GROUP, // only take damage from group
    GodMode_GROUPNADEONLY, // only take damage from group nades
    GodMode_COUNT,
};

#endif
