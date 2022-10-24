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

#define MAX_VWORLDS 64
new Handle:g_VWorlds[MAX_VWORLDS];

public OnPluginStart()
{
    for(new x = 0; x < MAX_VWORLDS; x++)
        g_VWorlds[x] = JEZ_CreateVWorld();
}

public JEZ_OnCustomChat(Handle:p_Player, const String:p_FirstWord[], const String:p_NextWords[], &bool:p_BlockChat)
{   
    new s_Client = JEZ_GetPlayerClient(p_Player);
    
    if(StrEqual("!vworld", p_FirstWord, false))
    {
        new s_VWorldIndex = StringToInt(p_NextWords) - 1;
        
        if(s_VWorldIndex < 0)
        {
            JEZ_RemovePlayerFromVWorld(p_Player);
            JEZ_Message(s_Client, "Back to \"reality\"!");
            return;
        }
        
        JEZ_AddPlayerToVWorld(g_VWorlds[s_VWorldIndex], p_Player);
        
        JEZ_Message(s_Client, "Bam! You're in a vworld! [#%d index %d]", s_VWorldIndex + 1, s_VWorldIndex);
    }
}