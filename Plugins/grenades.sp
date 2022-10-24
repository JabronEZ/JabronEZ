/**
 * This file is property of Freedom35.
 */ 

#pragma semicolon 1

#include <sourcemod>
#include <cstrike>
#include <sdktools_tempents>
#include <sdktools_tempents_stocks>
#include <smlib>
#include <sdkhooks>
#include "include/jez"
#include "include/grenades_globals"
#include "include/grenades_util"
#include "include/grenades_tehooks"
#include "include/grenades_events"
#include "include/grenades_commands"
#include "include/grenades_menus"
#include "include/grenades_grenadeplayer_struct"
#include "include/grenades_grenadetesterspot_struct"

public Plugin:myinfo = 
{
	name = "Freedom35 Grenade Practice",
	author = "ThinkASDF",
	description = "",
	version = "1.0",
	url = "http://www.thinkasdf.com/jez"
};

public OnPluginStart()
{
    LoadTranslations("freedom35.phrases");
    
    new String:s_CurrentGameMode[256];
    JEZ_GetGameMode(s_CurrentGameMode, sizeof(s_CurrentGameMode));
    
    if(!StrEqual(s_CurrentGameMode, GRENADES_GAMEMODE))
    {
        g_PluginEnabled = false;
        return;
    }
    
    g_PluginEnabled = true;
    g_GrenadePlayers = CreateArray();
    g_ShortSmokes = CreateArray();
    g_ShortDecoys = CreateArray();

    CreateTimer(0.5, OnGrenadesDrawThink, _, TIMER_REPEAT);
    HookEvent("flashbang_detonate", FlashbangDetonateEvent);
    HookEvent("hegrenade_detonate", HEGrenadeDetonateEvent);
    HookEvent("decoy_started", DecoyStartedEvent);
    HookEvent("player_death", PlayerDeathEvent);
    HookEvent("player_spawn", PlayerSpawnEvent);
    
    AddCommandListener(BuyCommandHandler, "buy");
}

public Action:BuyCommandHandler(p_Client, const String:p_Command[], p_ArgCount)
{
    if(p_Client <= 0)
        return Plugin_Continue;
        
    SetEntProp(p_Client, Prop_Send, "m_iAccount", 16000);
    return Plugin_Continue;
}

public JEZ_OnNormalSoundHook(p_Clients[64], &p_NumClients, String:p_Sample[PLATFORM_MAX_PATH], &p_Entity, &p_Channel, &Float:p_Volume, &p_Level, &p_Pitch, &p_Flags, String:p_SoundEntry[PLATFORM_MAX_PATH], &p_Seed)
{
    new s_ThinkingInferno = JEZ_GetCurrentlyThinkingInferno();
    
    if(s_ThinkingInferno > 0)
    {
        new Handle:s_Meta = JEZ_GetInfernoMetaByEntity(s_ThinkingInferno);
        
        if(s_Meta == INVALID_HANDLE)
            return;
            
        new s_NewClients[JEZ_MAX_PLAYERS + 1];
        new s_NewClientsCount;
        
        new CSWeaponID:s_InfernoGrenadeType = JEZ_GetInfernoMetaGrenadeType(s_Meta);
        new s_ThrowerRef = JEZ_GetInfernoMetaThrowerRef(s_Meta);
        new s_Thrower = EntRefToEntIndex(s_ThrowerRef);
        
        if(s_Thrower == INVALID_ENT_REFERENCE)
            s_Thrower = 0; // HACKHACK!
        
        FilterClientsForGrenadeType(s_InfernoGrenadeType, s_Thrower, p_Clients, p_NumClients, s_NewClients, s_NewClientsCount);
        CopyClientsArray(s_NewClients, s_NewClientsCount, p_Clients);
        p_NumClients = s_NewClientsCount;

        return;
    }
    
    new s_Molotov = JEZ_GetCurrentlyDetonatingMolotov();
    
    if(s_Molotov > 0)
    {
        new s_NewClients[JEZ_MAX_PLAYERS + 1];
        new s_NewClientsCount;
        
        new s_Thrower = GetEntPropEnt(s_Molotov, Prop_Data, "m_hThrower");
        
        FilterClientsForGrenadeType(CSWeapon_MOLOTOV, s_Thrower, p_Clients, p_NumClients, s_NewClients, s_NewClientsCount);
        CopyClientsArray(s_NewClients, s_NewClientsCount, p_Clients);
        p_NumClients = s_NewClientsCount;

        return;
    }
    
    new s_Incendiary = JEZ_GetCurrentlyDetonatingIncendiary();
    
    if(s_Incendiary > 0)
    {
        new s_NewClients[JEZ_MAX_PLAYERS + 1];
        new s_NewClientsCount;
        
        new s_Thrower = GetEntPropEnt(s_Incendiary, Prop_Data, "m_hThrower");
        
        FilterClientsForGrenadeType(CSWeapon_INCGRENADE, s_Thrower, p_Clients, p_NumClients, s_NewClients, s_NewClientsCount);
        CopyClientsArray(s_NewClients, s_NewClientsCount, p_Clients);
        p_NumClients = s_NewClientsCount;
        
        return;
    }
    
    new s_DestroyedInferno = JEZ_GetDestroyedInferno();
    
    if(s_DestroyedInferno > 0)
    {
        new CSWeaponID:s_DestroyedInfernoGrenadeType = JEZ_GetDestroyedInfernoGrenadeType();
        new s_DestroyedInfernoThrowerRef = JEZ_GetDestroyedInfernoThrowerRef();
        
        if(s_DestroyedInfernoGrenadeType == CSWeapon_NONE || s_DestroyedInfernoThrowerRef == INVALID_ENT_REFERENCE)
            return;
        
        new s_ThrowerRef = s_DestroyedInfernoThrowerRef;
        new s_Thrower = EntRefToEntIndex(s_ThrowerRef);
        
        if(s_Thrower == INVALID_ENT_REFERENCE)
            s_Thrower = 0; // HACK!
        
        new s_NewClients[JEZ_MAX_PLAYERS + 1];
        new s_NewClientsCount;
        
        FilterClientsForGrenadeType(s_DestroyedInfernoGrenadeType, s_Thrower, p_Clients, p_NumClients, s_NewClients, s_NewClientsCount);
        CopyClientsArray(s_NewClients, s_NewClientsCount, p_Clients);
        p_NumClients = s_NewClientsCount;
        
        return;
    }
    
    if(p_Entity <= 0)
        return;
    
    new CSWeaponID:s_GrenadeType = GetGrenadeTypeByProjectile(p_Entity);

    if(s_GrenadeType != CSWeapon_NONE)
    {
        new s_NewClients[JEZ_MAX_PLAYERS + 1];
        new s_NewClientsCount;
        
        new s_Thrower = GetEntPropEnt(p_Entity, Prop_Data, "m_hThrower");
        
        FilterClientsForGrenadeType(s_GrenadeType, s_Thrower, p_Clients, p_NumClients, s_NewClients, s_NewClientsCount);
        CopyClientsArray(s_NewClients, s_NewClientsCount, p_Clients);
        p_NumClients = s_NewClientsCount;
        
        return;
    }
    
    decl String:s_Classname[512];
    GetEntityClassname(p_Entity, s_Classname, sizeof(s_Classname));
    
    if(StrEqual("player", s_Classname))
    {
        new Handle:s_GrenadePlayer = GetGrenadePlayerByClient(p_Entity);
        
        if(s_GrenadePlayer == INVALID_HANDLE)
            return;
            
        new s_NewClients[JEZ_MAX_PLAYERS + 1];
        new s_NewClientsCount;

        FilterClientsForPlayerType(p_Entity, p_Clients, p_NumClients, s_NewClients, s_NewClientsCount);
        CopyClientsArray(s_NewClients, s_NewClientsCount, p_Clients);
        p_NumClients = s_NewClientsCount;
        
        return;
    }
    
    if(StrEqual("inferno", s_Classname))
    {
        new Handle:s_Meta = JEZ_GetInfernoMetaByEntity(p_Entity);
        
        if(s_Meta == INVALID_HANDLE)
            return;
            
        new CSWeaponID:s_InfernoGrenadeType = JEZ_GetInfernoMetaGrenadeType(s_Meta);
        new s_ThrowerRef = JEZ_GetInfernoMetaThrowerRef(s_Meta);
        new s_Thrower = EntRefToEntIndex(s_ThrowerRef);
        
        if(s_Thrower == INVALID_ENT_REFERENCE)
            s_Thrower = 0; // HACK!
        
        new s_NewClients[JEZ_MAX_PLAYERS + 1];
        new s_NewClientsCount;
        
        FilterClientsForGrenadeType(s_InfernoGrenadeType, s_Thrower, p_Clients, p_NumClients, s_NewClients, s_NewClientsCount);
        CopyClientsArray(s_NewClients, s_NewClientsCount, p_Clients);
        p_NumClients = s_NewClientsCount;
        
        return;
    }
}

