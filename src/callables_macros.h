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

#ifndef JABRONEZ_CALLABLES_MACROS_H
#define JABRONEZ_CALLABLES_MACROS_H

// FIXME: All the macros for definitions with offsets should probably take a parameter that allows you to offset the vtable address,
//        similar to how CreateVCall() does in bintools.

#define JEZ_CALLABLES_MEMBER_OFFSET_DECL0(name, className, ret) \
    extern int g_CallablesOffset##name; \
    class Callables_##name##_Class \
    { \
    public: \
        static ret (Callables_##name##_Class::* Callables_##name##_Actual)(); \
    }; \
    ret Callables_Call_##name(className *self)

#define JEZ_CALLABLES_MEMBER_OFFSET_DECL0_VOID(name, className) \
    extern int g_CallablesOffset##name;                         \
    class Callables_##name##_Class \
    { \
    public: \
        static void (Callables_##name##_Class::* Callables_##name##_Actual)(); \
    }; \
    void Callables_Call_##name(className *self)

#define JEZ_CALLABLES_MEMBER_SIG_DECL0_VOID(name, className) \
    extern void *g_CallablesAddress##name; \
    class Callables_##name##_Class \
    { \
    public: \
        static void (Callables_##name##_Class::* Callables_##name##_Actual)(); \
    }; \
    extern void Callables_Call_##name(className *self)

#define JEZ_CALLABLES_MEMBER_OFFSET_DECL1(name, className, ret, p1type, p1name) \
    extern int g_CallablesOffset##name; \
    class Callables_##name##_Class \
    { \
    public: \
        static ret (Callables_##name##_Class::* Callables_##name##_Actual)(p1type); \
    }; \
    extern ret Callables_Call_##name(className *self, p1type p1name)

#define JEZ_CALLABLES_MEMBER_OFFSET_DECL5(name, className, ret, p1type, p1name, p2type, p2name, p3type, p3name, p4type, p4name, p5type, p5name) \
    extern int CallablesOffset##name; \
    class Callables_##name##_Class \
    { \
    public: \
        static ret (Callables_##name##_Class::* Callables_##name##_Actual)(p1type, p2type, p3type, p4type, p5type); \
    }; \
    extern ret Callables_Call_##name(className *self, p1type p1name, p2type p2name, p3type p3name, p4type p4name, p5type p5name)

#define JEZ_CALLABLES_MEMBER_OFFSET_DEF0(name, className, ret) \
    int g_CallablesOffset##name = -1; \
    ret (Callables_##name##_Class::* Callables_##name##_Class::Callables_##name##_Actual)() = nullptr; \
    ret Callables_Call_##name(className *self) \
    { \
        uint32_t *vtableAddress = *(reinterpret_cast<uint32_t**>(self)); \
        uint32_t *methodAddress = reinterpret_cast<uint32_t*>(*(vtableAddress + (uint32_t)g_CallablesOffset##name));    \
        *((void **)(&Callables_##name##_Class::Callables_##name##_Actual)) = methodAddress; \
        Callables_##name##_Class *selfAsHook = reinterpret_cast<Callables_##name##_Class *>(self); \
        return (selfAsHook->*Callables_##name##_Class::Callables_##name##_Actual)(); \
    }

#define JEZ_CALLABLES_MEMBER_OFFSET_DEF0_VOID(name, className) \
    int g_CallablesOffset##name = -1; \
    void (Callables_##name##_Class::* Callables_##name##_Class::Callables_##name##_Actual)() = nullptr; \
    void Callables_Call_##name(className *self) \
    { \
        uint32_t *vtableAddress = *(reinterpret_cast<uint32_t**>(self)); \
        uint32_t *methodAddress = reinterpret_cast<uint32_t*>(*(vtableAddress + (uint32_t)g_CallablesOffset##name));    \
        *((void **)(&Callables_##name##_Class::Callables_##name##_Actual)) = methodAddress; \
        Callables_##name##_Class *selfAsHook = reinterpret_cast<Callables_##name##_Class *>(self); \
        (selfAsHook->*Callables_##name##_Class::Callables_##name##_Actual)(); \
    }

#define JEZ_CALLABLES_MEMBER_SIG_DEF0_VOID(name, className) \
    void *g_CallablesAddress##name = nullptr; \
    void (Callables_##name##_Class::* Callables_##name##_Class::Callables_##name##_Actual)() = nullptr; \
    void Callables_Call_##name(className *self) \
    { \
        *((void **)(&Callables_##name##_Class::Callables_##name##_Actual)) = g_CallablesAddress##name; \
        Callables_##name##_Class *selfAsHook = reinterpret_cast<Callables_##name##_Class *>(self); \
        (selfAsHook->*Callables_##name##_Class::Callables_##name##_Actual)(); \
    }

#define JEZ_CALLABLES_MEMBER_OFFSET_DEF1(name, className, ret, p1type, p1name) \
    int g_CallablesOffset##name = -1; \
    ret (Callables_##name##_Class::* Callables_##name##_Class::Callables_##name##_Actual)(p1type) = nullptr; \
    ret Callables_Call_##name(className *self, p1type p1name) \
    { \
        uint32_t *vtableAddress = *(reinterpret_cast<uint32_t**>(self)); \
        uint32_t *methodAddress = reinterpret_cast<uint32_t*>(*(vtableAddress + (uint32_t)g_CallablesOffset##name)); \
        *((void **)(&Callables_##name##_Class::Callables_##name##_Actual)) = methodAddress; \
        Callables_##name##_Class *selfAsHook = reinterpret_cast<Callables_##name##_Class *>(self); \
        return (selfAsHook->*Callables_##name##_Class::Callables_##name##_Actual)(p1name); \
    }

#define JEZ_CALLABLES_MEMBER_OFFSET_DEF5(name, className, ret, p1type, p1name, p2type, p2name, p3type, p3name, p4type, p4name, p5type, p5name) \
    int g_CallablesOffset##name = -1; \
    ret (Callables_##name##_Class::* Callables_##name##_Class::Callables_##name##_Actual)(p1type, p2type, p3type, p4type, p5type) = nullptr; \
    ret Callables_Call_##name(className *self, p1type p1name, p2type p2name, p3type p3name, p4type p4name, p5type p5name) \
    { \
        uint32_t *vtableAddress = *(reinterpret_cast<uint32_t**>(self)); \
        uint32_t *methodAddress = reinterpret_cast<uint32_t*>(*(vtableAddress + (uint32_t)g_CallablesOffset##name)); \
        *((void **)(&Callables_##name##_Class::Callables_##name##_Actual)) = methodAddress; \
        Callables_##name##_Class *selfAsHook = reinterpret_cast<Callables_##name##_Class *>(self); \
        return (selfAsHook->*Callables_##name##_Class::Callables_##name##_Actual)(p1name, p2name, p3name, p4name, p5name); \
    }

#define JEZ_CALLABLE_MEMBER_OFFSET_CREATE(name, functionName, gamedata) \
    if (!gamedata->GetOffset(functionName, &g_CallablesOffset##name)) \
    { \
        snprintf(error, maxlength, "Unable to find offset for callable %s", #name); \
        return false; \
    } \
    (void) g_CallablesOffset##name

#define JEZ_CALLABLE_MEMBER_SIG_CREATE(name, functionName, gamedata) \
    if (!gamedata->GetMemSig(functionName, &g_CallablesAddress##name) || g_CallablesAddress##name == nullptr) \
    { \
        snprintf(error, maxlength, "Unable to find address for callable %s", #name); \
        return false; \
    } \
    (void) g_CallablesAddress##name

#endif