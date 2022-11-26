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
#include "particle_manager.h"

SH_DECL_HOOK2_void(IServerGameClients, ClientCommand, SH_NOATTRIB, 0, edict_t *, const CCommand &);
SH_DECL_HOOK1_void(ConCommand, Dispatch, SH_NOATTRIB, false, const CCommand &);
SH_DECL_HOOK1_void(IServerGameClients, SetCommandClient, SH_NOATTRIB, false, int);

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

    auto sayCommand = _cvarInterface->FindCommand("say");

    if (sayCommand == nullptr)
    {
        snprintf(error, maxlen, "Unable to find say command");
        return false;
    }

    auto teamSayCommand = _cvarInterface->FindCommand("say_team");

    if (teamSayCommand == nullptr)
    {
        snprintf(error, maxlen, "Unable to find say_team command");
        return false;
    }

    _onClientCommandHookId = SH_ADD_HOOK(IServerGameClients, ClientCommand, _serverClients, SH_MEMBER(this, &ConsoleManager::OnClientCommand), false);
    _sayCommandHookId = SH_ADD_HOOK(ConCommand, Dispatch, sayCommand, SH_MEMBER(this, &ConsoleManager::OnSayCommand), false);
    _teamSayCommandHookId = SH_ADD_HOOK(ConCommand, Dispatch, teamSayCommand, SH_MEMBER(this, &ConsoleManager::OnSayCommand), false);
    _setCommandClientHookId = SH_ADD_HOOK(IServerGameClients, SetCommandClient, _serverClients, SH_MEMBER(this, &ConsoleManager::SetCommandClient), false);

    return true;
}

ConsoleManager::~ConsoleManager()
{
    if (_onClientCommandHookId != 0)
    {
        SH_REMOVE_HOOK_ID(_onClientCommandHookId);
        _onClientCommandHookId = 0;
    }

    if (_sayCommandHookId != 0)
    {
        SH_REMOVE_HOOK_ID(_sayCommandHookId);
        _sayCommandHookId = 0;
    }

    if (_teamSayCommandHookId != 0)
    {
        SH_REMOVE_HOOK_ID(_teamSayCommandHookId);
        _teamSayCommandHookId = 0;
    }

    if (_setCommandClientHookId != 0)
    {
        SH_REMOVE_HOOK_ID(_setCommandClientHookId);
        _setCommandClientHookId = 0;
    }

    _cvarInterface = nullptr;
    _engineServer = nullptr;
}

void ConsoleManager::OnClientCommand(edict_t *edict, const CCommand &args)
{
    auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByEdict(edict);

    if (player == nullptr || !player->IsValid() || args.ArgC() < 1)
    {
        return;
    }

    auto command = args.Arg(0);

    if (strcmp(command, "sm_jez_menu") == 0)
    {
        g_JabronEZ.GetMenuManager()->OpenMenu(player);
        RETURN_META(MRES_SUPERCEDE);
    }

    if (strcmp(command, "sm_jez_angles") == 0)
    {
        auto angles = player->GetEyeAngles();
        META_CONPRINTF("Angles: %f %f %f\n", angles.x, angles.y, angles.z);
        RETURN_META(MRES_SUPERCEDE);
    }

    if (strcmp(command, "sm_jez_short_smoke") == 0)
    {
        float x = atof(args.Arg(1));
        float y = atof(args.Arg(2));
        float z = atof(args.Arg(3));

        g_JabronEZ.GetParticleManager()->CreateShortSmoke(Vector(x, y, z), QAngle(0, 0, 0));

        RETURN_META(MRES_SUPERCEDE);
    }

    if (strcmp(command, "sm_jez_respawn") == 0)
    {
        if (!player->IsAlive())
            player->RespawnPlayer();

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

void ConsoleManager::OnSayCommand(const CCommand &command)
{
    if (command.ArgC() < 2 || _lastCommandClient <= 0)
        RETURN_META(MRES_IGNORED);

    auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByClientIndex(_lastCommandClient);

    if (player == nullptr || !player->IsValid())
        RETURN_META(MRES_IGNORED);

    auto message = command.Arg(1);

    if (strcmp("!jezmenu", message) == 0)
    {
        g_JabronEZ.GetMenuManager()->OpenMenu(player, 1);
        RETURN_META(MRES_SUPERCEDE);
    }

    if (strcmp("!jezaddspot", message) == 0)
    {
        player->DoAddSpot();
        player->RefreshGrenadesMenu();
        RETURN_META(MRES_SUPERCEDE);
    }

    if (strcmp("!jezremovespot", message) == 0)
    {
        player->DoRemoveSpot();
        player->RefreshGrenadesMenu();
        RETURN_META(MRES_SUPERCEDE);
    }

    if (strcmp("!jezplayback", message) == 0)
    {
        player->DoTogglePlayback();
        player->RefreshGrenadesMenu();
        RETURN_META(MRES_SUPERCEDE);
    }

    if (strcmp("!jezfastforward", message) == 0)
    {
        player->DoFastForward();
        player->RefreshGrenadesMenu();
        RETURN_META(MRES_SUPERCEDE);
    }

    if (strcmp("!jeztickrate", message) == 0)
    {
        player->DoToggleGrenadeThrowTickRate();
        player->RefreshGrenadesMenu();
        RETURN_META(MRES_SUPERCEDE);
    }

    if (strcmp("!jezshortgrenades", message) == 0)
    {
        player->DoToggleShortGrenades();
        player->RefreshGrenadesMenu();
        RETURN_META(MRES_SUPERCEDE);
    }

    if (strcmp("!jezgod", message) == 0)
    {
        player->DoToggleGodMode();
        player->RefreshGrenadesMenu();
        RETURN_META(MRES_SUPERCEDE);
    }

    if (strcmp("!jeznoclip", message) == 0)
    {
        player->DoToggleNoClip();
        player->RefreshGrenadesMenu();
        RETURN_META(MRES_SUPERCEDE);
    }

    RETURN_META(MRES_IGNORED);
}

void ConsoleManager::SetCommandClient(int clientSlot)
{
    // Convert the client slot to the client entity index.
    _lastCommandClient = clientSlot + 1;
    RETURN_META(MRES_IGNORED);
}
