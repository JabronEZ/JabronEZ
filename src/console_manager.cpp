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

#include "console_manager.h"
#include "extension.h"
#include "hooks.h"
#include "menu_manager.h"
#include "player.h"
#include "player_manager.h"
#include "smsdk_ext.h"

SH_DECL_HOOK2_void(IServerGameClients, ClientCommand, SH_NOATTRIB, 0, edict_t *, const CCommand &);

ConsoleManager::ConsoleManager(IVEngineServer *engineServer)
{
    _engineServer = engineServer;
    _cvarInterface = nullptr;
    _serverClients = nullptr;
}

bool ConsoleManager::RegisterConCommandBase(ConCommandBase *conCommandBase)
{
    return META_REGCVAR(conCommandBase);
}

bool ConsoleManager::Init(ISmmAPI *ismm, char *error, size_t maxlen)
{
    // In order for us to be able to register console commands and variables, we must set g_pCVar, call ConVar_Register,
    // and execute META_REGCVAR inside RegisterConCommandBase. Missing any of these things will cause new commands to fail
    // without any particular warning/error.
    // For more information, see: https://wiki.alliedmods.net/Porting_To_Orange_Box
    GET_V_IFACE_CURRENT(GetEngineFactory, _cvarInterface, ICvar, CVAR_INTERFACE_VERSION);
    g_pCVar = _cvarInterface;
    ConVar_Register(0, this);

    GET_V_IFACE_CURRENT(GetServerFactory, _serverClients, IServerGameClients, INTERFACEVERSION_SERVERGAMECLIENTS);
    GET_V_IFACE_ANY(GetEngineFactory, _pluginHelpers, IServerPluginHelpers, INTERFACEVERSION_ISERVERPLUGINHELPERS);

    SH_ADD_HOOK(IServerGameClients, ClientCommand, _serverClients, SH_MEMBER(this, &ConsoleManager::OnClientCommand), false);

    return true;
}

ConsoleManager::~ConsoleManager()
{
    SH_REMOVE_HOOK(IServerGameClients, ClientCommand, _serverClients, SH_MEMBER(this, &ConsoleManager::OnClientCommand), false);
    _cvarInterface = nullptr;
    _engineServer = nullptr;
}

void ConsoleManager::OnClientCommand(edict_t *edict, const CCommand &args)
{
    auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByEdict(edict);

    if (player == nullptr || args.ArgC() < 1)
    {
        return;
    }

    auto command = args.Arg(0);

    if (strcmp(command, "sm_jez_menu") == 0)
    {
        g_JabronEZ.GetMenuManager()->OpenMenu(player);
        RETURN_META(MRES_SUPERCEDE);
    }

    if (strcmp(command, "sm_jez_give_flash") == 0)
    {
        player->GiveNamedItem("weapon_flashbang");
        player->GiveNamedItem("weapon_molotov");
        player->GiveNamedItem("weapon_decoy");
        player->GiveNamedItem("weapon_smokegrenade");
        player->GiveNamedItem("weapon_hegrenade");
        RETURN_META(MRES_SUPERCEDE);
    }

    if (strcmp(command, "sm_jez_angles") == 0)
    {
        auto angles = player->GetEyeAngles();
        META_CONPRINTF("Angles: %f %f %f\n", angles.x, angles.y, angles.z);
        RETURN_META(MRES_SUPERCEDE);
    }
}

void ConsoleManager::SendClientCommand(edict_t *edict, const char *command)
{
    _pluginHelpers->ClientCommand(edict, command);
}

void ConsoleManager::LoadConfiguration()
{
    _engineServer->ServerCommand("exec jabronez\n");
}
