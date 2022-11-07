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
#include "handle_playback_timer.h"
#include "player.h"
#include "hooks.h"
#include "entity_utilities.h"

HandlePlaybackTimer::HandlePlaybackTimer(Player *player, ITimerSystem *timerSystem, IGameHelpers *gameHelpers)
    : PlayerTimer(player, 1.5f, timerSystem)
{
    _gameHelpers = gameHelpers;
}

HandlePlaybackTimer::~HandlePlaybackTimer()
{
    auto player = GetPlayer();

    if (player != nullptr)
        player->SetGrenadeHandlePlaybackTimer(nullptr);
}

CBaseEntity *ThrowGrenade(GrenadeType grenadeType, Player *player, Vector origin, QAngle angle, Vector velocity, Vector angularImpulse)
{
    auto playerEntity = g_JabronEZ.GetEntityUtilities()->GetEntityByIndex(player->GetClientIndex(), true);
    auto grenadeItemDefinition = GetItemDefinitionIndexFromGrenadeType(grenadeType);

    if(grenadeType == GrenadeType_FLASH)
        return Hook_Call_FlashbangProjectileCreate(origin, angle, velocity, angularImpulse, playerEntity, grenadeItemDefinition);
    else if(grenadeType == GrenadeType_SMOKE)
        return Hook_Call_SmokeProjectileCreate(origin, angle, velocity, angularImpulse, playerEntity, grenadeItemDefinition);
    else if(grenadeType == GrenadeType_DECOY)
        return Hook_Call_DecoyProjectileCreate(origin, angle, velocity, angularImpulse, playerEntity, grenadeItemDefinition);
    else if(grenadeType == GrenadeType_HEGRENADE)
        return Hook_Call_HEGrenadeProjectileCreate(origin, angle, velocity, angularImpulse, playerEntity, grenadeItemDefinition);
    else if(grenadeType == GrenadeType_INCENDIARY || grenadeType == GrenadeType_MOLOTOV)
        return Hook_Call_MolotovProjectileCreate(origin, angle, velocity, angularImpulse, playerEntity, grenadeItemDefinition);

    return nullptr;
}

void HandlePlaybackTimer::OnPlayerTimer()
{
    auto player = GetPlayer();

    if (player == nullptr)
        return;

    // TODO: In the old plugin, we would actually clear the player's HandlePlaybackTimer here.
    //       I don't think it will make a difference, so if we can avoid it that would be more maintainable for sure!

    if (!player->IsOnActualTeam())
        return;

    auto grenadeType = player->GetGrenadeType();

    // TODO: We should probably make a structure out of these.
    auto projectileOrigin = player->GetGrenadeProjectileOrigin();
    auto projectileAngle = player->GetGrenadeProjectileAngle();
    auto projectileVelocity = player->GetGrenadeProjectileVelocity();
    auto projectileAngularImpulse = player->GetGrenadeProjectileAngularImpulse();

    auto grenadeEntity = ThrowGrenade(grenadeType, player, projectileOrigin, projectileAngle, projectileVelocity, projectileAngularImpulse);

    auto projectileReference = _gameHelpers->EntityToReference(grenadeEntity);

    if (projectileReference == 0)
        return;

    player->SetGrenadeTossedGrenade(projectileReference);

    // TODO: Support short smokes/decoys
//    if(s_GrenadeType == CSWeapon_SMOKEGRENADE)
//        MarkShortSmoke(s_GrenadeIndex);
//
//    if(s_GrenadeType == CSWeapon_DECOY)
//        MarkShortDecoy(s_GrenadeIndex);
}

void HandlePlaybackTimer::OnPlayerTimerEnd()
{
}