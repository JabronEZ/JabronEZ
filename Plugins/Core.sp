/**
 * This file is property of ThinkASDF.
 */

/**
 * Preprocessors
 */
#pragma semicolon 1

/**
 * Includes
 */
#include <sourcemod>
#include <cstrike>
#include <regex>
#include <sdkhooks>
#include <sdktools>
#include <sdktools_functions>
#include <smlib>
#include "include/jez"
#include "include/core_globals"
#include "include/core_natives"
#include "include/core_forwards"
#include "include/core_usermessages"
#include "include/core_commandlisteners"
#include "include/core_database"
#include "include/core_maps"
#include "include/core_events"
#include "include/core_util"
#include "include/core_configs"
#include "include/core_console"
#include "include/core_query"
#include "include/core_usermeta"
#include "include/core_player_struct"
#include "include/core_group_struct"
#include "include/core_groupinvite_struct"
#include "include/core_statscontainer_struct"
#include "include/core_tloadfirststatscontainerquerydata_struct"
#include "include/core_tsendstatssessionupdatequerydata_struct"
#include "include/core_tloadstatsbyaccountidquerydata_struct"
#include "include/core_tsavestatscontainerquerydata_struct"
#include "include/core_statssession_struct"
#include "include/core_statssessiondelta_struct"
#include "include/core_helpmenumeta_struct"
#include "include/core_nomination_struct"
#include "include/core_inferno"
#include "include/core_infernometa_struct"
#include "include/core_groups"
#include "include/core_stats"
#include "include/core_language"
#include "include/core_adverts"
#include "include/core_warmup"
#include "include/core_help"
#include "include/core_tehooks"
#include "include/core_soundhooks"
#include "include/core_playerphysics"
#include "include/core_vworlds"
#include "include/core_vworld_struct"

#pragma dynamic 16384

/**
 * Plugin Info
 */
public Plugin:myinfo =
{
    name = "Freedom35 Core",
    author = "ThinkASDF",
    description = "",
    version = "1.0",
    url = "http://www.thinkasdf.com/jez"
};

/**
 * Here we are able to register our forwards, natives, etc.
 * This is called before OnPluginStart()
 */
public APLRes:AskPluginLoad2(Handle:myself, bool:late, String:error[], err_max)
{
    // Setup our array for our map list and map history
    g_MapList = CreateArray(64);
    g_MapHistory = CreateArray(64);
    g_ServerAdverts = CreateArray(256);

    // Setup our array for player tracking
    g_Players = CreateArray();
    
    // Setup our array for group tracking
    g_Groups = CreateArray();

    // Setup our array from menu meta tracking
    g_HelpMenuMetas = CreateArray();
    
    // Setup our array for nomination tracking
    g_Nominations = CreateArray();
    
    // Setup our array for inferno meta tracking
    g_InfernoMetas = CreateArray();
    
    // Setup our vworlds
    InitVWorlds();

    // Clear the current game mode
    Format(g_CurrentGameMode, sizeof(g_CurrentGameMode), "%s", "");

    // Determine the server's identity
    if(!DetermineServerIdentity())
    {
        Format(error, err_max, "%s", "Unable to determine server identity");
        return APLRes_Failure;
    }

    // Connect to the database
    if(!ConnectToDatabase())
    {
        Format(error, err_max, "%s", "Unable to connect to database");
        return APLRes_Failure;
    }

    // Determine the server info
    if(!DetermineServerInfo())
    {
        Format(error, err_max, "%s", "Unable to determine server info");
        return APLRes_Failure;
    }

    // Determine the root server config
    if(!DetermineRootConfig())
    {
        Format(error, err_max, "%s", "Unable to determine root server config");
        return APLRes_Failure;
    }
    
    // Determine the root server adverts
    if(!DetermineRootAdverts())
    {
        Format(error, err_max, "%s", "Unable to determine root server adverts");
        return APLRes_Failure;
    }

    // Determine the base config
    if(!DetermineBaseConfig())
    {
        Format(error, err_max, "%s", "Unable to determine base gamemode config");
        return APLRes_Failure;
    }

    // Compile the maplist
    if(!CompileMapList())
    {
        Format(error, err_max, "%s", "Unable to compile maplist");
        return APLRes_Failure;
    }
    
    // Compile the advert list
    if(!CompileAdvertList())
    {
        Format(error, err_max, "%s", "Unable to compile adverts");
        return APLRes_Failure;
    }

    // Disable server hibernation
    ServerCommand("sv_hibernate_when_empty 0\n");

    // Execute the root server config
    ExecuteRootConfig();

    // Execute the base config for the game mode
    ExecuteBaseConfig();

    // Execute the server config
    ExecuteServerConfig();

    // Create forwards
    if(!CreateForwards())
    {
        Format(error, err_max, "%s", "Unable to create forwards!");
        return APLRes_Failure;
    }

    // Create natives
    if(!CreateNatives())
    {
        Format(error, err_max, "%s", "Unable to create natives!");
        return APLRes_Failure;
    }

    return APLRes_Success;
}

