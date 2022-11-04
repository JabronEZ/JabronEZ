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

#ifndef JABRONEZ_PLAYER_H
#define JABRONEZ_PLAYER_H

#include "smsdk_ext.h"
#include <sh_vector.h>
#include "spot.h"
#include "god_mode.h"
#include "projectile_mode.h"
#include "player_mode.h"
#include "weapon_identifiers.h"

class Player {
public:
    Player(int clientIndex, int userId, IGamePlayer *gamePlayer);
    ~Player();

    int GetClientIndex() const { return _clientIndex; }
    int GetUserId() const { return _userId; }
    IGamePlayer *GetGamePlayer() const { return _gamePlayer; }
    const SourceHook::CVector<Spot> &GetGrenadeSpots() const { return _grenadeSpots; }

    GodMode GetGodMode() const { return _godMode; }
    void SetGodMode(GodMode godMode) { _godMode = godMode; }

    bool GetNoClip() const { return _noClip; }
    void SetNoClip(bool noClip) { _noClip = noClip; }

    PlayerMode GetPlayerMode() const { return _playerMode; }
    void SetPlayerMode(PlayerMode playerMode) { _playerMode = playerMode; }

    bool GetGrenadePlaybackEnabled() const { return _grenadePlaybackEnabled; }
    void SetGrenadePlaybackEnabled(bool grenadePlaybackEnabled) { _grenadePlaybackEnabled = grenadePlaybackEnabled; }

    bool GetGrenadePlaybackStarted() const { return _grenadePlaybackStarted; }
    void SetGrenadePlaybackStarted(bool grenadePlaybackStarted) { _grenadePlaybackStarted = grenadePlaybackStarted; }

    bool GetGrenadePlaybackStarting() const { return _grenadePlaybackStarting; }
    void SetGrenadePlaybackStarting(bool grenadePlaybackStarting) { _grenadePlaybackStarting = grenadePlaybackStarting; }

    bool GetGrenadeAwaitingDetonation() const { return _grenadeAwaitingDetonation; }
    void SetGrenadeAwaitingDetonation(bool grenadeAwaitingDetonation) { _grenadeAwaitingDetonation = grenadeAwaitingDetonation; }

    Vector GetGrenadeProjectileOrigin() const { return _grenadeProjectileOrigin; }
    void SetGrenadeProjectileOrigin(Vector grenadeProjectileOrigin) { _grenadeProjectileOrigin = grenadeProjectileOrigin; }

    QAngle GetGrenadeProjectileAngle() const { return _grenadeProjectileAngle; }
    void SetGrenadeProjectileAngle(QAngle grenadeProjectileAngle) { _grenadeProjectileAngle = grenadeProjectileAngle; }

    Vector GetGrenadeProjectileVelocity() const { return _grenadeProjectileVelocity; }
    void SetGrenadeProjectileVelocity(Vector grenadeProjectileVelocity) { _grenadeProjectileVelocity = grenadeProjectileVelocity; }

    Vector GetGrenadeProjectileAngularImpulse() const { return _grenadeProjectileAngularImpulse; }
    void SetGrenadeProjectileAngularImpulse(Vector grenadeProjectileAngularImpulse) { _grenadeProjectileAngularImpulse = grenadeProjectileAngularImpulse; }

    size_t GetGrenadeCurrentSpotIndex() const { return _grenadeCurrentSpotIndex; }
    void SetGrenadeCurrentSpotIndex(size_t grenadeCurrentSpotIndex) { _grenadeCurrentSpotIndex = grenadeCurrentSpotIndex; }

    ITimer *GetGrenadeSpotTimer() const { return _grenadeSpotTimer; }
    void SetGrenadeSpotTimer(ITimer *grenadeSpotTimer) { _grenadeSpotTimer = grenadeSpotTimer; }

    ITimer *GetGrenadeLaunchTimer() const { return _grenadeLaunchTimer; }
    void SetGrenadeLaunchTimer(ITimer *grenadeLaunchTimer) { _grenadeLaunchTimer = grenadeLaunchTimer; }

    CSWeaponID GetGrenadeType() const { return _grenadeType; }
    void SetGrenadeType(CSWeaponID grenadeType) { _grenadeType = grenadeType; }

    Spot GetGrenadeThrowerSpot() const { return _grenadeThrowerSpot; }
    void SetGrenadeThrowerSpot(const Spot& grenadeThrowerSpot) { _grenadeThrowerSpot = grenadeThrowerSpot; }

    bool GetGrenadeMenuOpen() const { return _grenadeMenuOpen; }
    void SetGrenadeMenuOpen(bool grenadeMenuOpen) { _grenadeMenuOpen = grenadeMenuOpen; }