public OnEntityDestroyed(entity)
{
    if(!g_PluginEnabled)
        return;

    RemoveShortSmoke(entity);
    RemoveShortDecoy(entity);
}

ThrowerSetTransmitHook(p_Thrower, p_Client, CSWeaponID:p_GrenadeType, &Action:p_Result)
{
    new Handle:s_GrenadePlayer = GetGrenadePlayerByClient(p_Client);
    
    if(!s_GrenadePlayer)
        return;
    
    new GrenadeProjectileMode:s_ProjectileMode = GetGrenadePlayerProjectileMode(s_GrenadePlayer, p_GrenadeType);

    if(s_ProjectileMode == GrenadeProjectileMode_NONE)
    {
        p_Result = Plugin_Handled;
        return;
    }
    
    if(s_ProjectileMode == GrenadeProjectileMode_SELF && p_Client != p_Thrower)
    {
        p_Result = Plugin_Handled;
        return;
    }
    
    if(s_ProjectileMode == GrenadeProjectileMode_GROUP && !IsPlayerInMyGroup(p_Thrower, p_Client))
    {
        p_Result = Plugin_Handled;
        return;
    }
}

public JEZ_OnInfernoSetTransmitHook(p_Inferno, p_Client, &Action:p_Result)
{
    new Handle:s_Meta = JEZ_GetInfernoMetaByEntity(p_Inferno);
        
    if(s_Meta == INVALID_HANDLE)
        return;
    
    new CSWeaponID:s_InfernoGrenadeType = JEZ_GetInfernoMetaGrenadeType(s_Meta);
    new s_ThrowerRef = JEZ_GetInfernoMetaThrowerRef(s_Meta);
    new s_Thrower = EntRefToEntIndex(s_ThrowerRef);
    
    if(s_Thrower == INVALID_ENT_REFERENCE)
        s_Thrower = 0; // HACKHACK!
        
    ThrowerSetTransmitHook(s_Thrower, p_Client, s_InfernoGrenadeType, p_Result);
}  

public JEZ_OnFlashbangSetTransmitHook(p_Flashbang, p_Client, &Action:p_Result)
{
    new s_Thrower = GetEntPropEnt(p_Flashbang, Prop_Data, "m_hThrower");
    
    ThrowerSetTransmitHook(s_Thrower, p_Client, CSWeapon_FLASHBANG, p_Result);
}

public JEZ_OnSmokeGrenadeSetTransmitHook(p_SmokeGrenade, p_Client, &Action:p_Result)
{
    new s_Thrower = GetEntPropEnt(p_SmokeGrenade, Prop_Data, "m_hThrower");
    
    ThrowerSetTransmitHook(s_Thrower, p_Client, CSWeapon_SMOKEGRENADE, p_Result);
}

public JEZ_OnHEGrenadeSetTransmitHook(p_HEGrenade, p_Client, &Action:p_Result)
{
    new s_Thrower = GetEntPropEnt(p_HEGrenade, Prop_Data, "m_hThrower");
    
    ThrowerSetTransmitHook(s_Thrower, p_Client, CSWeapon_HEGRENADE, p_Result);
}

public JEZ_OnDecoySetTransmitHook(p_Decoy, p_Client, &Action:p_Result)
{
    new s_Thrower = GetEntPropEnt(p_Decoy, Prop_Data, "m_hThrower");
    
    ThrowerSetTransmitHook(s_Thrower, p_Client, CSWeapon_DECOY, p_Result);
}

public JEZ_OnMolotovSetTransmitHook(p_Molotov, p_Client, &Action:p_Result)
{
    new s_Thrower = GetEntPropEnt(p_Molotov, Prop_Data, "m_hThrower");
    
    ThrowerSetTransmitHook(s_Thrower, p_Client, CSWeapon_MOLOTOV, p_Result);
}

public JEZ_OnIncendiarySetTransmitHook(p_Incendiary, p_Client, &Action:p_Result)
{
    new s_Thrower = GetEntPropEnt(p_Incendiary, Prop_Data, "m_hThrower");
    
    ThrowerSetTransmitHook(s_Thrower, p_Client, CSWeapon_INCGRENADE, p_Result);
}

public JEZ_OnPlayerSetTransmitHook(p_TargetClient, p_Client, &Action:p_Result)
{
    // you don't want to ever block your own transmit lol it causes wierd shit to happen
    if(p_Client == p_TargetClient)
        return;
        
    new Handle:s_GrenadePlayer = GetGrenadePlayerByClient(p_Client);
    
    if(!s_GrenadePlayer)
        return;
    
    new GrenadePlayerMode:s_PlayerMode = GrenadePlayerMode:GetGrenadePlayerPlayerMode(s_GrenadePlayer);
    
    if(s_PlayerMode == GrenadePlayerMode_NONE)
    {
        p_Result = Plugin_Handled;
        return;
    }
    
    if(s_PlayerMode == GrenadePlayerMode_GROUP && !IsPlayerInMyGroup(p_TargetClient, p_Client))
    {
        p_Result = Plugin_Handled;
        return;
    }
}

public JEZ_OnSmokeGrenadeDetonate(p_SmokeIndex, &bool:p_BlockDetonate)
{
    if(!g_PluginEnabled)
        return; 
        
    // Grab the owner from the smoke to determine who threw it
    new s_Thrower = GetEntPropEnt(p_SmokeIndex, Prop_Data, "m_hThrower");
    
    // Get the grenade player
    new Handle:s_GrenadePlayer = GetGrenadePlayerByClient(s_Thrower);
    
    // Validate the grenade player
    if(s_GrenadePlayer == INVALID_HANDLE)
        return;
        
    ProcessDetonationEvent(s_GrenadePlayer, CSWeapon_SMOKEGRENADE, p_SmokeIndex);
    
    new s_SmokeRef = EntIndexToEntRef(p_SmokeIndex);
    RequestFrame(OnSmokeGrenadeDetonate_NextFrame, s_SmokeRef);
}

public OnSmokeGrenadeDetonate_NextFrame(any:p_SmokeRef)
{
    new s_SmokeGrenade = EntRefToEntIndex(p_SmokeRef);
    
    if(s_SmokeGrenade == INVALID_ENT_REFERENCE)
        return;
        
    if(IsShortSmoke(s_SmokeGrenade))
        JEZ_SmokeThinkFade(s_SmokeGrenade);
}

public JEZ_OnMolotovDetonate(p_MolotovIndex, &bool:p_BlockDetonate)
{
    if(!g_PluginEnabled)
        return; 
        
    // Grab the owner from the molly to determine who threw it
    new s_Thrower = GetEntPropEnt(p_MolotovIndex, Prop_Data, "m_hThrower");
    
    // Get the grenade player
    new Handle:s_GrenadePlayer = GetGrenadePlayerByClient(s_Thrower);
    
    // Validate the grenade player
    if(s_GrenadePlayer == INVALID_HANDLE)
        return;
        
    ProcessDetonationEvent(s_GrenadePlayer, CSWeapon_MOLOTOV, p_MolotovIndex);
}

public JEZ_OnIncendiaryDetonate(p_IncendiaryIndex, &bool:p_BlockDetonate)
{
    if(!g_PluginEnabled)
        return; 
        
    // Grab the owner from the incendiary to determine who threw it
    new s_Thrower = GetEntPropEnt(p_IncendiaryIndex, Prop_Data, "m_hThrower");
    
    // Get the grenade player
    new Handle:s_GrenadePlayer = GetGrenadePlayerByClient(s_Thrower);
    
    // Validate the grenade player
    if(s_GrenadePlayer == INVALID_HANDLE)
        return;
        
    ProcessDetonationEvent(s_GrenadePlayer, CSWeapon_INCGRENADE, p_IncendiaryIndex);
}