public Action:CS_OnTerminateRound(&Float:p_Delay, &CSRoundEndReason:p_Reason)
{
    if(p_Reason == CSRoundEnd_Draw)
    {
        new CSRoundEndReason:s_NewReason = CSRoundEnd_TerroristWin;

        if(g_DefaultWinner == CS_TEAM_T)
            s_NewReason = CSRoundEnd_TerroristWin;
        else if(g_DefaultWinner == CS_TEAM_CT)
            s_NewReason = CSRoundEnd_CTWin;
        else
            s_NewReason = GetRandomInt(0, 1) ? CSRoundEnd_TerroristWin : CSRoundEnd_CTWin;

        p_Reason = s_NewReason;
        return Plugin_Changed;
    }

    return Plugin_Continue;
}

/**
 * Here we will create all of our core commands, variables, timers, event hooks,
 * etc...
 */
public OnPluginStart()
{
    // Load common phrases
    LoadTranslations("common.phrases");
    LoadTranslations("freedom35.phrases");
    
    // Compile our regex for workshop map names
    g_WorkshopMapRegex = CompileRegex("^workshop\\/[0-9]+\\/(.+)$");

    // Create some cvars / commands
    InitConsole();

    // Install our usermessage hooks
    InstallUserMessageHooks();

    // Install our event hooks
    InstallEventHooks();

    // Install our command listeners
    InstallCommandListeners();

    // Disable server hibernation
    ServerCommand("sv_hibernate_when_empty 0\n");

    // Execute the root server config
    ExecuteRootConfig();

    // Execute the base config for the game mode
    ExecuteBaseConfig();

    // Execute the server config
    ExecuteServerConfig();
    
    // Force spectator count
    ServerCommand("mp_spectators_max 64\n");

    // Allow our map changer to think
    CreateTimer(1.0, OnMapChangeThink, _, TIMER_REPEAT);

    // Allow our automatic map vote to think
    CreateTimer(1.0, OnMapVoteThink, _, TIMER_REPEAT);

    AddCommandListener(Action_OnChangelevel, "changelevel");
    AddCommandListener(Action_OnTeamJoin, "jointeam");
    
    // Our hooks to make core / vworlds play nicely
    HookEvent("smokegrenade_detonate", SmokeGrenadeDetonateEvent, EventHookMode_Pre); // Allows us to block the screen effect and produce our own
    
    AddTempEntHook("EffectDispatch", EffectDispatchHook);
    AddTempEntHook("Entity Decal", EntityDecalHook);
    AddTempEntHook("World Decal", WorldDecalHook);
    AddTempEntHook("Dynamic Light", DynamicLightHook);
    AddTempEntHook("Explosion", ExplosionHook);
    AddTempEntHook("Shotgun Shot", FireBulletsHook); // misleading name... splewis is a legend. allows us to block gunshot sounds!
    
    AddNormalSoundHook(BaseNormalSoundHook);
}

/**
 * Retrieve the number of players that are put in the server and fully authed (in-game and JEZ auth)
 * Optionally allow bots in the count by passing true to p_AllowBots
 */
GetFullyQualifiedPlayerCount(bool:p_AllowBots = true)
{
    new s_PlayerCount = 0;
    new s_TotalPlayers = GetArraySize(g_Players);
    decl Handle:s_Player;

    for(new s_Index = 0; s_Index < s_TotalPlayers; s_Index++)
    {
        s_Player = GetArrayCell(g_Players, s_Index);

        if(!GetPlayerConnected(s_Player) || !GetPlayerAuthorized(s_Player) || !GetPlayerPutInServer(s_Player))
        {
            continue;
        }

        if(!p_AllowBots && GetPlayerBot(s_Player))
        {
            continue;
        }

        if(!GetPlayerBot(s_Player) && !GetPlayerJEZAuthorized(s_Player))
        {
            continue;
        }

        s_PlayerCount++;
    }

    return s_PlayerCount;
}

