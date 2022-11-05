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

#ifndef JABRONEZ_MENU_MANAGER_H
#define JABRONEZ_MENU_MANAGER_H

#include "smsdk_ext.h"
#include <sh_vector.h>
#include "weapon_identifiers.h"

class Player;

class MenuManager : public IMenuHandler {
public:
    MenuManager(IMenuManager *smMenuManager, IExtension *extension);
    ~MenuManager();

    void OpenMenu(Player *player, size_t pageNumber = 1);
    void DestroyMenuSafely(IBaseMenu *menu);

    virtual void OnMenuSelect2(IBaseMenu *menu,
                               int client,
                               unsigned int item,
                               unsigned int item_on_page) override;

    virtual void OnMenuDestroy(SourceMod::IBaseMenu *menu) override;

    virtual void OnMenuEnd(IBaseMenu *menu, MenuEndReason reason) override;

private:
    IMenuManager *_smMenuManager { nullptr };
    IExtension *_extension { nullptr };
    SourceHook::CVector<IBaseMenu*> _menus;
};

#endif