public JEZ_OnFlashbangDeafen(p_ClientIndex, p_FlashbangIndex, Float:p_Distance, &bool:p_BlockDeafen)
{
    if(!g_PluginEnabled)
        return;
        
    new Handle:s_GrenadePlayer = GetGrenadePlayerByClient(p_ClientIndex);
    
    if(!s_GrenadePlayer)
        return;
    
    new GrenadeProjectileMode:s_FlashMode = GrenadeProjectileMode:GetGrenadePlayerFlashMode(s_GrenadePlayer);
    
    if(s_FlashMode == GrenadeProjectileMode_ALL)
        return;
    
    if(s_FlashMode == GrenadeProjectileMode_NONE)
    {
        p_BlockDeafen = true;
        return;
    }
    
    if(s_FlashMode == GrenadeProjectileMode_SELF)
    {
        new s_Thrower = GetEntPropEnt(p_FlashbangIndex, Prop_Data, "m_hThrower");
        
        if(s_Thrower != p_ClientIndex)
            p_BlockDeafen = true;
            
        return;
    }
    
    if(s_FlashMode == GrenadeProjectileMode_GROUP)
    {
        new s_Thrower = GetEntPropEnt(p_FlashbangIndex, Prop_Data, "m_hThrower");
        
        // is the thrower in our group?
        if(!IsPlayerInMyGroup(s_Thrower, p_ClientIndex))
            p_BlockDeafen = true;
                
        return;
    }
}

public JEZ_OnFlashbangBlind(p_ClientIndex, p_FlashbangIndex, Float:p_HoldTime, Float:p_FadeTime, Float:p_StartingAlpha, &bool:p_BlockBlind)
{
    if(!g_PluginEnabled)
        return;
        
    new Handle:s_GrenadePlayer = GetGrenadePlayerByClient(p_ClientIndex);
    
    if(!s_GrenadePlayer)
        return;
    
    new GrenadeProjectileMode:s_FlashMode = GrenadeProjectileMode:GetGrenadePlayerFlashMode(s_GrenadePlayer);
    
    if(s_FlashMode == GrenadeProjectileMode_ALL)
        return;
    
    if(s_FlashMode == GrenadeProjectileMode_NONE)
    {
        p_BlockBlind = true;
        return;
    }
    
    if(s_FlashMode == GrenadeProjectileMode_SELF)
    {
        new s_Thrower = GetEntPropEnt(p_FlashbangIndex, Prop_Data, "m_hThrower");
        
        if(s_Thrower != p_ClientIndex)
            p_BlockBlind = true;
            
        return;
    }
    
    if(s_FlashMode == GrenadeProjectileMode_GROUP)
    {
        new s_Thrower = GetEntPropEnt(p_FlashbangIndex, Prop_Data, "m_hThrower");
        
        // is the thrower in our group?
        if(!IsPlayerInMyGroup(s_Thrower, p_ClientIndex))
            p_BlockBlind = true;
                
        return;
    }
}

// IMPORTANT: p_ProjectileIndex WILL NOT EXIST IF IT'S A MOLOTOV/INCGRENADE... CONSIDER IT PURELY A REFERENCE!
stock ProcessDetonationEvent(Handle:p_GrenadePlayer, CSWeaponID:p_GrenadeType, p_ProjectileIndex)
{
    // Validate the grenade player
    if(p_GrenadePlayer == INVALID_HANDLE)
        return;
    
    new s_Client = GetGrenadePlayerClient(p_GrenadePlayer);
    new s_UserID = GetClientUserId(s_Client);

    // We don't care if they don't have playback enabled
    if(!GetGrenadePlayerPlaybackEnabled(p_GrenadePlayer))
        return;
        
    new Float:s_PostDetonationDelay = GetDelayPostDetonation(p_GrenadeType);
        
    if(GetGrenadePlayerAwaitingDetonation(p_GrenadePlayer))
    {
        // Get the selected grenade type
        new CSWeaponID:s_SelectedGrenadeType = CSWeaponID:GetGrenadePlayerType(p_GrenadePlayer);
        
        // If they aren't the same, we don't care
        if(s_SelectedGrenadeType != p_GrenadeType)
            return;
        
        SetGrenadePlayerAwaitingDetonation(p_GrenadePlayer, false);
        SetGrenadePlayerPlaybackStarting(p_GrenadePlayer, true);
        
        // Quickly mark the smoke as a short one if it's a smoke
        if(p_GrenadeType == CSWeapon_SMOKEGRENADE)
            MarkShortSmoke(p_ProjectileIndex);
            
        if(p_GrenadeType == CSWeapon_DECOY)
            MarkShortDecoy(p_ProjectileIndex);
        
        KillTriggerPlaybackTimer(p_GrenadePlayer);
        new Handle:s_TriggerPlaybackTimer = CreateTimer(s_PostDetonationDelay, TriggerGrenadeTesterPlaybackTimer, s_UserID);
        SetGrenadePlayerTriggerPlaybackTimer(p_GrenadePlayer, s_TriggerPlaybackTimer);
    }
    else if(GetGrenadePlayerPlaybackStarted(p_GrenadePlayer))
    {
        new s_TossedGrenadeIndex = GetGrenadePlayerTossedGrenade(p_GrenadePlayer);
        
        // We don't care if the nade that pops is the wrong one
        if(s_TossedGrenadeIndex != p_ProjectileIndex)
            return;
            
        SetGrenadePlayerTossedGrenade(p_GrenadePlayer, -1);
        
        KillGotoNextSpotOrFinishTimer(p_GrenadePlayer);
        new Handle:s_GotoNextSpotOrFinishTimer = CreateTimer(s_PostDetonationDelay, GotoNextSpotOrFinishPlaybackTimer, s_UserID);
        SetGrenadePlayerGotoNextSpotOrFinishTimer(p_GrenadePlayer, s_GotoNextSpotOrFinishTimer);
    }
}

public Action:TriggerGrenadeTesterPlaybackTimer(Handle:p_Timer, any:p_UserID)
{
    new s_Client = GetClientOfUserId(p_UserID);
    new Handle:s_GrenadePlayer = GetGrenadePlayerByClient(s_Client);
    
    if(!s_GrenadePlayer)
        return Plugin_Handled;
        
    SetGrenadePlayerTriggerPlaybackTimer(s_GrenadePlayer, INVALID_HANDLE);
    
    StartGrenadeTesterPlayback(s_Client);
    return Plugin_Handled;
}

StartGrenadeTesterPlayback(p_Client)
{        
    new Handle:s_GrenadePlayer = GetGrenadePlayerByClient(p_Client);
    
    if(!s_GrenadePlayer)
        return;

    SetGrenadePlayerCurrentSpotIndex(s_GrenadePlayer, 0);
    SetGrenadePlayerPlaybackStarting(s_GrenadePlayer, false);
    SetGrenadePlayerPlaybackStarted(s_GrenadePlayer, true);
    
    HandleGrenadeTesterPlayback(s_GrenadePlayer);
}

FinishGrenadeTesterPlayback(p_Client, bool:p_RestorePosition = true)
{
    new Handle:s_GrenadePlayer = GetGrenadePlayerByClient(p_Client);
    
    if(!s_GrenadePlayer)
        return;
        
    KillTriggerPlaybackTimer(s_GrenadePlayer);
    KillGotoNextSpotOrFinishTimer(s_GrenadePlayer);
    KillHandlePlaybackTimer(s_GrenadePlayer);

    SetGrenadePlayerCurrentSpotIndex(s_GrenadePlayer, 0);
    SetGrenadePlayerPlaybackStarted(s_GrenadePlayer, false);
    SetGrenadePlayerAwaitingDetonation(s_GrenadePlayer, false);
    
    RefreshGrenadesMenu(p_Client);
    
    if(p_RestorePosition)
    {
        // Did our client somehow start to disconnect or leave their team?
        if(!IsClientConnected(p_Client) || !IsClientInGame(p_Client) || !IsClientOnActualTeam(p_Client))
            return;
            
        if(!IsPlayerAlive(p_Client))
            CS_RespawnPlayer(p_Client);
            
        decl Float:s_ThrowerOrigin[3];
        GetGrenadePlayerThrowerOrigin(s_GrenadePlayer, s_ThrowerOrigin);
        
        decl Float:s_ThrowerEyeAngles[3];
        GetGrenadePlayerThrowerEyeAngles(s_GrenadePlayer, s_ThrowerEyeAngles);
    
        TeleportEntity(p_Client, s_ThrowerOrigin, s_ThrowerEyeAngles, g_EmptyVector);
    }
}

