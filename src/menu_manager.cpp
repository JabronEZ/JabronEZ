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

#include "extension.h"
#include "menu_manager.h"
#include "player.h"
#include "player_manager.h"
#include "translations.h"

MenuManager::MenuManager(
        IMenuManager *smMenuManager,
        IExtension *extension)
{
    _smMenuManager = smMenuManager;
    _extension = extension;
    _menus.clear();
}

MenuManager::~MenuManager()
{
    for (auto menu: _menus)
    {
        DestroyMenuSafely(menu);
    }

    _menus.clear();
}

void MenuManager::OnMenuSelect2(
        IBaseMenu *menu,
        int client,
        unsigned int item,
        unsigned int item_on_page)
{
    Player *player = g_JabronEZ.GetPlayerManager()->GetPlayerByClientIndex(client);

    if (player == nullptr)
    {
        return;
    }

    ItemDrawInfo itemInfo;
    const char *itemKey = menu->GetItemInfo(item, &itemInfo);

    if (itemKey == nullptr)
    {
        return;
    }

    auto pageNumber = (size_t)(((int)(floor((float)item / 6.0f))) + 1);
    player->SetGrenadeMenuPage(pageNumber);

     if (strcmp(itemKey, "add_spot") == 0)
         player->DoAddSpot();
     else if (strcmp(itemKey, "remove_spot") == 0)
         player->DoRemoveSpot();
     else if (strcmp(itemKey, "playback") == 0)
         player->DoTogglePlayback();
     else if (strcmp(itemKey, "fast_forward") == 0)
         player->DoFastForward();
     else if (strcmp(itemKey, "switch_to_grenade") == 0)
         player->DoSwitchToGrenade();
     else if (strcmp(itemKey, "god") == 0)
         player->DoToggleGodMode();
     else if (strcmp(itemKey, "noclip") == 0)
         player->DoToggleNoClip();

    OpenMenu(player, pageNumber);
}

void MenuManager::OnMenuEnd(SourceMod::IBaseMenu *menu, SourceMod::MenuEndReason reason)
{
    //    if(p_Action == MenuAction_Cancel)
    //    {
    //        new s_Client = p_Param1;
    //
    //        new Handle:s_GrenadePlayer = GetGrenadePlayerByClient(s_Client);
    //
    //        if(!s_GrenadePlayer)
    //            return;
    //
    //    }

    DestroyMenuSafely(menu);
}

void MenuManager::DestroyMenuSafely(SourceMod::IBaseMenu *menu)
{
    NotifyPlayerMenuDestroyed(menu);

    if (menu->GetHandle() == BAD_HANDLE)
    {
        return;
    }

    HandleSecurity sec;

    sec.pIdentity = nullptr;
    sec.pOwner = _extension->GetIdentity();

    handlesys->FreeHandle(menu->GetHandle(), &sec);
}

void MenuManager::OnMenuDestroy(SourceMod::IBaseMenu *menu)
{
    for (size_t menuIndex = 0; menuIndex < _menus.size(); menuIndex++)
    {
        auto checkMenu = _menus.at(menuIndex);

        if (checkMenu == menu)
        {
            auto iter = _menus.iterAt(menuIndex);
            _menus.erase(iter);
            break;
        }
    }
}

void SetMenuTitle(IBaseMenu *menu, int clientIndex, int spotCount)
{
    char menuTitle[1024];

    g_JabronEZ.GetTranslations()->FormatTranslated(
            menuTitle,
            sizeof(menuTitle),
            "%T",
            3,
            nullptr,
            "Grenades menu title",
            &clientIndex,
            &spotCount);

    menu->SetDefaultTitle(menuTitle);
}

void AppendAddSpotMenuItem(IBaseMenu *menu, int clientIndex, Player *player)
{
    char addSpotMenuItem[512];

    g_JabronEZ.GetTranslations()->FormatTranslated(
            addSpotMenuItem,
            sizeof(addSpotMenuItem),
            "%T",
            2,
            nullptr,
            "Grenades menu add spot",
            &clientIndex);

    menu->AppendItem("add_spot", ItemDrawInfo(
                                         addSpotMenuItem,
                                         player->GetGrenadePlaybackStarted()
                                                         || player->GetGrenadePlaybackStarting()
                                                         || player->GetGrenadeAwaitingDetonation()
                                                 ? ITEMDRAW_DISABLED
                                                 : ITEMDRAW_DEFAULT));
}

void AppendRemoveSpotMenuItem(IBaseMenu *menu, int clientIndex, Player *player, int spotCount)
{
    char removeSpotMenuItem[512];

    g_JabronEZ.GetTranslations()->FormatTranslated(
            removeSpotMenuItem,
            sizeof(removeSpotMenuItem),
            "%T",
            2,
            nullptr,
            "Grenades menu remove spot",
            &clientIndex);

    menu->AppendItem("remove_spot", ItemDrawInfo(
                                            removeSpotMenuItem,
                                            player->GetGrenadePlaybackStarted()
                                                            || player->GetGrenadePlaybackStarting()
                                                            || player->GetGrenadeAwaitingDetonation()
                                                            || spotCount == 0
                                                    ? ITEMDRAW_DISABLED
                                                    : ITEMDRAW_DEFAULT));
}

