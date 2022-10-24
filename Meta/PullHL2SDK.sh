#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Please specify the directory to pull Sourcemod into."
    exit 1
fi

JEZ_HL2SDK_DIR=$1

echo "Pulling HL2SDK into $JEZ_HL2SDK_DIR"

mkdir -p "$JEZ_HL2SDK_DIR"

# Do we have a repository here yet?
if [[ ! -d "$JEZ_HL2SDK_DIR/.git" ]]
then
	if ! git clone git@github.com:alliedmodders/hl2sdk.git "$JEZ_HL2SDK_DIR"
	then
		echo "Failed to clone HL2SDK Git repository, halting!"
		exit 1
	fi

	echo "Cloned HL2SDK Git repository into: $JEZ_HL2SDK_DIR"
fi

JEZ_CHANGES_COUNT=$(cd "$JEZ_HL2SDK_DIR" && git status --porcelain=v1 2>/dev/null | wc -l)

if [[ $JEZ_CHANGES_COUNT -gt 0 ]]
then
	echo "It looks like you have unstaged changes in the HL2SDK directory, do something with these before running this command."
	exit 1
fi

# Get us to the point where the repo is cloned and pointed to csgo
(cd "$JEZ_HL2SDK_DIR" && git fetch origin && git reset --hard origin/csgo && git submodule update --init --recursive)