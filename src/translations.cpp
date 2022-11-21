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

#include "translations.h"

Translations::Translations(ITranslator *smTranslator)
{
    _smTranslator = smTranslator;
    _phraseCollection = nullptr;
    _phraseFile = nullptr;
}

Translations::~Translations()
{
    // TODO: Do we need to clean up _phraseFile?

    if (_phraseCollection != nullptr)
    {
        _phraseCollection->Destroy();
        _phraseCollection = nullptr;
    }
}

bool Translations::Init(char *error, size_t maxlength)
{
    _phraseCollection = _smTranslator->CreatePhraseCollection();
    _phraseFile = _phraseCollection->AddPhraseFile("jabronez.phrases");

    if (_phraseFile == nullptr)
    {
        snprintf(error, maxlength, "Unable to load JabronEZ translation phrases file");
        return false;
    }

    return true;
}

bool Translations::FormatTranslated(
    char *buffer,
    size_t maxlength,
    const char *format,
    unsigned int num_params,
    size_t *out_length,
    ...)
{
    if (_phraseCollection == nullptr || _phraseFile == nullptr)
    {
        return false;
    }

    va_list argument_list;
    unsigned int param_index;
    const char *fail_phrase;
    void *params[MAX_TRANSLATE_PARAMS];

    if (num_params > MAX_TRANSLATE_PARAMS)
    {
        return false;
    }

    va_start(argument_list, out_length);
    for (param_index = 0; param_index < num_params; param_index++)
    {
        params[param_index] = va_arg(argument_list, void *);
    }
    va_end(argument_list);

    if (!_phraseCollection->FormatString(buffer,
                                      maxlength,
                                      format,
                                      params,
                                      num_params,
                                      out_length,
                                      &fail_phrase))
    {
        return false;
    }

    return true;
}

const char *Translations::GetGrenadeTranslationPhrase(GrenadeType grenadeType)
{
    return ChooseStringForGrenadeType(
            grenadeType,
            "Grenades display HE grenade",
            "Grenades display molotov grenade",
            "Grenades display incendiary grenade",
            "Grenades display decoy grenade",
            "Grenades display flashbang",
            "Grenades display smoke grenade");
}

const char *Translations::GetGrenadeThrowTickRateTranslationPhrase(GrenadeThrowTickRate grenadeThrowTickRate)
{
    switch (grenadeThrowTickRate)
    {
        case GrenadeThrowTickRate_64:
            return "Grenades grenade throw tickrate 64";

        case GrenadeThrowTickRate_128:
            return "Grenades grenade throw tickrate 128";
    }

    return "";
}

const char *Translations::GetShortGrenadesTranslationPhrase(bool shortGrenades)
{
    if (shortGrenades)
        return "Grenades short grenades on";

    return "Grenades short grenades off";
}

const char *Translations::GetGodModeTranslationPhrase(GodMode godMode)
{
    switch (godMode)
    {
        case GodMode_OFF:
            return "Grenades god mode off";

        case GodMode_ON:
            return "Grenades god mode on";

        case GodMode_SELFONLY:
            return "Grenades god mode only take self damage";

        case GodMode_NADEONLY:
            return "Grenades god mode only take grenade damage";

        case GodMode_GROUP:
            return "Grenades god mode only take group damage";

        case GodMode_GROUPNADEONLY:
            return "Grenades god mode only take group grenade damage";
    }

    return "";
}

const char *Translations::GetProjectileModeTranslationPhrase(ProjectileMode projectileMode)
{
    switch (projectileMode)
    {
        case ProjectileMode_ALL:
            return "Grenades projectile mode display all";

        case ProjectileMode_NONE:
            return "Grenades projectile mode display none";

        case ProjectileMode_SELF:
            return "Grenades projectile mode display self";

        case ProjectileMode_GROUP:
            return "Grenades projectile mode display group";
    }

    return "";
}

const char *Translations::GetGrenadeModeMenuItemTranslationPhrase(GrenadeType grenadeType)
{
    return ChooseStringForGrenadeType(
            grenadeType,
            "Grenades menu HE grenade mode",
            "Grenades menu molotov mode",
            "Grenades menu incendiary mode",
            "Grenades menu decoy mode",
            "Grenades menu flash mode",
            "Grenades menu smoke mode");
}

const char *Translations::GetPlayerModeTranslationPhrase(PlayerMode mode)
{
    switch (mode)
    {
        case PlayerMode_ALL:
            return "Grenades player mode display all";

        case PlayerMode_GROUP:
            return "Grenades player mode display group";

        case PlayerMode_NONE:
            return "Grenades player mode display none";
    }

    return "";
}