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

#include "particle_manager.h"
#include "smsdk_ext.h"
#include "extension.h"
#include "temporary_entities.h"
#include "temporary_entity_creator.h"
#include "recipient_filter.h"
#include "player_manager.h"

ParticleManager::ParticleManager(IVEngineServer *engineServer)
{
    _engineServer = engineServer;
}

ParticleManager::~ParticleManager()
{
}

void ParticleManager::LoadCustomParticles()
{
    if (!_engineServer->PrecacheGeneric("particles/jabronez_particles_0001.pcf", true))
    {
        META_CONPRINTF("Oh no, we were unable to load our custom particles.\n");
        return;
    }

    auto downloadables = _netStringTables->FindTable("downloadables");

    if (downloadables == nullptr)
    {
        META_CONPRINTF("Oh no, we could not find the downloadables string table.\n");
        return;
    }

    // Add our download so that clients will get the particles when they join
    auto previousLock = _engineServer->LockNetworkStringTables(false);
    downloadables->AddString(true, "particles/jabronez_particles_0001.pcf", -1, "");
    _engineServer->LockNetworkStringTables(previousLock);

    auto particleEffectNames = _netStringTables->FindTable("ParticleEffectNames");

    if (particleEffectNames == nullptr)
    {
        META_CONPRINTF("Unable to find the string table for ParticleEffectNames\n");
        return;
    }

    for (int stringIndex = 0; stringIndex < particleEffectNames->GetNumStrings(); stringIndex++)
    {
        META_CONPRINTF("str[%d] = %s\n", stringIndex, particleEffectNames->GetString(stringIndex));
    }

    auto effectDispatchTable = _netStringTables->FindTable("EffectDispatch");

    if (effectDispatchTable == nullptr)
    {
        META_CONPRINTF("Unable to find EffectDispatch table.\n");
        return;
    }

    _shortSmokeParticleType = particleEffectNames->FindStringIndex("jez_explosion_smokegrenade");
    _shortSmokeScreenParticleType = particleEffectNames->FindStringIndex("jez_explosion_screen_smokegrenade_new");
    _particleEffect = effectDispatchTable->FindStringIndex("ParticleEffect");
}

bool ParticleManager::Init(ISmmAPI *ismm, char *error, size_t maxlen)
{
    GET_V_IFACE_ANY(GetEngineFactory, _netStringTables, INetworkStringTableContainer, INTERFACENAME_NETWORKSTRINGTABLESERVER);

    return true;
}

void ParticleManager::CreateShortSmoke(Vector origin, QAngle angle) const
{
    auto effectDispatch = g_JabronEZ.GetTemporaryEntities()->FindByName("EffectDispatch");

    effectDispatch->SetPropertyInt("m_iEffectName", _particleEffect);
    effectDispatch->SetPropertyInt("m_nHitBox", _shortSmokeParticleType);
    effectDispatch->SetPropertyFloat("m_vOrigin.x", origin.x);
    effectDispatch->SetPropertyFloat("m_vOrigin.y", origin.y);
    effectDispatch->SetPropertyFloat("m_vOrigin.z", origin.z);
    effectDispatch->SetPropertyFloat("m_vStart.x", origin.x);
    effectDispatch->SetPropertyFloat("m_vStart.y", origin.y);
    effectDispatch->SetPropertyFloat("m_vStart.z", origin.z);
    effectDispatch->SetPropertyFloat("m_vAngles.x", angle.x);
    effectDispatch->SetPropertyFloat("m_vAngles.y", angle.y);
    effectDispatch->SetPropertyFloat("m_vAngles.z", angle.z);

    RecipientFilter filter(g_JabronEZ.GetPlayerManager()->GetPlayers(), false, false);
    effectDispatch->Send(filter, 0.0f);

    // FIXME: We need to show the screen smoke effect as well, or you will be able to see right throw the smoke.
}
