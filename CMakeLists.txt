cmake_minimum_required(VERSION 2.8)

set(SMSDK "" CACHE PATH "Path to the SourceMod repository")
set(HL2SDK "" CACHE PATH "Path to the HL2SDK for CSGO")
set(MMSOURCE "" CACHE PATH "Path to the MetaMod source repository")

if (SMSDK STREQUAL "" OR HL2SDK STREQUAL "" OR MMSOURCE STREQUAL "")
    message("WARNING! You are missing one or more variables that are required (SMSDK, HL2SDK, MMSOURCE), make sure you pass them in when using CMAKE!")
endif()

# Although this might not be necessary, we want to match how AMBuild works as closely as possible, so clear out the default values that CMake provides.
if (WIN32)
    set(CMAKE_C_FLAGS "")
    set(CMAKE_C_FLAGS_DEBUG "")
    set(CMAKE_C_FLAGS_RELEASE "")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "")
    set(CMAKE_C_FLAGS_MINSIZEREL "")
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "")
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

if (WIN32)
    set(HL2LIB "${HL2SDK}/lib/public")
else()
    set(HL2LIB "${HL2SDK}/lib/linux")
endif ()

string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_LOWERCASE)

project(jabronez)

add_library(jabronez SHARED
        src/extension.cpp
        src/extension.h
        src/hooks.cpp
        src/hooks.h
        src/hooks_macros.h
        src/callables.cpp
        src/callables.h
        src/callables_macros.h
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
        src/projectile_parameters.cpp
        src/projectile_parameters.h
        src/hud_utilities.cpp
        src/hud_utilities.h
        src/entity_utilities.cpp
        src/entity_utilities.h
        src/draw_spots_timer.cpp
        src/draw_spots_timer.h
        src/temporary_entities.cpp
        src/temporary_entities.h
        src/temporary_entity_creator.cpp
        src/temporary_entity_creator.h
        src/recipient_filter.cpp
        src/recipient_filter.h
        src/player_timer.cpp
        src/player_timer.h
        src/handle_playback_timer.cpp
        src/handle_playback_timer.h
        src/grenade_trigger_playback_timer.cpp
        src/grenade_trigger_playback_timer.h
        src/grenade_goto_next_spot_or_finish_timer.cpp
        src/grenade_goto_next_spot_or_finish_timer.h
        src/game_events_manager.cpp
        src/game_events_manager.h
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

if (WIN32)
    target_compile_definitions(jabronez PRIVATE
             _CRT_SECURE_NO_DEPRECATE
             _CRT_SECURE_NO_WARNINGS
             _CRT_NONSTDC_NO_DEPRECATE
             _ITERATOR_DEBUG_LEVEL=0
             WIN32
             _WINDOWS
             SOURCEMOD_BUILD
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
            COMPILER_MSVC
            COMPILER_MSVC32
            NETWORK_VARS_ENABLED
            HAVE_STRING_H
            )

    if (BUILD_TYPE_LOWERCASE STREQUAL "debug")
        target_compile_definitions(jabronez PRIVATE
                _DEBUG
                DEBUG
                )

        target_compile_options(jabronez PRIVATE
                /MTd
                /Od
                /RTC1
                )

        target_link_options(jabronez PRIVATE
                /NODEFAULTLIB:libcmt
                )
    else()
        target_compile_options(jabronez PRIVATE
                /MT
                )
    endif()

    target_compile_options(jabronez PRIVATE
            /W3
            /Oy-
            /Z7
            $<$<COMPILE_LANGUAGE:CXX>:/EHsc>
            $<$<COMPILE_LANGUAGE:CXX>:/GR->
            $<$<COMPILE_LANGUAGE:CXX>:/TP>
            )

    target_link_libraries(jabronez PRIVATE
            kernel32.lib
            user32.lib
            gdi32.lib
            winspool.lib
            comdlg32.lib
            advapi32.lib
            shell32.lib
            ole32.lib
            oleaut32.lib
            uuid.lib
            odbc32.lib
            odbccp32.lib
            legacy_stdio_definitions.lib
            )
else()
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
            $<$<COMPILE_LANGUAGE:CXX>:-Wno-class-memaccess>
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
endif ()

target_include_directories(jabronez PRIVATE
	src
        ${HL2SDK}/public/game/server
        ${HL2SDK}/game/shared
        ${HL2PUB}
        ${HL2PUB}/mathlib
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

if (WIN32)
    target_link_libraries(jabronez PRIVATE
            ${HL2LIB}/tier0.lib
            ${HL2LIB}/tier1.lib
            ${HL2LIB}/vstdlib.lib
            ${HL2LIB}/mathlib.lib
            ${HL2LIB}/interfaces.lib
            )
else()
    target_link_libraries(jabronez PRIVATE
            ${HL2LIB}/tier1_i486.a
            ${HL2LIB}/mathlib_i486.a
            ${HL2LIB}/interfaces_i486.a
            ${HL2LIB}/libvstdlib.so
            ${HL2LIB}/libtier0.so
            )
endif()

set_target_properties(jabronez PROPERTIES PREFIX "")
set_target_properties(jabronez PROPERTIES OUTPUT_NAME "jabronez.ext")

if (WIN32)
    set_target_properties(jabronez PROPERTIES SUFFIX ".dll")
else()
    set_target_properties(jabronez PROPERTIES SUFFIX ".so")
endif ()
