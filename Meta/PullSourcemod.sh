#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Please specify the directory to pull Sourcemod into."
    exit 1
fi

JEZ_SM_DIR=$1

echo "Pulling Sourcemod into $JEZ_SM_DIR"

mkdir -p "$JEZ_SM_DIR"

# Do we have a repository here yet?
if [[ ! -d "$JEZ_SM_DIR/.git" ]]
then
	if ! git clone git@github.com:alliedmodders/sourcemod.git "$JEZ_SM_DIR"
	then
		echo "Failed to clone Sourcemod Git repository, halting!"
		exit 1
	fi

	echo "Cloned Sourcemod Git repository into: $JEZ_SM_DIR"
fi

JEZ_CHANGES_COUNT=$(cd "$JEZ_SM_DIR" && git status --porcelain=v1 2>/dev/null | wc -l)

if [[ $JEZ_CHANGES_COUNT -gt 0 ]]
then
	echo "It looks like you have unstaged changes in the Sourcemod directory, do something with these before running this command."
	exit 1
fi

# Get us to the point where the repo is cloned and pointed to master
(cd "$JEZ_SM_DIR" && git fetch origin && git reset --hard origin/master && git submodule update --init --recursive)