    CBaseEntity *GetGrenadeTossedGrenade() const { return _grenadeTossedGrenade; }
    void SetGrenadeTossedGrenade(CBaseEntity *grenadeTossedGrenade) { _grenadeTossedGrenade = grenadeTossedGrenade; }

    ITimer *GetGrenadeTriggerPlaybackTimer() const { return _grenadeTriggerPlaybackTimer; }
    void SetGrenadeTriggerPlaybackTimer(ITimer *grenadeTriggerPlaybackTimer) { _grenadeTriggerPlaybackTimer = grenadeTriggerPlaybackTimer; }

    ITimer *GetGrenadeGotoNextSpotOrFinishTimer() const { return _grenadeGotoNextSpotOrFinishTimer; }
    void SetGrenadeGotoNextSpotOrFinishTimer(ITimer *grenadeGotoNextSpotOrFinishTimer) { _grenadeGotoNextSpotOrFinishTimer = grenadeGotoNextSpotOrFinishTimer; }

    ITimer *GetGrenadeHandlePlaybackTimer() const { return _grenadeHandlePlaybackTimer; }
    void SetGrenadeHandlePlaybackTimer(ITimer *grenadeHandlePlaybackTimer) { _grenadeHandlePlaybackTimer = grenadeHandlePlaybackTimer; }

    size_t GetGrenadeMenuPage() const { return _grenadeMenuPage; }
    void SetGrenadeMenuPage(size_t grenadeMenuPage) { _grenadeMenuPage = grenadeMenuPage; }

    ProjectileMode GetGrenadeSmokeMode() const { return _grenadeSmokeMode; }
    void SetGrenadeSmokeMode(ProjectileMode grenadeSmokeMode) { _grenadeSmokeMode = grenadeSmokeMode; }

    ProjectileMode GetGrenadeDecoyMode() const { return _grenadeDecoyMode; }
    void SetGrenadeDecoyMode(ProjectileMode grenadeDecoyMode) { _grenadeDecoyMode = grenadeDecoyMode; }

    ProjectileMode GetGrenadeHEGrenadeMode() const { return _grenadeHEGrenadeMode; }
    void SetGrenadeHEGrenadeMode(ProjectileMode grenadeHEGrenadeMode) { _grenadeHEGrenadeMode = grenadeHEGrenadeMode; }

    ProjectileMode GetGrenadeMolotovMode() const { return _grenadeMolotovMode; }
    void SetGrenadeMolotovMode(ProjectileMode grenadeMolotovMode) { _grenadeMolotovMode = grenadeMolotovMode; }

    ProjectileMode GetGrenadeIncendiaryMode() const { return _grenadeIncendiaryMode; }
    void SetGrenadeIncendiaryMode(ProjectileMode grenadeIncendiaryMode) { _grenadeIncendiaryMode = grenadeIncendiaryMode; }

    ProjectileMode GetProjectileMode(CSWeaponID grenadeType) const;

private:
    int _clientIndex { -1 };
    int _userId { -1 };
    IGamePlayer *_gamePlayer { nullptr };
    SourceHook::CVector<Spot> _grenadeSpots;
    GodMode _godMode { GodMode_OFF };
    bool _noClip { false };
    PlayerMode _playerMode { PlayerMode_ALL };
    bool _grenadePlaybackEnabled { false };
    bool _grenadePlaybackStarted { false };
    bool _grenadePlaybackStarting { false };
    bool _grenadeAwaitingDetonation { false };
    Vector _grenadeProjectileOrigin;
    QAngle _grenadeProjectileAngle;
    Vector _grenadeProjectileVelocity;
    Vector _grenadeProjectileAngularImpulse;
    size_t _grenadeCurrentSpotIndex { 0 };
    ITimer *_grenadeSpotTimer { nullptr };
    ITimer *_grenadeLaunchTimer { nullptr };
    CSWeaponID _grenadeType { CSWeapon_FLASHBANG };
    Spot _grenadeThrowerSpot;
    bool _grenadeMenuOpen { false };
    CBaseEntity *_grenadeTossedGrenade { nullptr };
    ITimer *_grenadeTriggerPlaybackTimer { nullptr };
    ITimer *_grenadeGotoNextSpotOrFinishTimer { nullptr };
    ITimer *_grenadeHandlePlaybackTimer { nullptr };
    size_t _grenadeMenuPage { 0 };
    ProjectileMode _grenadeFlashMode { ProjectileMode_ALL };
    ProjectileMode _grenadeSmokeMode { ProjectileMode_ALL };
    ProjectileMode _grenadeDecoyMode { ProjectileMode_ALL };
    ProjectileMode _grenadeHEGrenadeMode { ProjectileMode_ALL };
    ProjectileMode _grenadeMolotovMode { ProjectileMode_ALL };
    ProjectileMode _grenadeIncendiaryMode { ProjectileMode_ALL };
};

#endif