public bool:OnClientConnect(p_Client, String:p_RejectMsg[], p_MaxLen)
{
    // Check if the client is a bot
    new bool:s_IsBot = IsFakeClient(p_Client);

    // Get the client's user ID
    new s_UserID = GetClientUserId(p_Client);

    // Ignore group invites (list of jez account IDs)
    new Handle:s_IgnoreGroupInvites = CreateArray();
    
    // Create the player structure
    new Handle:s_Player = CreatePlayer(p_Client, s_UserID, "", s_IsBot, false, false, false, false, false, 0, false, INVALID_HANDLE, INVALID_HANDLE, s_IgnoreGroupInvites, "en", false, -1.0, 0.0, 1.0, 0.0, 1.0, 0.0);
    
    SetPlayerLanguageWrapper(s_Player, "en");

    // Push the player structure into the global player array
    PushArrayCell(g_Players, s_Player);

    new bool:s_ShouldKick = false;
    new String:s_RejectMsg[512];

    Call_StartForward(g_OnPlayerConnectForward);
    Call_PushCell(s_Player);
    Call_PushCellRef(s_ShouldKick);
    Call_PushStringEx(s_RejectMsg, sizeof(s_RejectMsg), SM_PARAM_STRING_UTF8, SM_PARAM_COPYBACK);
    Call_PushCell(sizeof(s_RejectMsg));
    Call_Finish();

    if(s_ShouldKick)
    {
        Format(p_RejectMsg, p_MaxLen, "%s", s_RejectMsg);
        return false;
    }

    return true;
}

public OnClientSettingsChanged(p_Client)
{
    // Get the user ID of the client
    new s_UserID = GetClientUserId(p_Client);

    // Lookup the player structure using the user ID
    new Handle:s_Player = FindPlayerByUserID(g_Players, s_UserID);

    FixPlayerLanguage(s_Player);
}

public OnClientConnected(p_Client)
{
    // Get the user ID of the client
    new s_UserID = GetClientUserId(p_Client);

    // Lookup the player structure using the user ID
    new Handle:s_Player = FindPlayerByUserID(g_Players, s_UserID);

    // Set the player as connected
    SetPlayerConnected(s_Player, true);

    Call_StartForward(g_OnPlayerConnectedForward);
    Call_PushCell(s_Player);
    Call_Finish();
}

public OnClientPutInServer(p_Client)
{
    // Get the user ID of the client
    new s_UserID = GetClientUserId(p_Client);

    // Lookup the player structure using the user ID
    new Handle:s_Player = FindPlayerByUserID(g_Players, s_UserID);

    // Set the player as put in server
    SetPlayerPutInServer(s_Player, true);

    // Client hooks
    SDKHook(p_Client, SDKHook_SetTransmit, PlayerSetTransmitHook);
    SDKHook(p_Client, SDKHook_OnTakeDamage, OnTakeDamageHook);
    
    Call_StartForward(g_OnPlayerPutInServerForward);
    Call_PushCell(s_Player);
    Call_Finish();

    // Check for state changes with RTV
    RTVThink();
}

