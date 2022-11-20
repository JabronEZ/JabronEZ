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

#ifndef JABRONEZ_PARTICLE_MANAGER_H
#define JABRONEZ_PARTICLE_MANAGER_H

#include "smsdk_ext.h"
#include <sh_vector.h>
#include <networkstringtabledefs.h>

class Player;

class ParticleManager {
public:
    explicit ParticleManager(IVEngineServer *engineServer);
    ~ParticleManager();

    bool Init(ISmmAPI *ismm, char *error, size_t maxlen);
    void LoadCustomParticles();

    void CreateShortSmoke(Vector origin, QAngle angle) const;


private:
    INetworkStringTableContainer *_netStringTables { nullptr };
    IVEngineServer *_engineServer { nullptr };
    int _shortSmokeParticleType;
    int _shortSmokeScreenParticleType;
    int _particleEffect;
};

#endif
