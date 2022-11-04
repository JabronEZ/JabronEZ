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

#ifndef JABRONEZ_MENUS_H
#define JABRONEZ_MENUS_H

#include "smsdk_ext.h"
#include <sh_vector.h>
#include "weapon_identifiers.h"

class Player;

class Menus : public IMenuHandler {
public:
    Menus();
    ~Menus();

    void OpenMenu(Player *player);
    void DestroyMenuSafely(IBaseMenu *menu);

    virtual void OnMenuSelect2(IBaseMenu *menu,
                               int client,
                               unsigned int item,
                               unsigned int item_on_page) override;

    virtual void OnMenuDestroy(SourceMod::IBaseMenu *menu) override;

    virtual void OnMenuEnd(IBaseMenu *menu, MenuEndReason reason) override;

private:
    SourceHook::CVector<IBaseMenu*> _menus;
};

#endif
