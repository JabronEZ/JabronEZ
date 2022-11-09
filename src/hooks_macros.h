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

#define JEZ_THIS_PTR(className) reinterpret_cast<className *>(this)

#define JEZ_HOOK_MEMBER_DECL1(name, className, ret, p1type, p1name) \
    extern CDetour *g_Detour##name; \
    class Hook_##name##_Class \
    { \
    public: \
        ret Hook_##name##_Callback(p1type p1name); \
        static ret (Hook_##name##_Class::* Hook_##name##_Actual)(p1type); \
    }; \
    extern ret Hook_Call_##name(className *self, p1type p1name)

#define JEZ_HOOK_MEMBER_DECL5(name, className, ret, p1type, p1name, p2type, p2name, p3type, p3name, p4type, p4name, p5type, p5name) \
    extern CDetour *g_Detour##name; \
    class Hook_##name##_Class \
    { \
    public: \
        ret Hook_##name##_Callback(p1type p1name, p2type p2name, p3type p3name, p4type p4name, p5type p5name); \
        static ret (Hook_##name##_Class::* Hook_##name##_Actual)(p1type, p2type, p3type, p4type, p5type); \
    }; \
    extern ret Hook_Call_##name(className *self, p1type p1name, p2type p2name, p3type p3name, p4type p4name, p5type p5name)

#define JEZ_HOOK_MEMBER_DEF1(name, className, ret, p1type, p1name) \
    CDetour *g_Detour##name = nullptr; \
    ret (Hook_##name##_Class::* Hook_##name##_Class::Hook_##name##_Actual)(p1type) = nullptr; \
    ret Hook_Call_##name(className *self, p1type p1name) \
    { \
        Hook_##name##_Class *selfAsHook = reinterpret_cast<Hook_##name##_Class *>(self); \
        return (selfAsHook->*Hook_##name##_Class::Hook_##name##_Actual)(p1name); \
    } \
    ret Hook_##name##_Class::Hook_##name##_Callback(p1type p1name)

#define JEZ_HOOK_MEMBER_DEF5(name, className, ret, p1type, p1name, p2type, p2name, p3type, p3name, p4type, p4name, p5type, p5name) \
    CDetour *g_Detour##name = nullptr; \
    ret (Hook_##name##_Class::* Hook_##name##_Class::Hook_##name##_Actual)(p1type, p2type, p3type, p4type, p5type) = nullptr; \
    ret Hook_Call_##name(className *self, p1type p1name, p2type p2name, p3type p3name, p4type p4name, p5type p5name) \
    { \
        Hook_##name##_Class *selfAsHook = reinterpret_cast<Hook_##name##_Class *>(self); \
        return (selfAsHook->*Hook_##name##_Class::Hook_##name##_Actual)(p1name, p2name, p3name, p4name, p5name); \
    } \
    ret Hook_##name##_Class::Hook_##name##_Callback(p1type p1name, p2type p2name, p3type p3name, p4type p4name, p5type p5name)

#define JEZ_HOOK_STATIC_DECL6(name, ret, callConv, p1type, p1name, p2type, p2name, p3type, p3name, p4type, p4name, p5type, p5name, p6type, p6name) \
    extern CDetour *g_Detour##name; \
    typedef ret (callConv *name##Function)(p1type, p2type, p3type, p4type, p5type, p6type); \
    ret callConv Hook_Callback_##name(p1type p1name, p2type p2name, p3type p3name, p4type p4name, p5type p5name, p6type p6name); \
    extern name##Function Hook_Original_##name; \
    extern ret Hook_Call_##name(p1type p1name, p2type p2name, p3type p3name, p4type p4name, p5type p5name, p6type p6name);

#define JEZ_HOOK_STATIC_DEF6(name, ret, callConv, p1type, p1name, p2type, p2name, p3type, p3name, p4type, p4name, p5type, p5name, p6type, p6name) \
    CDetour *g_Detour##name = nullptr; \
    name##Function Hook_Original_##name = nullptr; \
    ret Hook_Call_##name(p1type p1name, p2type p2name, p3type p3name, p4type p4name, p5type p5name, p6type p6name) \
    { \
        return Hook_Original_##name(p1name, p2name, p3name, p4name, p5name, p6name); \
    } \
    ret callConv Hook_Callback_##name(p1type p1name, p2type p2name, p3type p3name, p4type p4name, p5type p5name, p6type p6name)

#define JEZ_HOOK_STATIC_CREATE(name, functionName) \
    g_Detour##name = CDetourManager::CreateDetour((void *)&Hook_Callback_##name, (void **)&Hook_Original_##name, functionName); \
    if (g_Detour##name == nullptr) \
    { \
        snprintf(error, maxlength, "Unable to hook %s", #name); \
        return false; \
    } \
    g_Detour##name->EnableDetour()

#define JEZ_HOOK_STATIC_CREATE_EX(name, callbackName, originalName, functionName) \
    g_Detour##name = CDetourManager::CreateDetour((void *)&Hook_Callback_##callbackName, (void **)&Hook_Original_##originalName, functionName); \
    if (g_Detour##name == nullptr) \
    { \
        snprintf(error, maxlength, "Unable to hook %s", #name); \
        return false; \
    } \
    g_Detour##name->EnableDetour()

#define JEZ_HOOK_MEMBER_CREATE(name, functionName) \
    g_Detour##name = CDetourManager::CreateDetour(GetCodeAddress(&Hook_##name##_Class::Hook_##name##_Callback), (void **)(&Hook_##name##_Class::Hook_##name##_Actual), functionName); \
    if (g_Detour##name == nullptr) \
    { \
        snprintf(error, maxlength, "Unable to hook %s", #name); \
        return false; \
    } \
    g_Detour##name->EnableDetour()

#define JEZ_HOOK_CLEANUP(name) \
    if (g_Detour##name != nullptr) \
    { \
        g_Detour##name->Destroy(); \
    } \
    g_Detour##name = nullptr;

#endif