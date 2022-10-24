#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Please specify the Sourcemod directory to apply Freedom35 patches to, as well as the patches directory."
    exit 1
fi

JEZ_SM_DIR=$1
JEZ_PATCHES_DIR=$2

mkdir -p "$JEZ_SM_DIR"

# Do we have a repository here yet?
if [[ ! -d "$JEZ_SM_DIR/.git" ]]
then
	echo "The Git repository does not exist for Sourcemod, you probably need to run PullSourcemod.sh first!"
	exit 1
fi

echo "Applying patches from $JEZ_PATCHES_DIR..."

(cd "$JEZ_SM_DIR" && git am --3way --ignore-space-change "$JEZ_PATCHES_DIR/0001-Freedom35-Add-support-for-FireEventEx.patch")