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

#include "menus.h"
#include "extension.h"
#include "player.h"
#include "translations.h"

const char *GetGrenadeModeMenuItemKey(CSWeaponID grenadeType);
Menus::Menus()
{
    _menus.clear();
}

Menus::~Menus()
{
    for (auto menu: _menus)
    {
        DestroyMenuSafely(menu);
    }

    _menus.clear();
}

void Menus::OnMenuSelect2(
        IBaseMenu *menu,
        int client,
        unsigned int item,
        unsigned int item_on_page)
{
    META_CONPRINTF("Menu selection! %d %d\n", item, client);
}

void Menus::OnMenuEnd(SourceMod::IBaseMenu *menu, SourceMod::MenuEndReason reason)
{
    DestroyMenuSafely(menu);
}

void Menus::DestroyMenuSafely(SourceMod::IBaseMenu *menu)
{
    if (menu->GetHandle() == BAD_HANDLE)
    {
        return;
    }

    HandleSecurity sec;

    sec.pIdentity = nullptr;
    sec.pOwner = myself->GetIdentity();

    handlesys->FreeHandle(menu->GetHandle(), &sec);
}

void Menus::OnMenuDestroy(SourceMod::IBaseMenu *menu)
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

void AppendGrenadeTypeMenuItem(
        IBaseMenu *menu,
        int clientIndex,
        Player *player)
{
    char grenadeTypeMenuItem[512];

    g_JabronEZ.GetTranslations()->FormatTranslated(
            grenadeTypeMenuItem,
            sizeof(grenadeTypeMenuItem),
            "%T",
            3,
            nullptr,
            "Grenades menu type",
            &clientIndex,
            g_JabronEZ.GetTranslations()->GetGrenadeTranslationPhrase(player->GetGrenadeType()));

    menu->AppendItem("grenade_type", ItemDrawInfo(
                                             grenadeTypeMenuItem,
                                             player->GetGrenadePlaybackStarted()
                                                             || player->GetGrenadePlaybackStarting()
                                                             || player->GetGrenadeAwaitingDetonation()
                                                     ? ITEMDRAW_DISABLED
                                                     : ITEMDRAW_DEFAULT));
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

    // TODO: This should be disabled if the player is dead.
    menu->AppendItem("noclip", ItemDrawInfo(
                                       noClipMenuItem,
                                       ITEMDRAW_DEFAULT));
}

const char *GetGrenadeModeMenuItemKey(CSWeaponID grenadeType)
{
    switch (grenadeType)
    {
        case CSWeapon_INCGRENADE:
            return "incendiary_mode";

        case CSWeapon_DECOY:
            return "decoy_mode";

        case CSWeapon_MOLOTOV:
            return "molotov_mode";

        case CSWeapon_HEGRENADE:
            return "hegrenade_mode";

        case CSWeapon_SMOKEGRENADE:
            return "smoke_mode";

        case CSWeapon_FLASHBANG:
            return "flash_mode";
    }

    return "";
}

void AppendGrenadeModeMenuItem(
        IBaseMenu *menu,
        CSWeaponID grenadeType,
        int clientIndex,
        Player *player)
{
    char grenadeModeMenuItem[512];

    g_JabronEZ.GetTranslations()->FormatTranslated(
            grenadeModeMenuItem,
            sizeof(grenadeModeMenuItem),
            "%T",
            3,
            nullptr,
            g_JabronEZ.GetTranslations()->GetGrenadeModeMenuItemTranslationPhrase(grenadeType),
            &clientIndex,
            g_JabronEZ.GetTranslations()->GetProjectileModeTranslationPhrase(player->GetProjectileMode(grenadeType)));

    menu->AppendItem(GetGrenadeModeMenuItemKey(grenadeType), ItemDrawInfo(
                                                                     grenadeModeMenuItem,
                                                                     ITEMDRAW_DEFAULT));
}

void AppendPlayerModeMenuItem(
        IBaseMenu *menu,
        int clientIndex,
        Player *player)
{
    char playerModeMenuItem[512];

    g_JabronEZ.GetTranslations()->FormatTranslated(
            playerModeMenuItem,
            sizeof(playerModeMenuItem),
            "%T",
            3,
            nullptr,
            "Grenades menu player mode",
            &clientIndex,
            g_JabronEZ.GetTranslations()->GetPlayerModeTranslationPhrase(player->GetPlayerMode()));

    menu->AppendItem("player_mode", ItemDrawInfo(
                                            playerModeMenuItem,
                                            ITEMDRAW_DEFAULT));
}

void Menus::OpenMenu(Player *player)
{
    // TODO: We need to support starting their menu on a specific page, so that way we can support persistent menu mode.
    IMenuStyle *style = menus->GetDefaultStyle();
    IBaseMenu *menu = style->CreateMenu(this, myself->GetIdentity());
    _menus.push_back(menu);

    player->SetGrenadeMenuOpen(true);

    int clientIndex = player->GetClientIndex();
    int spotCount = (int) player->GetGrenadeSpots().size();

    const char *grenadesMenuOn = "Grenades menu on";
    const char *grenadesMenuOff = "Grenades menu off";

    SetMenuTitle(menu, clientIndex, spotCount);

    AppendAddSpotMenuItem(menu, clientIndex, player);
    AppendRemoveSpotMenuItem(menu, clientIndex, player, spotCount);
    AppendPlaybackMenuItem(menu, clientIndex, player, spotCount, grenadesMenuOn, grenadesMenuOff);
    AppendFastForwardMenuItem(menu, clientIndex, player);
    AppendGrenadeTypeMenuItem(menu, clientIndex, player);
    AppendSwitchToGrenadeMenuItem(menu, clientIndex, player);
    AppendGodModeMenuItem(menu, clientIndex, player);
    AppendNoClipMenuItem(menu, clientIndex, player, grenadesMenuOn, grenadesMenuOff);
    AppendGrenadeModeMenuItem(menu, CSWeapon_FLASHBANG, clientIndex, player);
    AppendGrenadeModeMenuItem(menu, CSWeapon_SMOKEGRENADE, clientIndex, player);
    AppendGrenadeModeMenuItem(menu, CSWeapon_HEGRENADE, clientIndex, player);
    AppendGrenadeModeMenuItem(menu, CSWeapon_DECOY, clientIndex, player);
    AppendGrenadeModeMenuItem(menu, CSWeapon_MOLOTOV, clientIndex, player);
    AppendGrenadeModeMenuItem(menu, CSWeapon_INCGRENADE, clientIndex, player);

    menu->SetMenuOptionFlags(menu->GetMenuOptionFlags() | MENUFLAG_BUTTON_EXIT);
    menu->Display(player->GetClientIndex(), MENU_TIME_FOREVER);
}