void AppendPlaybackMenuItem(
        IBaseMenu *menu,
        int clientIndex,
        Player *player,
        int spotCount,
        const char *onPhrase,
        const char *offPhrase)
{
    char playbackMenuItem[512];

    g_JabronEZ.GetTranslations()->FormatTranslated(
            playbackMenuItem,
            sizeof(playbackMenuItem),
            "%T",
            3,
            nullptr,
            "Grenades menu playback",
            &clientIndex,
            player->GetGrenadePlaybackEnabled() ? onPhrase : offPhrase);

    menu->AppendItem("playback", ItemDrawInfo(
                                         playbackMenuItem,
                                         player->GetGrenadePlaybackStarted()
                                                         || player->GetGrenadePlaybackStarting()
                                                         || player->GetGrenadeAwaitingDetonation()
                                                         || spotCount == 0
                                                 ? ITEMDRAW_DISABLED
                                                 : ITEMDRAW_DEFAULT));
}

void AppendFastForwardMenuItem(
        IBaseMenu *menu,
        int clientIndex,
        Player *player)
{
    char fastForwardMenuItem[512];

    g_JabronEZ.GetTranslations()->FormatTranslated(
            fastForwardMenuItem,
            sizeof(fastForwardMenuItem),
            "%T",
            2,
            nullptr,
            "Grenades menu fast forward",
            &clientIndex);

    menu->AppendItem("fast_forward", ItemDrawInfo(
                                             fastForwardMenuItem,
                                             player->GetGrenadePlaybackStarted()
                                                             || player->GetGrenadePlaybackStarting()
                                                             || player->GetGrenadeAwaitingDetonation()
                                                     ? ITEMDRAW_DEFAULT
                                                     : ITEMDRAW_DISABLED));
}

void AppendSwitchToGrenadeMenuItem(
        IBaseMenu *menu,
        int clientIndex,
        Player *player)
{
    char switchToGrenadeMenuItem[512];

    g_JabronEZ.GetTranslations()->FormatTranslated(
            switchToGrenadeMenuItem,
            sizeof(switchToGrenadeMenuItem),
            "%T",
            2,
            nullptr,
            "Grenades menu switch to grenade",
            &clientIndex);

    menu->AppendItem("switch_to_grenade", ItemDrawInfo(
                                                  switchToGrenadeMenuItem,
                                                  player->GetGrenadePlaybackStarted()
                                                                  || player->GetGrenadePlaybackStarting()
                                                                  || player->GetGrenadeAwaitingDetonation()
                                                          ? ITEMDRAW_DISABLED
                                                          : ITEMDRAW_DEFAULT));
}

void AppendGodModeMenuItem(
        IBaseMenu *menu,
        int clientIndex,
        Player *player)
{
    char godModeMenuItem[512];

    g_JabronEZ.GetTranslations()->FormatTranslated(
            godModeMenuItem,
            sizeof(godModeMenuItem),
            "%T",
            3,
            nullptr,
            "Grenades menu god mode",
            &clientIndex,
            g_JabronEZ.GetTranslations()->GetGodModeTranslationPhrase(player->GetGodMode()));

    menu->AppendItem("god", ItemDrawInfo(
                                    godModeMenuItem,
                                    ITEMDRAW_DEFAULT));
}

void AppendNoClipMenuItem(
        IBaseMenu *menu,
        int clientIndex,
        Player *player,
        const char *onPhrase,
        const char *offPhrase)
{
    char noClipMenuItem[512];

    g_JabronEZ.GetTranslations()->FormatTranslated(
            noClipMenuItem,
            sizeof(noClipMenuItem),
            "%T",
            3,
            nullptr,
            "Grenades menu noclip",
            &clientIndex,
            player->GetNoClip() ? onPhrase : offPhrase);

    menu->AppendItem("noclip", ItemDrawInfo(
                                       noClipMenuItem,
                                       player->IsAlive() ? ITEMDRAW_DEFAULT : ITEMDRAW_DISABLED));
}

void MenuManager::OpenMenu(Player *player, size_t pageNumber)
{
    IMenuStyle *style = _smMenuManager->GetDefaultStyle();
    IBaseMenu *menu = style->CreateMenu(this, _extension->GetIdentity());
    _menus.push_back(menu);

    player->SetGrenadeMenu(menu);
    player->SetGrenadeMenuPage(pageNumber);

    int clientIndex = player->GetClientIndex();
    int spotCount = (int) player->GetGrenadeSpots().size();

    const char *grenadesMenuOn = "Grenades menu on";
    const char *grenadesMenuOff = "Grenades menu off";

    SetMenuTitle(menu, clientIndex, spotCount);

    AppendAddSpotMenuItem(menu, clientIndex, player);
    AppendRemoveSpotMenuItem(menu, clientIndex, player, spotCount);
    AppendPlaybackMenuItem(menu, clientIndex, player, spotCount, grenadesMenuOn, grenadesMenuOff);
    AppendFastForwardMenuItem(menu, clientIndex, player);
    AppendSwitchToGrenadeMenuItem(menu, clientIndex, player);
    AppendGodModeMenuItem(menu, clientIndex, player);
    AppendNoClipMenuItem(menu, clientIndex, player, grenadesMenuOn, grenadesMenuOff);

    menu->SetMenuOptionFlags(menu->GetMenuOptionFlags() | MENUFLAG_BUTTON_EXIT);

    if (pageNumber == 1)
        menu->Display(player->GetClientIndex(), MENU_TIME_FOREVER);
    else
        menu->DisplayAtItem(player->GetClientIndex(), MENU_TIME_FOREVER, 6 * (pageNumber - 1));
}

void MenuManager::NotifyPlayerMenuDestroyed(IBaseMenu *menu)
{
    // TODO: This is kind of not very good, to be honest. I think we should instead store the user's ID in the menu instance somehow.
    auto playerManager = g_JabronEZ.GetPlayerManager();

    if (playerManager == nullptr)
        return;

    auto players = playerManager->GetPlayers();

    for (auto player : players)
    {
        if (player->GetGrenadeMenu() == menu)
        {
            player->SetGrenadeMenu(nullptr);
        }
    }
}