// Handle grenade tester playback function (kind of like think)
HandleGrenadeTesterPlayback(Handle:p_GrenadePlayer)
{
    SetGrenadePlayerTossedGrenade(p_GrenadePlayer, -1);
    new s_Client = GetGrenadePlayerClient(p_GrenadePlayer);
    
    // Did our client somehow start to disconnect?
    if(!IsClientConnected(s_Client) || !IsClientInGame(s_Client))
        return;
    
    // Did the client leave their team?
    if(!IsClientOnActualTeam(s_Client))
        return;
        
    if(!IsPlayerAlive(s_Client))
        CS_RespawnPlayer(s_Client);
    
    RefreshGrenadesMenu(s_Client);
    
    new s_SpotIndex = GetGrenadePlayerCurrentSpotIndex(p_GrenadePlayer);
    new Handle:s_Spots = GetGrenadePlayerSpots(p_GrenadePlayer);
    new Handle:s_Spot = GetArrayCell(s_Spots, s_SpotIndex);
    
    decl Float:s_SpotOrigin[3];
    GetGrenadeTesterSpotOrigin(s_Spot, s_SpotOrigin);
    
    decl Float:s_SpotEyeAngles[3];
    GetGrenadeTesterSpotEyeAngles(s_Spot, s_SpotEyeAngles);
    
    TeleportEntity(s_Client, s_SpotOrigin, s_SpotEyeAngles, g_EmptyVector);
    
    new s_UserID = GetClientUserId(s_Client);
    
    KillHandlePlaybackTimer(p_GrenadePlayer);
    
    new Handle:s_HandlePlaybackTimer = CreateTimer(1.5, HandleGrenadeTesterPlayback2, s_UserID);
    
    SetGrenadePlayerHandlePlaybackTimer(p_GrenadePlayer, s_HandlePlaybackTimer);
}

// Timer callback
public Action:HandleGrenadeTesterPlayback2(Handle:p_Timer, any:p_UserID)
{
    new s_Client = GetClientOfUserId(p_UserID);
    new Handle:s_GrenadePlayer = GetGrenadePlayerByClient(s_Client);
    
    if(!s_GrenadePlayer)
        return Plugin_Handled;
        
    SetGrenadePlayerHandlePlaybackTimer(s_GrenadePlayer, INVALID_HANDLE);
        
    // Did our client somehow start to disconnect?
    if(!IsClientConnected(s_Client) || !IsClientInGame(s_Client))
        return Plugin_Handled;
    
    // Did the client leave their team?
    if(!IsClientOnActualTeam(s_Client))
        return Plugin_Handled;
        
    // Get the grenade type
    new CSWeaponID:s_GrenadeType = CSWeaponID:GetGrenadePlayerType(s_GrenadePlayer);
    
    // Get the projectile parameters
    decl Float:s_ProjectileOrigin[3];
    GetGrenadePlayerProjectileOrigin(s_GrenadePlayer, s_ProjectileOrigin);
    
    decl Float:s_ProjectileAngle[3];
    GetGrenadePlayerProjectileAngle(s_GrenadePlayer, s_ProjectileAngle);
    
    decl Float:s_ProjectileVelocity[3];
    GetGrenadePlayerProjectileVelocity(s_GrenadePlayer, s_ProjectileVelocity);
    
    decl Float:s_ProjectileAngularImpulse[3];
    GetGrenadePlayerProjectileAngularImpulse(s_GrenadePlayer, s_ProjectileAngularImpulse);
    
    // Toss a generic grenade
    new s_GrenadeIndex = ThrowGrenade(s_GrenadeType, s_Client, s_ProjectileOrigin, s_ProjectileAngle, s_ProjectileVelocity, s_ProjectileAngularImpulse);
    
    // Track the grenade index
    SetGrenadePlayerTossedGrenade(s_GrenadePlayer, s_GrenadeIndex);
    
    if(s_GrenadeType == CSWeapon_SMOKEGRENADE)
        MarkShortSmoke(s_GrenadeIndex);
        
    if(s_GrenadeType == CSWeapon_DECOY)
        MarkShortDecoy(s_GrenadeIndex);
    
    return Plugin_Handled;
}

bool:GotoNextSpotOrFinishPlayback(Handle:p_GrenadePlayer)
{
    if(!p_GrenadePlayer)
        return false;
        
    new s_Client = GetGrenadePlayerClient(p_GrenadePlayer);
    
    // Did our client somehow start to disconnect?
    if(!IsClientConnected(s_Client) || !IsClientInGame(s_Client))
        return false;
    
    // Did the client leave their team?
    if(!IsClientOnActualTeam(s_Client))
        return false;
        
    new s_SpotIndex = GetGrenadePlayerCurrentSpotIndex(p_GrenadePlayer);
    new Handle:s_Spots = GetGrenadePlayerSpots(p_GrenadePlayer);
    new s_SpotCount = GetArraySize(s_Spots);
    
    if(s_SpotIndex + 1 >= s_SpotCount)
        FinishGrenadeTesterPlayback(s_Client);
    else
    {
        SetGrenadePlayerCurrentSpotIndex(p_GrenadePlayer, s_SpotIndex + 1);
        HandleGrenadeTesterPlayback(p_GrenadePlayer);
    }
    
    return true;
}

// Timer callback
public Action:GotoNextSpotOrFinishPlaybackTimer(Handle:p_Timer, any:p_UserID)
{
    new s_Client = GetClientOfUserId(p_UserID);
    new Handle:s_GrenadePlayer = GetGrenadePlayerByClient(s_Client);
    
    if(!s_GrenadePlayer)
        return Plugin_Handled;
        
    SetGrenadePlayerGotoNextSpotOrFinishTimer(s_GrenadePlayer, INVALID_HANDLE);
    
    GotoNextSpotOrFinishPlayback(s_GrenadePlayer);
    return Plugin_Handled;
}

stock GetAllClientsForRecipientFilter(p_Clients[], &p_ClientCount)
{
    new s_PlayerCount = JEZ_GetPlayerCount();
    p_ClientCount = 0;
    decl Handle:s_Player;
    decl s_Client;
    
    for(new s_Index = 0; s_Index < s_PlayerCount; s_Index++)
    {
        s_Player = JEZ_GetPlayer(s_Index);
        s_Client = JEZ_GetPlayerClient(s_Player);
        
        if(!IsClientConnected(s_Client) || !IsClientInGame(s_Client))
            continue;
        
        p_Clients[p_ClientCount++] = s_Client;
    }
}

bool:ClientHasWeapon(p_Client, const String:p_Classname[])
{
    new s_WeaponCount = GetEntPropArraySize(p_Client, Prop_Send, "m_hMyWeapons");
    decl String:s_EntClass[512];
    
    for (new s_Index = 0; s_Index < s_WeaponCount; s_Index++)
    {
        new s_Weapon = GetEntPropEnt(p_Client, Prop_Send, "m_hMyWeapons", s_Index);
        
        if(s_Weapon == INVALID_ENT_REFERENCE)
            continue;
        
        GetEntityClassname(s_Weapon, s_EntClass, sizeof(s_EntClass));
        
        if(!StrEqual(s_EntClass, p_Classname, false))
            continue;
            
        return true;
    }
    
    return false;
}

RemoveClientWeapon(p_Client, const String:p_Classname[])
{
    new s_WeaponCount = GetEntPropArraySize(p_Client, Prop_Send, "m_hMyWeapons");
    decl String:s_EntClass[512];
    
    for (new s_Index = 0; s_Index < s_WeaponCount; s_Index++)
    {
        new s_Weapon = GetEntPropEnt(p_Client, Prop_Send, "m_hMyWeapons", s_Index);
        
        if(s_Weapon == INVALID_ENT_REFERENCE)
            continue;
        
        GetEntityClassname(s_Weapon, s_EntClass, sizeof(s_EntClass));
        
        if(!StrEqual(s_EntClass, p_Classname, false))
            continue;
        
        RemovePlayerItem(p_Client, s_Weapon);
    }
}

public Action:PlayerSpawnRefreshTimer(Handle:p_Timer, any:p_UserID)
{
    new s_Client = GetClientOfUserId(p_UserID);
    
    if(!IsClientConnected(s_Client) || !IsClientInGame(s_Client))
        return Plugin_Handled;
    
    RefreshGrenadesMenu(s_Client);
    return Plugin_Handled;
}

