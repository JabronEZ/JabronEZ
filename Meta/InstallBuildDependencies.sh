#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Please specify the dependencies directory."
    exit 1
fi

JEZ_DEPENDENCIES_DIR=$1
JEZ_AMBUILD_DIR="$JEZ_DEPENDENCIES_DIR/AMBuild"
JEZ_MYSQL_DIR="$JEZ_DEPENDENCIES_DIR/MySQL"
mkdir -p "$JEZ_DEPENDENCIES_DIR"
mkdir -p "$JEZ_AMBUILD_DIR"
mkdir -p "$JEZ_MYSQL_DIR"

wget https://cdn.mysql.com/archives/mysql-5.6/mysql-5.6.15-linux-glibc2.5-x86_64.tar.gz -P "$JEZ_DEPENDENCIES_DIR"
tar -xf "$JEZ_DEPENDENCIES_DIR/mysql-5.6.15-linux-glibc2.5-x86_64.tar.gz" -C "$JEZ_MYSQL_DIR"

# Do we have a repository here yet?
if [[ ! -d "$JEZ_AMBUILD_DIR/.git" ]]
then
	if ! git clone https://github.com/alliedmodders/ambuild.git "$JEZ_AMBUILD_DIR"
	then
		echo "Failed to clone AMBuild Git repository, halting!"
		exit 1
	fi

	echo "Cloned AMBuild Git repository into: $JEZ_AMBUILD_DIR"
fi

JEZ_CHANGES_COUNT=$(cd "$JEZ_AMBUILD_DIR" && git status --porcelain=v1 2>/dev/null | wc -l)

if [[ $JEZ_CHANGES_COUNT -gt 0 ]]
then
	echo "It looks like you have unstaged changes in the AMBuild directory, do something with these before running this command."
	exit 1
fi

# Get us to the point where the repo is cloned and pointed to master
(cd "$JEZ_AMBUILD_DIR" && git fetch origin && git reset --hard origin/master && git submodule update --init --recursive)

# Install PIP dependencies
(cd "$JEZ_AMBUILD_DIR" && pip install "$JEZ_AMBUILD_DIR")