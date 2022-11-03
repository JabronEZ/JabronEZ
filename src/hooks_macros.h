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

#ifndef JABRONEZ_HOOKS_MACROS_H
#define JABRONEZ_HOOKS_MACROS_H

#define JEZ_HOOK_STATIC_DECL6(name, ret, callConv, p1type, p1name, p2type, p2name, p3type, p3name, p4type, p4name, p5type, p5name, p6type, p6name) \
    extern CDetour *g_Detour##name; \
    typedef ret (callConv *name##Function)(p1type, p2type, p3type, p4type, p5type, p6type); \
    ret callConv Hook_Callback_##name(p1type p1name, p2type p2name, p3type p3name, p4type p4name, p5type p5name, p6type p6name); \
    extern name##Function Hook_Call_##name;

#define JEZ_HOOK_STATIC_DEF6(name, ret, callConv, p1type, p1name, p2type, p2name, p3type, p3name, p4type, p4name, p5type, p5name, p6type, p6name) \
    CDetour *g_Detour##name = nullptr; \
    name##Function Hook_Call_##name = nullptr; \
    ret callConv Hook_Callback_##name(p1type p1name, p2type p2name, p3type p3name, p4type p4name, p5type p5name, p6type p6name)

#define JEZ_HOOK_STATIC_CREATE(name, functionName) \
    g_Detour##name = CDetourManager::CreateDetour((void *)&Hook_Callback_##name, (void **)&Hook_Call_##name, functionName); \
    if (g_Detour##name == nullptr) \
    { \
        snprintf(error, maxlength, "Unable to hook %s", #name); \
        return false; \
    } \
    g_Detour##name->EnableDetour()

#endif