stock bool:ProcessEmitEvent(Handle:p_GrenadePlayer, CSWeaponID:p_AcceptedGrenadeType, const Float:p_ProjectileOrigin[3], const Float:p_ProjectileAngle[3], const Float:p_ProjectileVelocity[3], const Float:p_ProjectileAngularImpulse[3])
{
    if(!p_GrenadePlayer)
        return false;
        
    new s_Client = GetGrenadePlayerClient(p_GrenadePlayer);
    
    // Get the grenade type
    new CSWeaponID:s_GrenadeType = CSWeaponID:GetGrenadePlayerType(p_GrenadePlayer);
    
    // Don't track if not the appropriate grenade type
    if(s_GrenadeType != p_AcceptedGrenadeType)
        return false;
    
    // Don't track if the playback has already started
    if(GetGrenadePlayerPlaybackStarted(p_GrenadePlayer))
        return false;
        
    // Don't track if playback is not enabled
    if(!GetGrenadePlayerPlaybackEnabled(p_GrenadePlayer))
        return false;
        
    // Don't track if we are actually already awaiting a detonation
    if(GetGrenadePlayerAwaitingDetonation(p_GrenadePlayer))
        return false;
    

    SetGrenadePlayerAwaitingDetonation(p_GrenadePlayer, true);
    RefreshGrenadesMenu(s_Client);
    
    // Track the grenade
    SetGrenadePlayerProjectileOrigin(p_GrenadePlayer, p_ProjectileOrigin);
    SetGrenadePlayerProjectileAngle(p_GrenadePlayer, p_ProjectileAngle);
    SetGrenadePlayerProjectileVelocity(p_GrenadePlayer, p_ProjectileVelocity);
    SetGrenadePlayerProjectileAngularImpulse(p_GrenadePlayer, p_ProjectileAngularImpulse);
    
    // Track the thrower
    decl Float:s_PlayerOrigin[3];
    GetClientAbsOrigin(s_Client, s_PlayerOrigin);
    
    decl Float:s_PlayerEyeAngles[3];
    GetClientEyeAngles(s_Client, s_PlayerEyeAngles);
    
    SetGrenadePlayerThrowerOrigin(p_GrenadePlayer, s_PlayerOrigin);
    SetGrenadePlayerThrowerEyeAngles(p_GrenadePlayer, s_PlayerEyeAngles);
    
    return true;
}

public JEZ_OnEmitFlashGrenade(p_Client, const Float:p_Origin[3], const Float:p_Angle[3], const Float:p_Velocity[3], const Float:p_AngularImpulse[3], &bool:p_BlockEmit)
{
    if(!g_PluginEnabled)
        return;
        
    // Get the grenade player
    new Handle:s_GrenadePlayer = GetGrenadePlayerByClient(p_Client);
    
    // Validate the grenade player
    if(s_GrenadePlayer == INVALID_HANDLE)
        return;
        
    ProcessEmitEvent(s_GrenadePlayer, CSWeapon_FLASHBANG, p_Origin, p_Angle, p_Velocity, p_AngularImpulse);
}

public JEZ_OnEmitSmokeGrenade(p_Client, const Float:p_Origin[3], const Float:p_Angle[3], const Float:p_Velocity[3], const Float:p_AngularImpulse[3], &bool:p_BlockEmit)
{
    if(!g_PluginEnabled)
        return;
        
    // Get the grenade player
    new Handle:s_GrenadePlayer = GetGrenadePlayerByClient(p_Client);
    
    // Validate the grenade player
    if(s_GrenadePlayer == INVALID_HANDLE)
        return;
        
    ProcessEmitEvent(s_GrenadePlayer, CSWeapon_SMOKEGRENADE, p_Origin, p_Angle, p_Velocity, p_AngularImpulse);
}

public JEZ_OnEmitMolotovGrenade(p_Client, const Float:p_Origin[3], const Float:p_Angle[3], const Float:p_Velocity[3], const Float:p_AngularImpulse[3], &bool:p_BlockEmit)
{
    if(!g_PluginEnabled)
        return;
        
    // Get the grenade player
    new Handle:s_GrenadePlayer = GetGrenadePlayerByClient(p_Client);
    
    // Validate the grenade player
    if(s_GrenadePlayer == INVALID_HANDLE)
        return;
        
    ProcessEmitEvent(s_GrenadePlayer, CSWeapon_MOLOTOV, p_Origin, p_Angle, p_Velocity, p_AngularImpulse);
}

public JEZ_OnEmitIncendiaryGrenade(p_Client, const Float:p_Origin[3], const Float:p_Angle[3], const Float:p_Velocity[3], const Float:p_AngularImpulse[3], &bool:p_BlockEmit)
{
    if(!g_PluginEnabled)
        return;
        
    // Get the grenade player
    new Handle:s_GrenadePlayer = GetGrenadePlayerByClient(p_Client);
    
    // Validate the grenade player
    if(s_GrenadePlayer == INVALID_HANDLE)
        return;
        
    ProcessEmitEvent(s_GrenadePlayer, CSWeapon_INCGRENADE, p_Origin, p_Angle, p_Velocity, p_AngularImpulse);
}

public JEZ_OnEmitDecoyGrenade(p_Client, const Float:p_Origin[3], const Float:p_Angle[3], const Float:p_Velocity[3], const Float:p_AngularImpulse[3], &bool:p_BlockEmit)
{
    if(!g_PluginEnabled)
        return;
        
    // Get the grenade player
    new Handle:s_GrenadePlayer = GetGrenadePlayerByClient(p_Client);
    
    // Validate the grenade player
    if(s_GrenadePlayer == INVALID_HANDLE)
        return;
        
    ProcessEmitEvent(s_GrenadePlayer, CSWeapon_DECOY, p_Origin, p_Angle, p_Velocity, p_AngularImpulse);
}

public JEZ_OnEmitHEGrenade(p_Client, const Float:p_Origin[3], const Float:p_Angle[3], const Float:p_Velocity[3], const Float:p_AngularImpulse[3], &bool:p_BlockEmit)
{
    if(!g_PluginEnabled)
        return;
        
    // Get the grenade player
    new Handle:s_GrenadePlayer = GetGrenadePlayerByClient(p_Client);
    
    // Validate the grenade player
    if(s_GrenadePlayer == INVALID_HANDLE)
        return;
        
    ProcessEmitEvent(s_GrenadePlayer, CSWeapon_HEGRENADE, p_Origin, p_Angle, p_Velocity, p_AngularImpulse);
}

public Action:OnGrenadesDrawThink(Handle:p_Timer, any:p_Data)
{
    new s_GrenadePlayerCount = GetArraySize(g_GrenadePlayers);
    decl Handle:s_GrenadePlayer;
    decl Handle:s_Player;
    decl s_Client;
    decl Handle:s_Spots;
    decl s_SpotCount;
    decl Handle:s_Spot;
    decl Float:s_SpotOrigin[3];
    decl Float:s_SpotPoint1[3];
    decl Float:s_SpotPoint2[3];
    decl Float:s_Origin[3];
    decl Handle:s_ClosestSpot;
    
    for(new s_Index = 0; s_Index < s_GrenadePlayerCount; s_Index++)
    {
        s_GrenadePlayer = GetArrayCell(g_GrenadePlayers, s_Index);
        s_Player = GetGrenadePlayerPlayer(s_GrenadePlayer);
        s_Client = JEZ_GetPlayerClient(s_Player);
        
        if(!IsClientConnected(s_Client) || !IsClientInGame(s_Client))
            continue;
            
        GetClientAbsOrigin(s_Client, s_Origin);
        
        s_Spots = GetGrenadePlayerSpots(s_GrenadePlayer);
        s_SpotCount = GetArraySize(s_Spots);
        
        s_ClosestSpot = GetClosestGrenadeTesterSpot(s_Client, s_Spots);
        
        for(new s_SpotIndex = 0; s_SpotIndex < s_SpotCount; s_SpotIndex++)
        {
            s_Spot = GetArrayCell(s_Spots, s_SpotIndex);
            
            GetGrenadeTesterSpotOrigin(s_Spot, s_SpotOrigin);
            CreateBoundingBoxForPlayerOrigin(s_SpotOrigin, s_SpotPoint1, s_SpotPoint2);
            
            if(s_Spot == s_ClosestSpot)
                DrawRectangle(s_Client, s_SpotPoint1, s_SpotPoint2, g_YellowColor);
            else
                DrawRectangle(s_Client, s_SpotPoint1, s_SpotPoint2, g_WhiteColor);
        }
    }
}

