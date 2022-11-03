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

Menus::Menus()
{

}

Menus::~Menus()
{
    // TODO: If we are unloaded, we should first destroy any menus that are inflight since we are their listener and we don't want to explode and crash.
}

void Menus::OnMenuSelect2(
    IBaseMenu *menu,
    int client,
    unsigned int item,
    unsigned int item_on_page)
{
    META_CONPRINTF("Hey, some nerd selected an item: %d %d\n", client, item);
}

void Menus::OpenMenu(Player *player)
{
    IMenuStyle *style = menus->GetDefaultStyle();
    IBaseMenu *menu = style->CreateMenu(this, nullptr);
    menu->SetDefaultTitle("Hey nerd");

    ItemDrawInfo dr;
    dr.display = "This is the text probably";
    dr.style = ITEMDRAW_DEFAULT;

    menu->AppendItem("our_key", dr);

    menu->Display(player->GetClientIndex(), MENU_TIME_FOREVER);
}