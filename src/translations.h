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

#ifndef JABRONEZ_TRANSLATIONS_H
#define JABRONEZ_TRANSLATIONS_H

#include "god_mode.h"
#include "player_mode.h"
#include "projectile_mode.h"
#include "smsdk_ext.h"
#include "weapon_identifiers.h"

class Translations {
public:
    Translations(ITranslator *smTranslator);
    ~Translations();

    bool Init(char *error, size_t maxlength);

    bool FormatTranslated(
            char *buffer,
            size_t maxlength,
            const char *format,
            unsigned int num_params,
            size_t *out_length,
            ...);

    const char *GetGrenadeTranslationPhrase(GrenadeType grenadeType);
    const char *GetGodModeTranslationPhrase(GodMode godMode);
    const char *GetProjectileModeTranslationPhrase(ProjectileMode projectileMode);
    const char *GetGrenadeModeMenuItemTranslationPhrase(GrenadeType grenadeType);
    const char *GetPlayerModeTranslationPhrase(PlayerMode mode);

private:
    ITranslator *_smTranslator { nullptr };
    IPhraseCollection *_phraseCollection { nullptr };
    IPhraseFile *_phraseFile { nullptr };
};

#endif