public JEZ_OnTakeDamageHook(p_Victim, &p_Attacker, &p_Inflictor, &Float:p_Damage, &p_DamageType, &p_Weapon, Float:p_DamageForce[3], Float:p_DamagePosition[3], p_DamageCustom, &Action:p_Result)
{
    new Handle:s_GrenadePlayer = GetGrenadePlayerByClient(p_Victim);
    
    if(!s_GrenadePlayer)
        return;
    
    new GrenadeGodMode:s_GodMode = GrenadeGodMode:GetGrenadePlayerGodMode(s_GrenadePlayer);
    
    new String:s_WeaponClassname[256];
    new String:s_InflictorClassname[256];
    
    if(p_Weapon > 0)
        GetEntityClassname(p_Weapon, s_WeaponClassname, sizeof(s_WeaponClassname));
        
    if(p_Inflictor > 0)
        GetEntityClassname(p_Inflictor, s_InflictorClassname, sizeof(s_InflictorClassname));
      
    if(s_GodMode == GrenadeGodMode_ON)
    {
        p_Result = Plugin_Handled;
        return;
    }
        
    if(s_GodMode == GrenadeGodMode_SELFONLY && p_Attacker != p_Victim && p_Attacker > 0)
    {
        p_Result = Plugin_Handled;
        return;
    }
    
    if(s_GodMode == GrenadeGodMode_NADEONLY)
    {
        new bool:s_IsFireGrenadeDamage = (p_Attacker > 0 && StrEqual(s_InflictorClassname, "inferno", false));
        new bool:s_InflictorIsProjectile = StrEqual(s_InflictorClassname, "hegrenade_projectile", false) ||
            StrEqual(s_InflictorClassname, "smokegrenade_projectile", false) ||
            StrEqual(s_InflictorClassname, "flashbang_projectile", false) ||
            StrEqual(s_InflictorClassname, "decoy_projectile", false) ||
            StrEqual(s_InflictorClassname, "molotov_projectile", false);
        
        if(!s_InflictorIsProjectile && !s_IsFireGrenadeDamage)
        {
            p_Result = Plugin_Handled;
            return;
        }
        
        return;
    }
    
    if(s_GodMode == GrenadeGodMode_GROUP && !IsPlayerInMyGroup(p_Attacker, p_Victim))
    {
        p_Result = Plugin_Handled;
        return;
    }
    
    if(s_GodMode == GrenadeGodMode_GROUPNADEONLY)
    {
        new bool:s_IsGrouped = IsPlayerInMyGroup(p_Attacker, p_Victim);
        
        new bool:s_InflictorIsProjectile = StrEqual(s_InflictorClassname, "hegrenade_projectile", false) ||
            StrEqual(s_InflictorClassname, "smokegrenade_projectile", false) ||
            StrEqual(s_InflictorClassname, "flashbang_projectile", false) ||
            StrEqual(s_InflictorClassname, "decoy_projectile", false) ||
            StrEqual(s_InflictorClassname, "molotov_projectile", false);
        
        new bool:s_IsFireGrenadeDamage = (p_Attacker > 0 && StrEqual(s_InflictorClassname, "inferno", false));
        new bool:s_IsNadeDamage = s_IsFireGrenadeDamage || s_InflictorIsProjectile;
        
        if(!s_IsGrouped || !s_IsNadeDamage)
        {
            p_Result = Plugin_Handled;
            return;
        }
        
        return;
    }
}

public JEZ_OnPlayerConnect(Handle:p_Player, &bool:p_ShouldKick, String:p_RejectMsg[], p_MaxLen)
{
    if(!g_PluginEnabled)
        return;
    
    new Handle:s_GrenadeTesterSpots = CreateArray();
    new Handle:s_GrenadePlayer = CreateGrenadePlayer(p_Player, s_GrenadeTesterSpots, false, false, false, false, g_EmptyVector, g_EmptyVector, g_EmptyVector, g_EmptyVector, 0, INVALID_HANDLE, INVALID_HANDLE, _:CSWeapon_FLASHBANG, g_EmptyVector, g_EmptyVector, false, -1, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, _:GrenadeGodMode_OFF, 0, false, _:GrenadeProjectileMode_ALL, _:GrenadeProjectileMode_ALL, _:GrenadeProjectileMode_ALL, _:GrenadeProjectileMode_ALL, _:GrenadeProjectileMode_ALL, _:GrenadeProjectileMode_ALL, _:GrenadePlayerMode_ALL);
    PushArrayCell(g_GrenadePlayers, s_GrenadePlayer);
}

public JEZ_OnPlayerDisconnect(Handle:p_Player)
{
    if(!g_PluginEnabled)
        return;
    
    decl s_GrenadePlayerIndex;
    new Handle:s_GrenadePlayer = FindGrenadePlayerByPlayer(g_GrenadePlayers, p_Player, 0, s_GrenadePlayerIndex);
    
    if(s_GrenadePlayer == INVALID_HANDLE)
        return;
        
    // Clean up
    CloseHandle(s_GrenadePlayer);
    RemoveFromArray(g_GrenadePlayers, s_GrenadePlayerIndex);
}

public JEZ_OnCustomChat(Handle:p_Player, const String:p_FirstWord[], const String:p_NextWords[], &bool:p_BlockChat)
{
    if(!g_PluginEnabled)
        return;
    
    new s_Client = JEZ_GetPlayerClient(p_Player);
    
    if(StrEqual("!menu", p_FirstWord, false))
        ShowGrenadesMenu(s_Client);
    else if(StrEqual("!addspot", p_FirstWord, false))
    {
        DoAddSpot(s_Client);
        RefreshGrenadesMenu(s_Client);
        p_BlockChat = true;
    }
    else if(StrEqual("!removespot", p_FirstWord, false))
    {
        DoRemoveSpot(s_Client);
        RefreshGrenadesMenu(s_Client);
        p_BlockChat = true;
    }
    else if(StrEqual("!playback", p_FirstWord, false))
    {
        DoTogglePlayback(s_Client);
        RefreshGrenadesMenu(s_Client);
        p_BlockChat = true;
    }
    else if(StrEqual("!fastforward", p_FirstWord, false))
    {
        DoFastForward(s_Client);
        RefreshGrenadesMenu(s_Client);
        p_BlockChat = true;
    }
    else if(StrEqual("!grenadetype", p_FirstWord, false))
    {
        DoToggleGrenadeType(s_Client);
        RefreshGrenadesMenu(s_Client);
        p_BlockChat = true;
    }
    else if(StrEqual("!switchtogrenade", p_FirstWord, false))
    {
        DoSwitchToGrenade(s_Client);
        RefreshGrenadesMenu(s_Client);
        p_BlockChat = true;
    }
    else if(StrEqual("!god", p_FirstWord, false))
    {
        DoToggleGodMode(s_Client);
        RefreshGrenadesMenu(s_Client);
        p_BlockChat = true;
    }
    else if(StrEqual("!noclip", p_FirstWord, false))
    {
        DoToggleNoClip(s_Client);
        RefreshGrenadesMenu(s_Client);
        p_BlockChat = true;
    }
    else if(StrEqual("!playermode", p_FirstWord, false))
    {
        DoTogglePlayerMode(s_Client);
        RefreshGrenadesMenu(s_Client);
        p_BlockChat = true;
    }
    else if(StrEqual("!flashmode", p_FirstWord, false))
    {
        DoToggleFlashMode(s_Client);
        RefreshGrenadesMenu(s_Client);
        p_BlockChat = true;
    }
    else if(StrEqual("!smokemode", p_FirstWord, false))
    {
        DoToggleSmokeMode(s_Client);
        RefreshGrenadesMenu(s_Client);
        p_BlockChat = true;
    }
    else if(StrEqual("!hegrenademode", p_FirstWord, false))
    {
        DoToggleHEGrenadeMode(s_Client);
        RefreshGrenadesMenu(s_Client);
        p_BlockChat = true;
    }
    else if(StrEqual("!decoymode", p_FirstWord, false))
    {
        DoToggleDecoyMode(s_Client);
        RefreshGrenadesMenu(s_Client);
        p_BlockChat = true;
    }
    else if(StrEqual("!molotovmode", p_FirstWord, false))
    {
        DoToggleMolotovMode(s_Client);
        RefreshGrenadesMenu(s_Client);
        p_BlockChat = true;
    }
    else if(StrEqual("!incendiarymode", p_FirstWord, false))
    {
        DoToggleIncendiaryMode(s_Client);
        RefreshGrenadesMenu(s_Client);
        p_BlockChat = true;
    }
}

