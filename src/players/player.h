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
#include "grenade_throw_tickrate.h"
#include "projectile_parameters.h"
#include "weapon_identifiers.h"

class GrenadeTriggerPlaybackTimer;
class HandlePlaybackTimer;
class GrenadeGotoNextSpotOrFinishTimer;
class CUserCmd;
class IMoveHelper;
enum class CheckSlotOccupiedResult;

class Player {
public:
    explicit Player(int clientIndex, int userId, IGamePlayer *gamePlayer, IGameHelpers *gameHelpers, ITimerSystem *timerSystem);
    ~Player();

    int GetClientIndex() const { return _clientIndex; }
    int GetUserId() const { return _userId; }
    IGamePlayer *GetGamePlayer() const { return _gamePlayer; }
    const SourceHook::CVector<Spot> &GetGrenadeSpots() const { return _grenadeSpots; }

    GrenadeThrowTickRate GetGrenadeThrowTickRate() const { return _grenadeThrowTickRate; }
    void SetGrenadeThrowTickRate(GrenadeThrowTickRate grenadeThrowTickRate) { _grenadeThrowTickRate = grenadeThrowTickRate; }

    GodMode GetGodMode() const { return _godMode; }
    void SetGodMode(GodMode godMode) { _godMode = godMode; }

    bool GetShortGrenades() const { return _shortGrenades; }
    void SetShortGrenades(bool shortGrenades) { _shortGrenades = shortGrenades; }

    bool GetGrenadePlaybackEnabled() const { return _grenadePlaybackEnabled; }
    void SetGrenadePlaybackEnabled(bool grenadePlaybackEnabled) { _grenadePlaybackEnabled = grenadePlaybackEnabled; }

    bool GetGrenadePlaybackStarted() const { return _grenadePlaybackStarted; }
    void SetGrenadePlaybackStarted(bool grenadePlaybackStarted) { _grenadePlaybackStarted = grenadePlaybackStarted; }

    bool GetGrenadePlaybackStarting() const { return _grenadePlaybackStarting; }
    void SetGrenadePlaybackStarting(bool grenadePlaybackStarting) { _grenadePlaybackStarting = grenadePlaybackStarting; }

    bool GetGrenadeAwaitingDetonation() const { return _grenadeAwaitingDetonation; }
    void SetGrenadeAwaitingDetonation(bool grenadeAwaitingDetonation) { _grenadeAwaitingDetonation = grenadeAwaitingDetonation; }

    ProjectileParameters GetProjectileParameters() const { return _projectileParameters; }
    void SetProjectileParameters(const ProjectileParameters& projectileParameters) { _projectileParameters = projectileParameters; }

    size_t GetGrenadeCurrentSpotIndex() const { return _grenadeCurrentSpotIndex; }
    void SetGrenadeCurrentSpotIndex(size_t grenadeCurrentSpotIndex) { _grenadeCurrentSpotIndex = grenadeCurrentSpotIndex; }

    GrenadeType GetGrenadeType() const { return _grenadeType; }
    void SetGrenadeType(GrenadeType grenadeType) { _grenadeType = grenadeType; }

    Spot GetGrenadeThrowerSpot() const { return _grenadeThrowerSpot; }
    void SetGrenadeThrowerSpot(const Spot& grenadeThrowerSpot) { _grenadeThrowerSpot = grenadeThrowerSpot; }

    cell_t GetGrenadeTossedGrenade() const { return _grenadeTossedGrenade; }
    void SetGrenadeTossedGrenade(cell_t grenadeTossedGrenade) { _grenadeTossedGrenade = grenadeTossedGrenade; }

    GrenadeTriggerPlaybackTimer *GetGrenadeTriggerPlaybackTimer() const { return _grenadeTriggerPlaybackTimer; }
    void SetGrenadeTriggerPlaybackTimer(GrenadeTriggerPlaybackTimer *grenadeTriggerPlaybackTimer) { _grenadeTriggerPlaybackTimer = grenadeTriggerPlaybackTimer; }

    GrenadeGotoNextSpotOrFinishTimer *GetGrenadeGotoNextSpotOrFinishTimer() const { return _grenadeGotoNextSpotOrFinishTimer; }
    void SetGrenadeGotoNextSpotOrFinishTimer(GrenadeGotoNextSpotOrFinishTimer *grenadeGotoNextSpotOrFinishTimer) { _grenadeGotoNextSpotOrFinishTimer = grenadeGotoNextSpotOrFinishTimer; }

    HandlePlaybackTimer *GetGrenadeHandlePlaybackTimer() const { return _grenadeHandlePlaybackTimer; }
    void SetGrenadeHandlePlaybackTimer(HandlePlaybackTimer *grenadeHandlePlaybackTimer) { _grenadeHandlePlaybackTimer = grenadeHandlePlaybackTimer; }

    bool IsGrenadeMenuOpen() const { return _grenadeMenu != nullptr; }

