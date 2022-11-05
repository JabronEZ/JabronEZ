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

#ifndef JABRONEZ_VIRTUAL_CALLABLES_MACROS_H
#define JABRONEZ_VIRTUAL_CALLABLES_MACROS_H

// FIXME: All the macros for definitions should probably take a parameter that allows you to offset the vtable address,
//        similar to how CreateVCall() does in bintools.

#define JEZ_VIRTUAL_CALLABLES_DECL0(name, className, ret) \
    extern int g_VirtualCallablesOffset##name; \
    class Virtual_Callables_##name##_Class \
    { \
    public: \
        static ret (Virtual_Callables_##name##_Class::* Virtual_Callables_##name##_Actual)(); \
    }; \
    extern ret Virtual_Callables_Call_##name(className *self)

#define JEZ_VIRTUAL_CALLABLES_DECL1(name, className, ret, p1type, p1name) \
    extern int g_VirtualCallablesOffset##name; \
    class Virtual_Callables_##name##_Class \
    { \
    public: \
        static ret (Virtual_Callables_##name##_Class::* Virtual_Callables_##name##_Actual)(p1type); \
    }; \
    extern ret Virtual_Callables_Call_##name(className *self, p1type p1name)

#define JEZ_VIRTUAL_CALLABLES_DECL5(name, className, ret, p1type, p1name, p2type, p2name, p3type, p3name, p4type, p4name, p5type, p5name) \
    extern int g_VirtualCallablesOffset##name; \
    class Virtual_Callables_##name##_Class \
    { \
    public: \
        static ret (Virtual_Callables_##name##_Class::* Virtual_Callables_##name##_Actual)(p1type, p2type, p3type, p4type, p5type); \
    }; \
    extern ret Virtual_Callables_Call_##name(className *self, p1type p1name, p2type p2name, p3type p3name, p4type p4name, p5type p5name)

#define JEZ_VIRTUAL_CALLABLES_DEF0(name, className, ret) \
    int g_VirtualCallablesOffset##name = -1; \
    ret (Virtual_Callables_##name##_Class::* Virtual_Callables_##name##_Class::Virtual_Callables_##name##_Actual)() = nullptr; \
    ret Virtual_Callables_Call_##name(className *self) \
    { \
        uint32_t *vtableAddress = *(reinterpret_cast<uint32_t**>(self)); \
        uint32_t *methodAddress = reinterpret_cast<uint32_t*>(*(vtableAddress + (uint32_t)g_VirtualCallablesOffset##name));    \
        *((void **)(&Virtual_Callables_##name##_Class::Virtual_Callables_##name##_Actual)) = methodAddress; \
        Virtual_Callables_##name##_Class *selfAsHook = reinterpret_cast<Virtual_Callables_##name##_Class *>(self); \
        return (selfAsHook->*Virtual_Callables_##name##_Class::Virtual_Callables_##name##_Actual)(); \
    }

#define JEZ_VIRTUAL_CALLABLES_DEF1(name, className, ret, p1type, p1name) \
    int g_VirtualCallablesOffset##name = -1; \
    ret (Virtual_Callables_##name##_Class::* Virtual_Callables_##name##_Class::Virtual_Callables_##name##_Actual)(p1type) = nullptr; \
    ret Virtual_Callables_Call_##name(className *self, p1type p1name) \
    { \
        uint32_t *vtableAddress = *(reinterpret_cast<uint32_t**>(self)); \
        uint32_t *methodAddress = reinterpret_cast<uint32_t*>(*(vtableAddress + (uint32_t)g_VirtualCallablesOffset##name)); \
        *((void **)(&Virtual_Callables_##name##_Class::Virtual_Callables_##name##_Actual)) = methodAddress; \
        Virtual_Callables_##name##_Class *selfAsHook = reinterpret_cast<Virtual_Callables_##name##_Class *>(self); \
        return (selfAsHook->*Virtual_Callables_##name##_Class::Virtual_Callables_##name##_Actual)(p1name); \
    }

#define JEZ_VIRTUAL_CALLABLES_DEF5(name, className, ret, p1type, p1name, p2type, p2name, p3type, p3name, p4type, p4name, p5type, p5name) \
    int g_VirtualCallablesOffset##name = -1; \
    ret (Virtual_Callables_##name##_Class::* Virtual_Callables_##name##_Class::Virtual_Callables_##name##_Actual)(p1type, p2type, p3type, p4type, p5type) = nullptr; \
    ret Virtual_Callables_Call_##name(className *self, p1type p1name, p2type p2name, p3type p3name, p4type p4name, p5type p5name) \
    { \
        uint32_t *vtableAddress = *(reinterpret_cast<uint32_t**>(self)); \
        uint32_t *methodAddress = reinterpret_cast<uint32_t*>(*(vtableAddress + (uint32_t)g_VirtualCallablesOffset##name)); \
        *((void **)(&Virtual_Callables_##name##_Class::Virtual_Callables_##name##_Actual)) = methodAddress; \
        Virtual_Callables_##name##_Class *selfAsHook = reinterpret_cast<Virtual_Callables_##name##_Class *>(self); \
        return (selfAsHook->*Virtual_Callables_##name##_Class::Virtual_Callables_##name##_Actual)(p1name, p2name, p3name, p4name, p5name); \
    }

#define JEZ_VIRTUAL_CALLABLE_CREATE(name, functionName, gamedata) \
    if (!gamedata->GetOffset(functionName, &g_VirtualCallablesOffset##name)) \
    { \
        snprintf(error, maxlength, "Unable to find offset for virtual callable %s", #name); \
        return false; \
    } \
    (void) g_VirtualCallablesOffset##name

#endif