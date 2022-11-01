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

#ifndef _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_

#define SMEXT_CONF_NAME			"JabronEZ"
#define SMEXT_CONF_DESCRIPTION	"Practice modes for CS:GO that help you be less bad."
#define SMEXT_CONF_VERSION		"1.0.0.0"
#define SMEXT_CONF_AUTHOR		"JabronEZ Team"
#define SMEXT_CONF_URL			"http://www.sourcemod.net/"
#define SMEXT_CONF_LOGTAG		"JEZ"
#define SMEXT_CONF_LICENSE		"GPL"
#define SMEXT_CONF_DATESTRING	__DATE__

#define SMEXT_LINK(name) SDKExtension *g_pExtensionIface = name;
#define SMEXT_CONF_METAMOD

#define SMEXT_ENABLE_FORWARDSYS
#define SMEXT_ENABLE_HANDLESYS
#define SMEXT_ENABLE_PLAYERHELPERS
#define SMEXT_ENABLE_DBMANAGER
#define SMEXT_ENABLE_GAMECONF
#define SMEXT_ENABLE_MEMUTILS
#define SMEXT_ENABLE_GAMEHELPERS
#define SMEXT_ENABLE_TIMERSYS
#define SMEXT_ENABLE_THREADER
#define SMEXT_ENABLE_LIBSYS
#define SMEXT_ENABLE_MENUS
#define SMEXT_ENABLE_ADTFACTORY
#define SMEXT_ENABLE_PLUGINSYS
#define SMEXT_ENABLE_ADMINSYS
#define SMEXT_ENABLE_TEXTPARSERS
#define SMEXT_ENABLE_USERMSGS
#define SMEXT_ENABLE_TRANSLATOR
#define SMEXT_ENABLE_ROOTCONSOLEMENU

#endif