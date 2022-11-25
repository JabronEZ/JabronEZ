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

#include "csmokegrenade_projectile_detonate_hook.h"
#include "extension.h"
#include "entity_utilities.h"
#include "player_manager.h"
#include "player.h"
#include <CDetour/detours.h>

JEZ_HOOK_MEMBER_DEF0_VOID(
        CSmokeGrenadeProjectileDetonate,
        CBaseEntity)
{
    auto projectileEntity = reinterpret_cast<CBaseEntity*>(this);
    Hook_Call_CSmokeGrenadeProjectileDetonate(projectileEntity);

    auto throwerEntity = g_JabronEZ.GetEntityUtilities()->GetProjectileThrower(projectileEntity);

    if (throwerEntity != nullptr)
    {
        auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByBaseEntity(throwerEntity);

        if (player != nullptr && player->IsValid() && player->GetShortGrenades())
        {
            static unsigned int smokeEffectTickBeginOffset = 0;

            if (smokeEffectTickBeginOffset == 0)
            {
                sm_sendprop_info_t sendpropInfo {};
                gamehelpers->FindSendPropInfo("CSmokeGrenadeProjectile", "m_nSmokeEffectTickBegin", &sendpropInfo);

                if (sendpropInfo.prop == nullptr)
                    return;

                smokeEffectTickBeginOffset = sendpropInfo.actual_offset;
            }

            static unsigned int didSmokeEffectOffset = 0;

            if (didSmokeEffectOffset == 0)
            {
                sm_sendprop_info_t sendpropInfo {};
                gamehelpers->FindSendPropInfo("CSmokeGrenadeProjectile", "m_bDidSmokeEffect", &sendpropInfo);

                if (sendpropInfo.prop == nullptr)
                    return;

                didSmokeEffectOffset = sendpropInfo.actual_offset;
            }

            // Setting these values right after the detonate function will cause the original smoke particle to never appear.
            *(bool*)((uint8_t*)projectileEntity + didSmokeEffectOffset) = false;
            *(uint32_t*)((uint8_t*)projectileEntity + smokeEffectTickBeginOffset) = 0;
        }
    }
}

bool Hooks_Init_CSmokeGrenadeProjectileDetonateHook(
        char *error,
        size_t maxlength)
{
    JEZ_HOOK_MEMBER_CREATE(CSmokeGrenadeProjectileDetonate, "CSmokeGrenadeProjectileDetonate");
    return true;
}

void Hooks_Cleanup_CSmokeGrenadeProjectileDetonateHook()
{
    JEZ_HOOK_CLEANUP(CSmokeGrenadeProjectileDetonate);
}