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

#include "cmolotov_projectile_detonate_hook.h"
#include "weapon_identifiers.h"
#include "extension.h"
#include "entity_utilities.h"
#include "player_manager.h"
#include "player.h"

SH_DECL_MANUALHOOK0_void(CMolotovProjectileDetonate, 0, 0, 0);
int g_MolotovProjectileDetonateHookId = 0;

void Hook_Callback_CMolotovProjectileDetonate()
{
    auto projectileEntity = META_IFACEPTR(CBaseEntity);
    auto isIncendiary = g_JabronEZ.GetEntityUtilities()->IsIncendiaryGrenade(projectileEntity);
    auto throwerEntity = g_JabronEZ.GetEntityUtilities()->GetProjectileThrower(projectileEntity);

    if (throwerEntity != nullptr)
    {
        auto player = g_JabronEZ.GetPlayerManager()->GetPlayerByBaseEntity(throwerEntity);

        if (player != nullptr && player->IsValid())
            player->OnGrenadeDetonationEvent(isIncendiary ? GrenadeType_INCENDIARY : GrenadeType_MOLOTOV, gamehelpers->EntityToReference(projectileEntity));
    }

    RETURN_META(MRES_IGNORED);
}

void Hooks_MaybeSetupCMolotovProjectileDetonateHook(CBaseEntity *molotovProjectile)
{
    if (molotovProjectile == nullptr)
        return;

    if (g_MolotovProjectileDetonateHookId == 0)
    {
        void *molotovVtable = *(void**)molotovProjectile;
        g_MolotovProjectileDetonateHookId = SH_ADD_MANUALDVPHOOK(CMolotovProjectileDetonate, molotovVtable, SH_STATIC(Hook_Callback_CMolotovProjectileDetonate), false);
    }
}

bool Hooks_Init_CMolotovProjectileDetonateHook(
        IGameConfig *gameConfig,
        char *error,
        size_t maxlength)
{
    int molotovProjectileDetonateOffset;
    if (!gameConfig->GetOffset("CMolotovProjectileDetonate", &molotovProjectileDetonateOffset))
    {
        snprintf(error, maxlength, "Unable to find offset for %s\n", "CMolotovProjectileDetonate");
        return false;
    }

    SH_MANUALHOOK_RECONFIGURE(CMolotovProjectileDetonate, molotovProjectileDetonateOffset, 0, 0);

    return true;
}

void Hooks_Cleanup_CMolotovProjectileDetonateHook()
{
    if (g_MolotovProjectileDetonateHookId != 0)
    {
        SH_REMOVE_HOOK_ID(g_MolotovProjectileDetonateHookId);
        g_MolotovProjectileDetonateHookId = 0;
    }
}