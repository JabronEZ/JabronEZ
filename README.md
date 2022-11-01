# Overview

JabronEZ is a SourceMod extension which enables CS:GO players and teams to practice. It provides a grenade practice mode, which allows players to practice line-ups, preview what their grenades look like from other player's perspective, and much more.

## Local Development Setup

Currently, these instructions only apply to Linux. As of the time of writing, the machine used during testing was running Ubuntu 20.04, but we will update these instructions soon with multi-platform support.

### Installing Dependencies

```bash
sudo apt-get install clang lib32z1-dev libc6-dev-i386 linux-libc-dev:i386 g++-multilib cmake
```

### Cloning SDK Dependencies

You must clone the following repositories for building the extension:

SourceMod: https://github.com/alliedmodders/sourcemod
- Branch name: Use the branch that corresponds to your installed SourceMod build

MetaMod Source: https://github.com/alliedmodders/metamod-source/
- Branch name: Use the branch that corresponds to your installed MetaMod Source build

HL2SDK (CSGO): https://github.com/alliedmodders/hl2sdk
- Branch name: csgo

Take special care to check out to the appropriate branches as mentioned above, as this will impact compile time for the below steps.

### Building the Extension

```bash
mkdir build
cd build
cmake .. -DSMSDK=/home/anthony/Git/alliedmodders/sourcemod -DMMSOURCE=/home/anthony/Git/alliedmodders/mmsource-1.10 -DHL2SDK=/home/anthony/Git/alliedmodders/hl2sdk-csgo
```