#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Please specify the directory to pull SourceMM into."
    exit 1
fi

JEZ_SOURCEMM_DIR=$1

echo "Pulling SourceMM into $JEZ_SOURCEMM_DIR"

mkdir -p "$JEZ_SOURCEMM_DIR"

# Do we have a repository here yet?
if [[ ! -d "$JEZ_SOURCEMM_DIR/.git" ]]
then
	if ! git clone git@github.com:alliedmodders/metamod-source.git "$JEZ_SOURCEMM_DIR"
	then
		echo "Failed to clone SourceMM Git repository, halting!"
		exit 1
	fi

	echo "Cloned SourceMM Git repository into: $JEZ_SOURCEMM_DIR"
fi

JEZ_CHANGES_COUNT=$(cd "$JEZ_SOURCEMM_DIR" && git status --porcelain=v1 2>/dev/null | wc -l)

if [[ $JEZ_CHANGES_COUNT -gt 0 ]]
then
	echo "It looks like you have unstaged changes in the SourceMM directory, do something with these before running this command."
	exit 1
fi

# Get us to the point where the repo is cloned and pointed to master
(cd "$JEZ_SOURCEMM_DIR" && git fetch origin && git reset --hard origin/master && git submodule update --init --recursive)