public OnClientDisconnect(p_Client)
{
    // Create a place to store the player's array index
    decl s_ArrayIndex;

    // Find the player by client index
    new Handle:s_Player = FindPlayerByClient(g_Players, p_Client, 0, s_ArrayIndex);
    
    // Unhook for SDKHooks
    SDKUnhook(p_Client, SDKHook_OnTakeDamage, OnTakeDamageHook);
    
    // remove player from their groups
    if(s_Player)
    {
        new Handle:s_Group = GetPlayerGroup(s_Player);
            
        if(s_Group != INVALID_HANDLE)
        {
            // first of all, we might need to make someone else the leader.
            new Handle:s_Leader = GetGroupLeader(s_Group);
            new bool:s_PlayerIsLeader = s_Leader == s_Player;
            
            // demote player from leader
            if(s_PlayerIsLeader)
                DemotePlayerFromGroupLeader(s_Player);
                
            // remove them from the group
            RemovePlayerFromGroup(s_Player);
            
            // new size of group
            new s_GroupSize = GetGroupSize(s_Group);
            
            // tell everyone else in the group that you left
            decl String:s_ClientName[256];
            GetClientName(p_Client, s_ClientName, sizeof(s_ClientName));
            
            decl Handle:s_GroupPlayer;
            decl s_GroupPlayerClient;
            for(new s_Index = 0; s_Index < s_GroupSize; s_Index++)
            {
                s_GroupPlayer = GetGroupPlayer(s_Group, s_Index);
                s_GroupPlayerClient = GetPlayerClient(s_GroupPlayer);
                
                UTIL_Message(s_GroupPlayerClient, "%t", "JEZ someone has left the group", s_ClientName);
            }
            
            if(s_PlayerIsLeader)
            {
                if(s_GroupSize > 0)
                {
                    new s_RandomGroupPlayerIndex = GetRandomInt(0, s_GroupSize - 1);
                    new Handle:s_RandomGroupPlayer = GetGroupPlayer(s_Group, s_RandomGroupPlayerIndex);
                    new s_RandomGroupPlayerClient = GetPlayerClient(s_RandomGroupPlayer);
                    
                    PromotePlayerToGroupLeader(s_RandomGroupPlayer);
                    UTIL_Message(s_RandomGroupPlayerClient, "%t", "JEZ old group leader left you have been promoted");
                }
            }
            
            if(s_GroupSize <= 0)
                DisbandGroup(s_Group);
        }
    }

    // Check that the player handle is valid
    if(!s_Player)
    {
        // Check for state changes with RTV
        RTVThink();

        return;
    }

    Call_StartForward(g_OnPlayerDisconnectForward);
    Call_PushCell(s_Player);
    Call_Finish();

    // Remove the player from the array
    RemoveFromArray(g_Players, s_ArrayIndex);

    // Close the handle
    CloseHandle(s_Player);

    // Check for state changes with RTV
    RTVThink();
}

public OnClientAuthorized(p_Client, const String:p_SteamID[])
{
    // Get the user ID of the client
    new s_UserID = GetClientUserId(p_Client);

    // Lookup the player structure using the user ID
    new Handle:s_Player = FindPlayerByUserID(g_Players, s_UserID);

    // Set the player as authorized
    SetPlayerAuthorized(s_Player, true);

    // Set the player steam ID
    SetPlayerSteamID(s_Player, p_SteamID);

    Call_StartForward(g_OnPlayerAuthorizedForward);
    Call_PushCell(s_Player);
    Call_Finish();

    // If they are not a bot, they need to be JEZ authed
    if(!IsFakeClient(p_Client))
    {
        // Begin authorization of JEZ player by querying the users table
        decl String:s_SelectQuery[1024];
        Format(s_SelectQuery, sizeof(s_SelectQuery), "SELECT * FROM `jez_users` WHERE `user_csgo_steamid` = '%s' LIMIT 1", p_SteamID);

        SQL_TQuery(g_Database, OnSelectPlayerQueryCompleted, s_SelectQuery, s_UserID);
    }
}

/**
 * Callback for JEZ player authorization query.
 */
public OnSelectPlayerQueryCompleted(Handle:p_Owner, Handle:p_Handle, const String:p_Error[], any:p_UserID)
{
    // Lookup the player by user ID
    new Handle:s_Player = FindPlayerByUserID(g_Players, p_UserID);

    // Store the client index
    new s_Client = GetPlayerClient(s_Player);

    // The player must have already disconnected, so just return
    if(!s_Player)
    {
        return;
    }

    // If the handle is invalid, then the query failed
    if(!p_Handle)
    {
        PrintToServer("[JEZ] ERROR! Select player query failed: %s", p_Error);
        return;
    }

    // Check if there was any data returned, otherwise we know they aren't a member
    if(!SQL_GetRowCount(p_Handle))
    {
        // Kick them and tell them to sign-up
        KickClient(s_Client, "%s", "Please sign-up at www.thinkasdf.com/jez");

        // Clean up the handle and return
        CloseHandle(p_Handle);
        return;
    }

    // Fetch the row
    SQL_FetchRow(p_Handle);

    // Set the JEZ authorized state
    SetPlayerJEZAuthorized(s_Player, true);

    // Determine the field indexes
    decl s_IDField, s_PremiumField;
    SQL_FieldNameToNum(p_Handle, "id", s_IDField);
    SQL_FieldNameToNum(p_Handle, "user_premium", s_PremiumField);

    // Fetch the account ID
    new s_AccountID = SQL_FetchInt(p_Handle, s_IDField);

    // Store the account ID into the player structure
    SetPlayerJEZAccountID(s_Player, s_AccountID);

    // Fetch the premium state
    new bool:s_IsPremium = SQL_FetchInt(p_Handle, s_PremiumField) != 0;

    // Store the premium state into the player structure
    SetPlayerJEZPremium(s_Player, s_IsPremium);

    Call_StartForward(g_OnPlayerJEZAuthorizedForward);
    Call_PushCell(s_Player);
    Call_Finish();

    // Check if the server is premium only, and if it is then kick the player
    // if they are not premium.
    if(StrEqual(g_ServerAccess, "Premium") && !s_IsPremium)
    {
        KickClient(s_Client, "%s", "This server requires a premium membership. Please subscribe at www.thinkasdf.com/jez");

        // Clean up the handle and return
        CloseHandle(p_Handle);
        return;
    }
    
    // At this point we can load their language using user metas
    TGetUserMeta(s_AccountID, "language", "OnLanguageLoadedCallback", INVALID_HANDLE, p_UserID);

    // Close the query handle
    CloseHandle(p_Handle);
}

