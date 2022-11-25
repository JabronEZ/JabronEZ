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

#ifndef JABRONEZ_PLAYER_MANAGER_H
#define JABRONEZ_PLAYER_MANAGER_H

#include "smsdk_ext.h"
#include <sh_vector.h>

class Player;

class PlayerManager : IClientListener
{
public:
    explicit PlayerManager(IPlayerManager *smPlayerManager, IGameHelpers *gameHelpers, ITimerSystem *timerSystem);
    ~PlayerManager();

    void OnClientConnected(int clientIndex) override;
    void OnClientDisconnecting(int clientIndex) override;

    Player *GetPlayerByClientIndex(int clientIndex) const;
    Player *GetPlayerByEdict(edict_t *edict) const;
    Player *GetPlayerByUserId(int userId) const;
    Player *GetPlayerByBaseEntity(CBaseEntity *entity) const;

    const SourceHook::CVector<Player *> &GetPlayers() const;
    SourceHook::CVector<Player *> GetValidPlayers() const;

private:
    void CleanupPlayer(Player *player);

private:
    IGameHelpers *_gameHelpers { nullptr };
    ITimerSystem *_timerSystem { nullptr };
    IPlayerManager *_smPlayerManager { nullptr };
    SourceHook::CVector<Player*> _players;
};

#endif