    void SetGrenadeMenu(IBaseMenu *menu) { _grenadeMenu = menu; }
    IBaseMenu *GetGrenadeMenu() const { return _grenadeMenu; }

    size_t GetGrenadeMenuPage() const { return _grenadeMenuPage; }
    void SetGrenadeMenuPage(size_t grenadeMenuPage) { _grenadeMenuPage = grenadeMenuPage; }

    CBaseEntity *GiveNamedItem(const char *entityName) const;

    CBaseEntity *FindWeapon(const char *entityName) const;
    void RemoveWeapon(CBaseEntity *weaponEntity) const;

    bool GetNoClip() const;
    void SetNoClip(bool noClip);

    Vector GetAbsOrigin() const;
    QAngle GetEyeAngles() const;
    CBaseEntity *GetActiveWeapon() const;
    CBaseEntity *GetEntity() const;
    SourceHook::CVector<CBaseEntity*> GetAllWeapons() const;

    void OnProjectileCreate(const Vector &origin, const QAngle &angle, const Vector &velocity, const Vector &angularImpulse, GrenadeType grenadeType);
    void OnProjectileCreated(const Vector &origin, const QAngle &angle, const Vector &velocity, const Vector &angularImpulse, GrenadeType grenadeType, CBaseEntity *projectileEntity);
    void OnGrenadeDetonationEvent(GrenadeType grenadeType, cell_t projectileReference);
    bool OnRunCmd(CUserCmd *command, IMoveHelper *moveHelper);

    bool IsValid() const;
    bool IsAlive() const;
    void RespawnPlayer() const;
    Spot GetClosestGrenadeSpot(size_t *outSpotIndex) const;
    bool IsOnActualTeam() const;
    void GiveAllGrenades() const;

    void StartGrenadeTesterPlayback();
    void FinishGrenadeTesterPlayback(bool restorePosition = true);
    void HandleGrenadeTesterPlayback();
    void RefreshGrenadesMenu();

    void DoAddSpot();
    void DoRemoveSpot();
    void DoTogglePlayback();
    void DoFastForward();
    void DoToggleShortGrenades();
    void DoToggleGodMode();
    void DoToggleNoClip();
    void DoToggleGrenadeThrowTickRate();
    void GotoNextSpotOrFinishPlayback();

    int OnCanAcquire(void *econView, int type, int originalResult);
    void OnBumpWeapon(CBaseEntity *weaponEntity);
    void OnBumpWeaponPost(CBaseEntity *weaponEntity);
    CheckSlotOccupiedResult OnCheckSlotOccupied(CBaseEntity *weaponEntity) const;
    bool OnDropWeapon(CBaseEntity *weaponEntity) const;

private:
    unsigned int GetMoveTypeOffset() const;
    bool HandleSwitchGrenade(
            CUserCmd *command,
            const char *weaponClassName,
            const char *activeWeaponClassName,
            const SourceHook::CVector<CBaseEntity*>& allWeapons);
    bool HandleSwitchPrimaryWeapon(
            CUserCmd *command,
            const char *weaponClassName,
            const char *activeWeaponClassName,
            const SourceHook::CVector<CBaseEntity*>& allWeapons);
    bool HandleSwitchSecondaryWeapon(
            CUserCmd *command,
            const char *weaponClassName,
            const char *activeWeaponClassName,
            const SourceHook::CVector<CBaseEntity*>& allWeapons);

    IGameHelpers *_gameHelpers { nullptr };
    ITimerSystem *_timerSystem { nullptr };
    int _clientIndex { -1 };
    int _userId { -1 };
    IGamePlayer *_gamePlayer { nullptr };
    SourceHook::CVector<Spot> _grenadeSpots;
    GodMode _godMode { GodMode_OFF };
    GrenadeThrowTickRate _grenadeThrowTickRate { GrenadeThrowTickRate_128 };
    bool _grenadePlaybackEnabled { false };
    bool _grenadePlaybackStarted { false };
    bool _grenadePlaybackStarting { false };
    bool _grenadeAwaitingDetonation { false };
    bool _shortGrenades { true };
    ProjectileParameters _projectileParameters { Vector(0.0f, 0.0f, 0.0f), QAngle(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f) };
    size_t _grenadeCurrentSpotIndex { 0 };
    GrenadeType _grenadeType { GrenadeType_FLASH };
    Spot _grenadeThrowerSpot;
    cell_t _grenadeTossedGrenade { 0 };
    GrenadeTriggerPlaybackTimer *_grenadeTriggerPlaybackTimer { nullptr };
    GrenadeGotoNextSpotOrFinishTimer *_grenadeGotoNextSpotOrFinishTimer { nullptr };
    HandlePlaybackTimer *_grenadeHandlePlaybackTimer { nullptr };
    IBaseMenu *_grenadeMenu { nullptr };
    size_t _grenadeMenuPage { 0 };
    bool _isBumpingWeapon { false };
};

#endif