KillWarmup()
{
    ServerCommand("mp_warmup_end\n");
}

KillWarmupEx()
{
    CreateTimer(0.1, KillWarmupTimerCallback);
}

public Action:KillWarmupTimerCallback(Handle:p_Timer)
{
    KillWarmup();
}

public OnMapStart()
{
    PrintToServer("[JEZ] Map started");

    // Add map to history
    decl String:s_CurrentMap[256];
    GetCurrentMap(s_CurrentMap, sizeof(s_CurrentMap));

    AddMapToHistory(s_CurrentMap);

    // Clear nominations
    ClearNominations();

    // Disable server hibernation
    ServerCommand("sv_hibernate_when_empty 0\n");

    // Execute the root server config
    ExecuteRootConfig();

    // Execute the base config for the game mode
    ExecuteBaseConfig();

    // Execute the server config
    ExecuteServerConfig();

    CalculateMapCycleNextmap(s_CurrentMap);

    Call_StartForward(g_MapStartForward);
    Call_PushString(s_CurrentMap);
    Call_Finish();

    if(StrEqual(s_CurrentMap, "workshop/382106887/jez_loadmap"))
    {
        ClearMapHistory();
        ChangeMap(g_FirstMap, true, "Starting JEZ core");
    }
    
    g_AdvertsTimer = CreateTimer(float(g_AdvertsInterval), DisplayAdvertsTimer, _, TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
    
    g_IsWarmupPeriod = false;
    g_WarmupPeriodStartTime = 0.0;
    g_MatchStartTime = 0.0;
    g_MatchHasStarted = false;
    g_MatchStartRound = 0;
    g_MatchCurrentRound = 0;
    g_CurrentRoundEnded = false;
    g_MapIsChangingViaLimit = false;
    g_MatchRoundsWon = 0;

    ResetRTVGlobals();
    ResetMapVoteGlobals();
    
    // read out indexes/info from string tables
    g_EffectDispatchStringTable = FindStringTable("EffectDispatch");
    
    if(g_EffectDispatchStringTable == INVALID_STRING_TABLE)
    {
        PrintToServer("Error looking up EffectDispatch string table!");
        return;
    }
    
    g_ParticleEffectIndex = FindStringIndex(g_EffectDispatchStringTable, "ParticleEffect");
       
    if(g_ParticleEffectIndex == INVALID_STRING_INDEX)
    {
        PrintToServer("Error looking up ParticleEffect in EffectDispatch string table!");
        return;
    }
    
    g_ParticleEffectNamesStringTable = FindStringTable("ParticleEffectNames");
    
    if(g_ParticleEffectNamesStringTable == INVALID_STRING_TABLE)
    {
        PrintToServer("Error looking up ParticleEffectNames string table!");
        return;
    }
    
    g_ExplosionSmokeGrenadeIndex = FindStringIndex(g_ParticleEffectNamesStringTable, "explosion_smokegrenade");
    
    if(g_ExplosionSmokeGrenadeIndex == INVALID_STRING_INDEX)
    {
        PrintToServer("Error looking up explosion_smokegrenade in ParticleEffectNames string table!");
        return;
    }
    
    g_SmokeScreenIndex = FindStringIndex(g_ParticleEffectNamesStringTable, "explosion_screen_smokegrenade_new");
    
    if(g_SmokeScreenIndex == INVALID_STRING_INDEX)
    {
        PrintToServer("Error looking up explosion_screen_smokegrenade_new in ParticleEffectNames string table!");
        return;
    }
    
    new s_ParticlePrecacheGlow = PrecacheModel("particle/particle_glow_01.vmt", true);
    
    new s_DecalStringTable = FindStringTable("decalprecache");
    
    if(s_DecalStringTable == INVALID_STRING_TABLE)
    {
        PrintToServer("Error finding decalprecache stringtable.");
        return;
    }
    
    g_MolotovScorchDecal = PrecacheDecal("decals/MolotovScorch", true);    
    g_ScorchDecal = PrecacheDecal("decals/scorch1_subrect", true);
}

public OnMapEnd()
{
    PrintToServer("[JEZ] Map ended");

    g_IsWarmupPeriod = false;
    g_WarmupPeriodStartTime = 0.0;
    g_MatchHasStarted = false;
    g_MatchStartTime = 0.0;
    g_MatchStartRound = 0;
    g_MapIsChangingViaLimit = false;
    g_MatchCurrentRound = 0;
    g_CurrentRoundEnded = false;
    g_MatchRoundsWon = 0;

    ResetRTVGlobals();
    ResetMapVoteGlobals();

    if(g_RestartGameTimer != INVALID_HANDLE)
    {
        KillTimer(g_RestartGameTimer);
        g_RestartGameTimer = INVALID_HANDLE;
    }
}

public OnGameFrame()
{
    JEZWarmupThink();
    
    g_DestroyedInferno = -1;
    g_DestroyedInfernoThrowerRef = INVALID_ENT_REFERENCE;
    g_DestroyedInfernoGrenadeType = CSWeapon_NONE;
}

public Action:OnPlayerRunCmd(p_Client, &p_Buttons, &p_Impulse, Float:p_Velocity[3], Float:p_Angles[3], &p_Weapon, &p_SubType, &p_CmdNum, &p_TickCount, &p_Seed, p_Mouse[2])
{
    new Action:s_Result = Physics_OnPlayerRunCmd(p_Client, p_Buttons, p_Impulse, p_Velocity, p_Angles, p_Weapon, p_SubType, p_CmdNum, p_TickCount, p_Seed, p_Mouse);
    
    if(s_Result == Plugin_Handled)
        return Plugin_Handled;
        
    return Plugin_Continue;
}

public Action:Action_OnChangelevel(p_Client, const String:p_Command[], p_Args)
{
    if (!g_ShouldRestartGracefully)
        return Plugin_Continue;

    // Someone has requested a graceful restart.
    PrintToServer("[JEZ] Restarting server");
    g_ShouldRestartGracefully = false;
    ServerCommand("quit");

    return Plugin_Handled;
}


public Action:Action_OnTeamJoin(p_Client, const String:p_Command[], p_Args)
{
    if(!g_DisableTeamChangeLimit || !UTIL_IsValidClient(p_Client) || p_Args < 1)
        return Plugin_Continue;
    
    decl String:s_Arg[64];
    GetCmdArg(1, s_Arg, sizeof(s_Arg));
    
    new s_ToTeam = StringToInt(s_Arg);
    
    if(s_ToTeam < CS_TEAM_NONE || s_ToTeam > CS_TEAM_CT)
        return Plugin_Continue;
        
    if(s_ToTeam == CS_TEAM_NONE)
        s_ToTeam = GetRandomInt(0, 1) == 1 ? CS_TEAM_CT : CS_TEAM_T;

    ChangeClientTeam(p_Client, s_ToTeam);
    return Plugin_Handled;
}

public OnEntityCreated(p_Entity, const String:p_Classname[])
{
    if(StrEqual(p_Classname, "flashbang_projectile") || StrEqual(p_Classname, "smokegrenade_projectile") ||
        StrEqual(p_Classname, "hegrenade_projectile") || StrEqual(p_Classname, "decoy_projectile") ||
        StrEqual(p_Classname, "molotov_projectile"))
    {
        SDKHook(p_Entity, SDKHook_SpawnPost, ProjectileSpawnHook);
    }
    else if(StrEqual(p_Classname, "inferno"))
    {
        new CSWeaponID:s_GrenadeType = CSWeapon_NONE;
        
        new s_DetonatingMolotov = JEZ_GetCurrentlyDetonatingMolotov();
        new s_DetonatingIncendiary = JEZ_GetCurrentlyDetonatingIncendiary();
        
        if(s_DetonatingMolotov > 0)
            s_GrenadeType = CSWeapon_MOLOTOV;
        else if(s_DetonatingIncendiary > 0)
            s_GrenadeType = CSWeapon_INCGRENADE;
            
        new s_ThrowerRef = INVALID_ENT_REFERENCE;
        
        if(s_GrenadeType == CSWeapon_MOLOTOV)
            s_ThrowerRef = EntIndexToEntRef(GetEntPropEnt(s_DetonatingMolotov, Prop_Data, "m_hThrower"));
        else if(s_GrenadeType == CSWeapon_INCGRENADE)
            s_ThrowerRef = EntIndexToEntRef(GetEntPropEnt(s_DetonatingIncendiary, Prop_Data, "m_hThrower"));
            
        new Handle:s_Meta = CreateInfernoMeta(p_Entity, _:s_GrenadeType, s_ThrowerRef);
        PushArrayCell(g_InfernoMetas, s_Meta);
        
        SDKHook(p_Entity, SDKHook_SetTransmit, InfernoSetTransmitHook);
    }
}

// Hook for entity destruction
public OnEntityDestroyed(entity)
{
    new String:s_Classname[512];
    
    if(IsValidEdict(entity))
        GetEntityClassname(entity, s_Classname, sizeof(s_Classname));
    
    if(StrEqual(s_Classname, "inferno", false))
    {
        new Handle:s_Meta = FindInfernoMetaByEntity(g_InfernoMetas, entity);
        
        if(s_Meta != INVALID_HANDLE)
        {
            g_DestroyedInferno = entity;
            g_DestroyedInfernoThrowerRef = GetInfernoMetaThrowerRef(s_Meta);
            g_DestroyedInfernoGrenadeType = CSWeaponID:GetInfernoMetaGrenadeType(s_Meta);
        }
    }

    RemoveInfernoMeta(entity);
}

public Action:ProjectileSpawnHook(p_Projectile)
{
    decl String:s_Classname[256];
    GetEntityClassname(p_Projectile, s_Classname, sizeof(s_Classname));
    
    if(StrEqual(s_Classname, "flashbang_projectile"))
        SDKHook(p_Projectile, SDKHook_SetTransmit, FlashbangSetTransmitHook);
    else if(StrEqual(s_Classname, "smokegrenade_projectile"))
        SDKHook(p_Projectile, SDKHook_SetTransmit, SmokeSetTransmitHook);
    else if(StrEqual(s_Classname, "hegrenade_projectile"))
        SDKHook(p_Projectile, SDKHook_SetTransmit, HEGrenadeSetTransmitHook);
    else if(StrEqual(s_Classname, "decoy_projectile"))
        SDKHook(p_Projectile, SDKHook_SetTransmit, DecoySetTransmitHook);
    else if(StrEqual(s_Classname, "molotov_projectile"))
        SDKHook(p_Projectile, SDKHook_SetTransmit, MolotovSetTransmitHook);
}

// Route flashbang set transmit hook
public Action:FlashbangSetTransmitHook(p_Flashbang, p_Client)
{
    new Action:s_Result = Plugin_Continue;
    
    VWorlds_FlashbangSetTransmitHook(p_Flashbang, p_Client, s_Result);
    
    Call_StartForward(g_FlashbangSetTransmitHookForward);
    
    Call_PushCell(p_Flashbang);
    Call_PushCell(p_Client);
    Call_PushCellRef(s_Result);
    
    decl s_UnusedResult;
    Call_Finish(s_UnusedResult);
    
    return s_Result;
}  

// Route smoke set transmit hook
public Action:SmokeSetTransmitHook(p_Smoke, p_Client)
{
    new Action:s_Result = Plugin_Continue;
    
    VWorlds_SmokeGrenadeSetTransmitHook(p_Smoke, p_Client, s_Result);
    
    Call_StartForward(g_SmokeGrenadeSetTransmitHookForward);
    
    Call_PushCell(p_Smoke);
    Call_PushCell(p_Client);
    Call_PushCellRef(s_Result);
    
    decl s_UnusedResult;
    Call_Finish(s_UnusedResult);
    
    return s_Result;
}  

// Route hegrenade set transmit hook
public Action:HEGrenadeSetTransmitHook(p_HEGrenade, p_Client)
{
    new Action:s_Result = Plugin_Continue;
    
    VWorlds_HEGrenadeSetTransmitHook(p_HEGrenade, p_Client, s_Result);
    
    Call_StartForward(g_HEGrenadeSetTransmitHookForward);
    
    Call_PushCell(p_HEGrenade);
    Call_PushCell(p_Client);
    Call_PushCellRef(s_Result);
    
    decl s_UnusedResult;
    Call_Finish(s_UnusedResult);
    
    return s_Result;
}

// Route decoy set transmit hook
public Action:DecoySetTransmitHook(p_Decoy, p_Client)
{
    new Action:s_Result = Plugin_Continue;
    
    VWorlds_DecoySetTransmitHook(p_Decoy, p_Client, s_Result);

    Call_StartForward(g_DecoySetTransmitHookForward);
    
    Call_PushCell(p_Decoy);
    Call_PushCell(p_Client);
    Call_PushCellRef(s_Result);
    
    decl s_UnusedResult;
    Call_Finish(s_UnusedResult);
    
    return s_Result;
}

// Route molotov/incendiary set transmit hook
public Action:MolotovSetTransmitHook(p_Molotov, p_Client)
{
    new bool:s_IsIncendiary = UTIL_IsIncendiary(p_Molotov);
    new Action:s_Result = Plugin_Continue;
    
    if(s_IsIncendiary)
    {
        VWorlds_IncendiarySetTransmitHook(p_Molotov, p_Client, s_Result);

        Call_StartForward(g_IncendiarySetTransmitHookForward);
        
        Call_PushCell(p_Molotov);
        Call_PushCell(p_Client);
        Call_PushCellRef(s_Result);
        
        decl s_UnusedResult;
        Call_Finish(s_UnusedResult);
        
        return s_Result;
    }
    else
    {
        VWorlds_MolotovSetTransmitHook(p_Molotov, p_Client, s_Result);

        Call_StartForward(g_MolotovSetTransmitHookForward);
        
        Call_PushCell(p_Molotov);
        Call_PushCell(p_Client);
        Call_PushCellRef(s_Result);
        
        decl s_UnusedResult;
        Call_Finish(s_UnusedResult);
        
        return s_Result;
    }
}

// Set transmit hook for players
public Action:PlayerSetTransmitHook(p_TargetClient, p_Client)
{
    // you don't want to ever block your own transmit
    if(p_Client == p_TargetClient)
        return Plugin_Continue;
    
    new Action:s_Result = Plugin_Continue;
    
    VWorlds_PlayerSetTransmitHook(p_TargetClient, p_Client, s_Result);

    Call_StartForward(g_PlayerSetTransmitHookForward);
    
    Call_PushCell(p_TargetClient);
    Call_PushCell(p_Client);
    Call_PushCellRef(s_Result);
    
    decl s_UnusedResult;
    Call_Finish(s_UnusedResult);
    
    return s_Result;
}

// Proxy for the fire emits from a molotov/incendiary
public Action:InfernoSetTransmitHook(p_Inferno, p_Client)
{
    new Action:s_Result = Plugin_Continue;
    
    VWorlds_InfernoSetTransmitHook(p_Inferno, p_Client, s_Result);
    
    Call_StartForward(g_InfernoSetTransmitHookForward);
    
    Call_PushCell(p_Inferno);
    Call_PushCell(p_Client);
    Call_PushCellRef(s_Result);
    
    decl s_UnusedResult;
    Call_Finish(s_UnusedResult);
    
    return s_Result;
}

// Proxy for OnTakeDamage function
public Action:OnTakeDamageHook(p_Victim, &p_Attacker, &p_Inflictor, &Float:p_Damage, &p_DamageType, &p_Weapon, Float:p_DamageForce[3], Float:p_DamagePosition[3], p_DamageCustom)
{
    new Action:s_Result = Plugin_Continue;
    
    VWorlds_OnTakeDamageHook(p_Victim, p_Attacker, p_Inflictor, p_Damage, p_DamageType, p_Weapon, p_DamageForce, p_DamagePosition, p_DamageCustom, s_Result);
    
    // Plugin logic
    Call_StartForward(g_TakeDamageHookForward);
    
    Call_PushCell(p_Victim);
    Call_PushCellRef(p_Attacker);
    Call_PushCellRef(p_Inflictor);
    Call_PushFloatRef(p_Damage);
    Call_PushCellRef(p_DamageType);
    Call_PushCellRef(p_Weapon);
    Call_PushArrayEx(p_DamageForce, sizeof(p_DamageForce), SM_PARAM_COPYBACK);
    Call_PushArrayEx(p_DamagePosition, sizeof(p_DamagePosition), SM_PARAM_COPYBACK);
    Call_PushCell(p_DamageCustom);
    Call_PushCellRef(s_Result);
    
    decl s_UnusedResult;
    Call_Finish(s_UnusedResult);
        
    return s_Result;
}