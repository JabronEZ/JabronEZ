#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Please specify the root directory of the Freedom35 repository."
    exit 1
fi

JEZ_DIR=$1

# FIXME: This is not seeming to work with ../ and ./ as intended.
abspath() {                                               
    cd "$(dirname "$1")"
    printf "%s/%s\n" "$(pwd)" "$(basename "$1")"
    cd "$OLDPWD"
}

JEZ_ABS_DIR=$(abspath "$JEZ_DIR")

echo "Freedom35 repository directory: $JEZ_ABS_DIR"

if [ ! -d "$JEZ_ABS_DIR" ]; then
	echo "$JEZ_ABS_DIR does not exist, please try again."
	exit 1
fi

# Make sure that we have Git installed
JEZ_GIT_VERSION=$(git --version)

if [[ $JEZ_GIT_VERSION == "" ]]
then
    echo "Git does not appear to be installed. Make sure you install it first and put it in your system's PATH."
    exit 1
fi

JEZ_META_DIR="$JEZ_ABS_DIR/Meta"
JEZ_SM_DIR="$JEZ_ABS_DIR/Sourcemod"
JEZ_PATCHES_DIR="$JEZ_ABS_DIR/Patches"
JEZ_HL2SDK_DIR="$JEZ_ABS_DIR/HL2SDK"
JEZ_SOURCEMM_DIR="$JEZ_ABS_DIR/SourceMM"

if ! "$JEZ_META_DIR/PullSourcemod.sh" "$JEZ_SM_DIR"
then
	echo "Unable to pull Sourcemod codebase, halting."
	exit 1
fi

if ! "$JEZ_META_DIR/ApplyPatches.sh" "$JEZ_SM_DIR" "$JEZ_PATCHES_DIR"
then
	echo "Unable to apply patches, halting."
	exit 1
fi

if ! "$JEZ_META_DIR/PullHL2SDK.sh" "$JEZ_HL2SDK_DIR"
then
	echo "Unable to pull HL2SDK codebase, halting."
	exit 1
fi

if ! "$JEZ_META_DIR/PullSourceMM.sh" "$JEZ_SOURCEMM_DIR"
then
	echo "Unable to pull SourceMM codebase, halting."
	exit 1
fi

echo "Development setup complete."