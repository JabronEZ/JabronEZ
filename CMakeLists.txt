cmake_minimum_required(VERSION 2.8)

set(SMSDK "" CACHE PATH "Path to the SourceMod repository")
set(HL2SDK "" CACHE PATH "Path to the HL2SDK for CSGO")
set(MMSOURCE "" CACHE PATH "Path to the MetaMod source repository")

if (SMSDK STREQUAL "" OR HL2SDK STREQUAL "" OR MMSOURCE STREQUAL "")
    message("WARNING! You are missing one or more variables that are required (SMSDK, HL2SDK, MMSOURCE), make sure you pass them in when using CMAKE!")
endif()

if (SMSDK STREQUAL "")
    set(SMEXTCPP "")
    set(CDETOURCPP "")
    set(ASMCPP "")
    set(LIBUDIS86DECODEC "")
    set(LIBUDIS86ITABC "")
    set(LIBUDIS86SYNATTC "")
    set(LIBUDIS86SYNINTELC "")
    set(LIBUDIS86SYNC "")
    set(LIBUDIS86C "")
else()
    set(SMEXTCPP "${SMSDK}/public/smsdk_ext.cpp")
    set(CDETOURCPP "${SMSDK}/public/CDetour/detours.cpp")
    set(ASMCPP "${SMSDK}/public/asm/asm.c")
    set(LIBUDIS86DECODEC "${SMSDK}/public/libudis86/decode.c")
    set(LIBUDIS86ITABC "${SMSDK}/public/libudis86/itab.c")
    set(LIBUDIS86SYNATTC "${SMSDK}/public/libudis86/syn-att.c")
    set(LIBUDIS86SYNINTELC "${SMSDK}/public/libudis86/syn-intel.c")
    set(LIBUDIS86SYNC "${SMSDK}/public/libudis86/syn.c")
    set(LIBUDIS86C "${SMSDK}/public/libudis86/udis86.c")
endif()

set(HL2PUB "${HL2SDK}/public")
set(METAMOD "${MMSOURCE}/core")
set(HL2LIB "${HL2SDK}/lib/linux")
string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_LOWERCASE)

project(jabronez)

add_library(jabronez SHARED
        src/extension.cpp
        src/extension.h
        src/hooks.cpp
        src/hooks.h
        src/hooks_macros.h
        src/console_manager.cpp
        src/console_manager.h
        src/player_manager.cpp
        src/player_manager.h
        src/menu_manager.cpp
        src/menu_manager.h
        src/translations.cpp
        src/translations.h
        src/weapon_identifiers.cpp
        src/weapon_identifiers.h
        src/spot.cpp
        src/spot.h
        src/player.cpp
        src/player.h
        src/god_mode.h
        src/player_mode.h
        src/projectile_mode.cpp
        src/projectile_mode.h
        src/hud_utilities.cpp
        src/hud_utilities.h
        src/entity_utilities.cpp
        src/entity_utilities.h
        ${SMEXTCPP}
        ${CDETOURCPP}
        ${ASMCPP}
        ${LIBUDIS86DECODEC}
        ${LIBUDIS86ITABC}
        ${LIBUDIS86SYNATTC}
        ${LIBUDIS86SYNINTELC}
        ${LIBUDIS86SYNC}
        ${LIBUDIS86C}
)

target_compile_definitions(jabronez PRIVATE
        _LINUX
        POSIX
        stricmp=strcasecmp
        _stricmp=strcasecmp
        _strnicmp=strncasecmp
        strnicmp=strncasecmp
        _snprintf=snprintf
        _vsnprintf=vsnprintf
        _alloca=alloca
        strcmpi=strcasecmp
        COMPILER_GCC
        SOURCE_ENGINE=12
        SE_EPISODEONE=1
        SE_DARKMESSIAH=2
        SE_ORANGEBOX=3
        SE_BLOODYGOODTIME=4
        SE_EYE=5
        SE_CSS=6
        SE_ORANGEBOXVALVE=7
        SE_LEFT4DEAD=8
        SE_LEFT4DEAD2=9
        SE_ALIENSWARM=10
        SE_PORTAL2=11
        SE_CSGO=12
        SOURCEMOD_BUILD
        HAVE_STDINT_H
        HAVE_STRING_H
)

target_compile_options(jabronez PRIVATE
        -fvisibility=hidden
        $<$<COMPILE_LANGUAGE:CXX>:-fvisibility-inlines-hidden>
        $<$<COMPILE_LANGUAGE:CXX>:-std=c++14>
        -Wall
        -Werror
        $<$<COMPILE_LANGUAGE:CXX>:-Wno-overloaded-virtual>
        -Wno-switch
        -Wno-unused
        $<$<COMPILE_LANGUAGE:CXX>:-Wno-non-virtual-dtor>
        $<$<COMPILE_LANGUAGE:CXX>:-Wno-delete-non-virtual-dtor>
        -fno-exceptions
        $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
        -Wno-deprecated-register
        -msse
        -m32
        -mfpmath=sse
)

target_link_options(jabronez PRIVATE
        -shared
        -static-libgcc
        -m32
)

target_link_libraries(jabronez PRIVATE
        stdc++
        m
        dl
)

if (BUILD_TYPE_LOWERCASE STREQUAL "debug")
    target_compile_definitions(jabronez PRIVATE
            _DEBUG
            DEBUG
    )

    target_compile_options(jabronez PRIVATE
            -g
            -ggdb3
    )
else()
    target_compile_definitions(jabronez PRIVATE
            NDEBUG
    )

    target_compile_options(jabronez PRIVATE
            -O3
            -funroll-loops
            -pipe
            -fno-strict-aliasing
    )
endif()

target_include_directories(jabronez PRIVATE
	src
        ${HL2SDK}/public/game/server
        ${HL2PUB}
        ${HL2PUB}/engine
        ${HL2PUB}/tier0
        ${HL2PUB}/tier1
        ${METAMOD}
        ${METAMOD}/sourcehook
        ${SMSDK}
        ${SMSDK}/public
        ${SMSDK}/public/amtl/amtl
        ${SMSDK}/public/amtl
        ${SMSDK}/public/jit
        ${SMSDK}/public/jit/x86
        ${SMSDK}/extensions
        ${SMSDK}/sourcepawn
        ${SMSDK}/sourcepawn/include
)

target_link_libraries(jabronez PRIVATE
        ${HL2LIB}/tier1_i486.a
        ${HL2LIB}/mathlib_i486.a
        ${HL2LIB}/interfaces_i486.a
        ${HL2LIB}/libvstdlib.so
        ${HL2LIB}/libtier0.so
        )

set_target_properties(jabronez PROPERTIES PREFIX "")
set_target_properties(jabronez PROPERTIES OUTPUT_NAME "jabronez.ext")
set_target_properties(jabronez PROPERTIES SUFFIX ".so")