stock Handle:GetGrenadePlayerByClient(p_Client)
{
    new Handle:s_Player = JEZ_GetPlayerByClient(p_Client);
    
    if(s_Player == INVALID_HANDLE)
        return INVALID_HANDLE;
    
    new Handle:s_GrenadePlayer = FindGrenadePlayerByPlayer(g_GrenadePlayers, s_Player);
    
    return s_GrenadePlayer;
}

stock Handle:GetGrenadePlayerByUserID(p_UserID)
{
    new Handle:s_Player = JEZ_GetPlayerByUserID(p_UserID);
    
    if(s_Player == INVALID_HANDLE)
        return INVALID_HANDLE;
    
    new Handle:s_GrenadePlayer = FindGrenadePlayerByPlayer(g_GrenadePlayers, s_Player);
    
    return s_GrenadePlayer;
}

public JEZ_OnMapStart(const String:p_Map[])
{
    if(!g_PluginEnabled)
        return;
    
    g_LaserBeamSprite = PrecacheModel("materials/sprites/laserbeam.vmt");
    g_CustomExplosionParticles = PrecacheGeneric("particles/jez_explosions_fx.pcf", true);
    PrecacheGeneric("particles/explosions_fx.pcf", true);
    
    AddFileToDownloadsTable("particles/jez_explosions_fx.pcf");
    PrintToServer("Custom index is: %d", g_CustomExplosionParticles);
    
    g_EffectDispatchStringTable = FindStringTable("EffectDispatch");
    
    if(g_EffectDispatchStringTable == INVALID_STRING_TABLE)
    {
        PrintToServer("Error looking up EffectDispatch string table!");
        return;
    }
    
    PrintToServer("g_EffectDispatchStringTable = %d", g_EffectDispatchStringTable);
    
    g_ParticleEffectIndex = FindStringIndex(g_EffectDispatchStringTable, "ParticleEffect");
       
    if(g_ParticleEffectIndex == INVALID_STRING_INDEX)
    {
        PrintToServer("Error looking up ParticleEffect in EffectDispatch string table!");
        return;
    }
    
    PrintToServer("g_ParticleEffectIndex = %d", g_ParticleEffectIndex);
    
    g_ParticleEffectNamesStringTable = FindStringTable("ParticleEffectNames");
    
    if(g_ParticleEffectNamesStringTable == INVALID_STRING_TABLE)
    {
        PrintToServer("Error looking up ParticleEffectNames string table!");
        return;
    }
    
    PrintToServer("g_ParticleEffectNamesStringTable = %d", g_ParticleEffectNamesStringTable);
    
    // test: what is 129 97 27
    decl String:s_PartName[512];
    ReadStringTable(g_ParticleEffectNamesStringTable, 129, s_PartName, sizeof(s_PartName));
    PrintToServer("Particle: %s", s_PartName);
    ReadStringTable(g_ParticleEffectNamesStringTable, 97, s_PartName, sizeof(s_PartName));
    PrintToServer("Particle: %s", s_PartName);
    ReadStringTable(g_ParticleEffectNamesStringTable, 27, s_PartName, sizeof(s_PartName));
    PrintToServer("Particle: %s", s_PartName);
    
    g_ExplosionSmokeGrenadeIndex = FindStringIndex(g_ParticleEffectNamesStringTable, "explosion_smokegrenade");
    
    if(g_ExplosionSmokeGrenadeIndex == INVALID_STRING_INDEX)
    {
        PrintToServer("Error looking up explosion_smokegrenade in ParticleEffectNames string table!");
        return;
    }
    
    PrintToServer("g_ExplosionSmokeGrenadeIndex = %d", g_ExplosionSmokeGrenadeIndex);
    
    g_CustomExplosionSmokeGrenadeIndex = FindStringIndex(g_ParticleEffectNamesStringTable, "jez_explosion_smokegrenade");
    
    if(g_CustomExplosionSmokeGrenadeIndex == INVALID_STRING_INDEX)
    {
        PrintToServer("Error looking up jez_explosion_smokegrenade in ParticleEffectNames string table!");
        return;
    }
    
    PrintToServer("g_ExplosionSmokeGrenadeIndex = %d", g_ExplosionSmokeGrenadeIndex);
    
    g_SmokeScreenIndex = FindStringIndex(g_ParticleEffectNamesStringTable, "explosion_screen_smokegrenade_new");
    
    if(g_SmokeScreenIndex == INVALID_STRING_INDEX)
    {
        PrintToServer("Error looking up explosion_screen_smokegrenade_new in ParticleEffectNames string table!");
        return;
    }
    
    PrintToServer("g_SmokeScreenIndex = %d", g_SmokeScreenIndex);
    
    g_CustomSmokeScreenIndex = FindStringIndex(g_ParticleEffectNamesStringTable, "jez_explosion_screen_smokegrenade_new");
    
    if(g_CustomSmokeScreenIndex == INVALID_STRING_INDEX)
    {
        PrintToServer("Error looking up jez_explosion_screen_smokegrenade_new in ParticleEffectNames string table!");
        return;
    }
    
    PrintToServer("g_CustomSmokeScreenIndex = %d", g_CustomSmokeScreenIndex);
    
    new s_ParticlePrecacheGlow = PrecacheModel("particle/particle_glow_01.vmt", true);
    
    PrintToServer("s_ParticlePrecacheGlow = %d", s_ParticlePrecacheGlow);
    
    new s_DecalStringTable = FindStringTable("decalprecache");
    
    if(s_DecalStringTable == INVALID_STRING_TABLE)
    {
        PrintToServer("Error finding decalprecache stringtable.");
        return;
    }
    
    g_MolotovScorchDecal = PrecacheDecal("decals/MolotovScorch", true);
    PrintToServer("g_MolotovScorchDecal = %d", g_MolotovScorchDecal);
    
    g_ScorchDecal = PrecacheDecal("decals/scorch1_subrect", true);
    PrintToServer("g_ScorchDecal = %d", g_ScorchDecal);
}

bool:IsIncendiary(p_Molotov)
{
    decl String:s_ModelName[1024];
    GetEntPropString(p_Molotov, Prop_Data, "m_ModelName", s_ModelName, sizeof(s_ModelName));

    return !StrEqual(s_ModelName, "models/Weapons/w_eq_molotov_thrown.mdl");
}

