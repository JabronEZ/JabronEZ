cmake_minimum_required(VERSION 2.8)

set(SMSDK "" CACHE PATH "Path to the SourceMod repository")
set(HL2SDK "" CACHE PATH "Path to the HL2SDK for CSGO")
set(MMSOURCE "" CACHE PATH "Path to the MetaMod source repository")

if (SMSDK STREQUAL "" OR HL2SDK STREQUAL "" OR MMSOURCE STREQUAL "")
    message("WARNING! You are missing one or more variables that are required (SMSDK, HL2SDK, MMSOURCE), make sure you pass them in when using CMAKE!")
endif()

if (SMSDK STREQUAL "")
    set(SMEXTCPP "")
else()
    set(SMEXTCPP "${SMSDK}/public/smsdk_ext.cpp")
endif()

set(HL2PUB "${HL2SDK}/public")
set(METAMOD "${MMSOURCE}/core")
set(HL2LIB "${HL2SDK}/lib/linux")
string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_LOWERCASE)

project(jabronez)

add_library(jabronez SHARED
        src/extension.cpp
        src/extension.h
        ${SMEXTCPP}
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
)

target_compile_options(jabronez PRIVATE
        -fvisibility=hidden
        -fvisibility-inlines-hidden
        -std=c++14
        -Wall
        -Werror
        -Wno-overloaded-virtual
        -Wno-switch
        -Wno-unused
        -Wno-non-virtual-dtor
        -Wno-delete-non-virtual-dtor
        -fno-exceptions
        -fno-rtti
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
        ${HL2LIB}/interfaces_i486.a
        ${HL2LIB}/libvstdlib.so
        ${HL2LIB}/libtier0.so
        )

set_target_properties(jabronez PROPERTIES PREFIX "")
set_target_properties(jabronez PROPERTIES OUTPUT_NAME "jabronez.ext")
set_target_properties(jabronez PROPERTIES SUFFIX ".so")