public JEZ_OnBuildHelpMenu(Handle:p_Player, const String:p_PageKey[], Handle:p_HelpMenu)
{
    new bool:s_IsRootPage = strlen(p_PageKey) == 0;
    new s_Client = JEZ_GetPlayerClient(p_Player);
    
    // Buffer for our menu display item
    new String:s_MenuDisplayItem[512];
    
    if(s_IsRootPage)
    {
        Format(s_MenuDisplayItem, sizeof(s_MenuDisplayItem), "%T", "Grenades help menu item commands", s_Client);
        AddMenuItem(p_HelpMenu, "grenade_commands", s_MenuDisplayItem);
    }
    else if(StrEqual(p_PageKey, "grenade_commands", false))
    {
        SetMenuTitle(p_HelpMenu, "%T", "Grenades help menu title", s_Client);
            
        Format(s_MenuDisplayItem, sizeof(s_MenuDisplayItem), "%T", "Grenades help menu menu item", s_Client);
        AddMenuItem(p_HelpMenu, "grenade_commands_menu_detail", s_MenuDisplayItem);
        
        Format(s_MenuDisplayItem, sizeof(s_MenuDisplayItem), "%T", "Grenades help menu addspot item", s_Client);
        AddMenuItem(p_HelpMenu, "grenade_commands_addspot_detail", s_MenuDisplayItem);
        
        Format(s_MenuDisplayItem, sizeof(s_MenuDisplayItem), "%T", "Grenades help menu removespot item", s_Client);
        AddMenuItem(p_HelpMenu, "grenade_commands_removespot_detail", s_MenuDisplayItem);
        
        Format(s_MenuDisplayItem, sizeof(s_MenuDisplayItem), "%T", "Grenades help menu playback item", s_Client);
        AddMenuItem(p_HelpMenu, "grenade_commands_playback_detail", s_MenuDisplayItem);
        
        Format(s_MenuDisplayItem, sizeof(s_MenuDisplayItem), "%T", "Grenades help menu fastforward item", s_Client);
        AddMenuItem(p_HelpMenu, "grenade_commands_fastforward_detail", s_MenuDisplayItem);
        
        Format(s_MenuDisplayItem, sizeof(s_MenuDisplayItem), "%T", "Grenades help menu grenadetype item", s_Client);
        AddMenuItem(p_HelpMenu, "grenade_commands_grenadetype_detail", s_MenuDisplayItem);
        
        Format(s_MenuDisplayItem, sizeof(s_MenuDisplayItem), "%T", "Grenades help menu switchtogrenade item", s_Client);
        AddMenuItem(p_HelpMenu, "grenade_commands_switchtogrenade_detail", s_MenuDisplayItem);
        
        Format(s_MenuDisplayItem, sizeof(s_MenuDisplayItem), "%T", "Grenades help menu god item", s_Client);
        AddMenuItem(p_HelpMenu, "grenade_commands_god_detail", s_MenuDisplayItem);
        
        Format(s_MenuDisplayItem, sizeof(s_MenuDisplayItem), "%T", "Grenades help menu noclip item", s_Client);
        AddMenuItem(p_HelpMenu, "grenade_commands_noclip_detail", s_MenuDisplayItem);
        
        Format(s_MenuDisplayItem, sizeof(s_MenuDisplayItem), "%T", "Grenades help menu playermode item", s_Client);
        AddMenuItem(p_HelpMenu, "grenade_commands_playermode_detail", s_MenuDisplayItem);
        
        Format(s_MenuDisplayItem, sizeof(s_MenuDisplayItem), "%T", "Grenades help menu flashmode item", s_Client);
        AddMenuItem(p_HelpMenu, "grenade_commands_flashmode_detail", s_MenuDisplayItem);
        
        Format(s_MenuDisplayItem, sizeof(s_MenuDisplayItem), "%T", "Grenades help menu smokemode item", s_Client);
        AddMenuItem(p_HelpMenu, "grenade_commands_smokemode_detail", s_MenuDisplayItem);
        
        Format(s_MenuDisplayItem, sizeof(s_MenuDisplayItem), "%T", "Grenades help menu hegrenademode item", s_Client);
        AddMenuItem(p_HelpMenu, "grenade_commands_hegrenademode_detail", s_MenuDisplayItem);
        
        Format(s_MenuDisplayItem, sizeof(s_MenuDisplayItem), "%T", "Grenades help menu decoymode item", s_Client);
        AddMenuItem(p_HelpMenu, "grenade_commands_decoymode_detail", s_MenuDisplayItem);
        
        Format(s_MenuDisplayItem, sizeof(s_MenuDisplayItem), "%T", "Grenades help menu molotovmode item", s_Client);
        AddMenuItem(p_HelpMenu, "grenade_commands_molotovmode_detail", s_MenuDisplayItem);
        
        Format(s_MenuDisplayItem, sizeof(s_MenuDisplayItem), "%T", "Grenades help menu incendiarymode item", s_Client);
        AddMenuItem(p_HelpMenu, "grenade_commands_incendiarymode_detail", s_MenuDisplayItem);
    }
    else if(StrEqual(p_PageKey, "grenade_commands_menu_detail", false))
    {
        SetMenuTitle(p_HelpMenu, "%T", "Grenades help menu menu title", s_Client);
    }
    else if(StrEqual(p_PageKey, "grenade_commands_addspot_detail", false))
    {
        SetMenuTitle(p_HelpMenu, "%T", "Grenades help menu addspot title", s_Client);
    }
    else if(StrEqual(p_PageKey, "grenade_commands_removespot_detail", false))
    {
        SetMenuTitle(p_HelpMenu, "%T", "Grenades help menu removespot title", s_Client);
    }
    else if(StrEqual(p_PageKey, "grenade_commands_playback_detail", false))
    {
        SetMenuTitle(p_HelpMenu, "%T", "Grenades help menu playback title", s_Client);
    }
    else if(StrEqual(p_PageKey, "grenade_commands_fastforward_detail", false))
    {
        SetMenuTitle(p_HelpMenu, "%T", "Grenades help menu fastforward title", s_Client);
    }
    else if(StrEqual(p_PageKey, "grenade_commands_grenadetype_detail", false))
    {
        SetMenuTitle(p_HelpMenu, "%T", "Grenades help menu grenadetype title", s_Client);
    }
    else if(StrEqual(p_PageKey, "grenade_commands_switchtogrenade_detail", false))
    {
        SetMenuTitle(p_HelpMenu, "%T", "Grenades help menu switchtogrenade title", s_Client);
    }
    else if(StrEqual(p_PageKey, "grenade_commands_god_detail", false))
    {
        SetMenuTitle(p_HelpMenu, "%T", "Grenades help menu god title", s_Client);
    }
    else if(StrEqual(p_PageKey, "grenade_commands_noclip_detail", false))
    {
        SetMenuTitle(p_HelpMenu, "%T", "Grenades help menu noclip title", s_Client);
    }
    else if(StrEqual(p_PageKey, "grenade_commands_playermode_detail", false))
    {
        SetMenuTitle(p_HelpMenu, "%T", "Grenades help menu playermode title", s_Client);
    }
    else if(StrEqual(p_PageKey, "grenade_commands_flashmode_detail", false))
    {
        SetMenuTitle(p_HelpMenu, "%T", "Grenades help menu flashmode title", s_Client);
    }
    else if(StrEqual(p_PageKey, "grenade_commands_smokemode_detail", false))
    {
        SetMenuTitle(p_HelpMenu, "%T", "Grenades help menu smokemode title", s_Client);
    }
    else if(StrEqual(p_PageKey, "grenade_commands_hegrenademode_detail", false))
    {
        SetMenuTitle(p_HelpMenu, "%T", "Grenades help menu hegrenademode title", s_Client);
    }
    else if(StrEqual(p_PageKey, "grenade_commands_decoymode_detail", false))
    {
        SetMenuTitle(p_HelpMenu, "%T", "Grenades help menu decoymode title", s_Client);
    }
    else if(StrEqual(p_PageKey, "grenade_commands_molotovmode_detail", false))
    {
        SetMenuTitle(p_HelpMenu, "%T", "Grenades help menu molotovmode title", s_Client);
    }
    else if(StrEqual(p_PageKey, "grenade_commands_incendiarymode_detail", false))
    {
        SetMenuTitle(p_HelpMenu, "%T", "Grenades help menu incendiarymode title", s_Client);
    }
}

public JEZ_OnHelpMenuQueryBackPageKey(Handle:p_Player, const String:p_CurrentPageKey[], &bool:p_FoundBackKey, String:p_BackPageKey[], p_BackPageKeyMaxLen)
{
    if(StrEqual(p_CurrentPageKey, "grenade_commands_menu_detail", false) || StrEqual(p_CurrentPageKey, "grenade_commands_addspot_detail", false)
        || StrEqual(p_CurrentPageKey, "grenade_commands_removespot_detail", false) || StrEqual(p_CurrentPageKey, "grenade_commands_playback_detail", false)
        || StrEqual(p_CurrentPageKey, "grenade_commands_fastforward_detail", false) || StrEqual(p_CurrentPageKey, "grenade_commands_grenadetype_detail", false)
        || StrEqual(p_CurrentPageKey, "grenade_commands_switchtogrenade_detail", false) || StrEqual(p_CurrentPageKey, "grenade_commands_god_detail", false)
        || StrEqual(p_CurrentPageKey, "grenade_commands_noclip_detail", false) || StrEqual(p_CurrentPageKey, "grenade_commands_playermode_detail", false)
        || StrEqual(p_CurrentPageKey, "grenade_commands_flashmode_detail", false) || StrEqual(p_CurrentPageKey, "grenade_commands_smokemode_detail", false)
        || StrEqual(p_CurrentPageKey, "grenade_commands_hegrenademode_detail", false) || StrEqual(p_CurrentPageKey, "grenade_commands_decoymode_detail", false)
        || StrEqual(p_CurrentPageKey, "grenade_commands_molotovmode_detail", false) || StrEqual(p_CurrentPageKey, "grenade_commands_incendiarymode_detail", false))
    {
        p_FoundBackKey = true;
        Format(p_BackPageKey, p_BackPageKeyMaxLen, "grenade_commands");
        return;
    }
}