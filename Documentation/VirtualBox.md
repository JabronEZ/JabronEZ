Notes about VBox setup:
- Make sure your hard disk is at least 100gb so you can install srcds
- Set the network mode to "Bridged" so the machine gets a real IP (the IP should be like 192.168.1.32 or similar, in your ACTUAL subnet locally)
- When launching SRCDS, set +ip 192.168.1.32 (IP from guest machine)
- Make sure you install Guest Additions, which will need:
	sudo apt update && sudo apt upgrade && sudo apt install build-essential dkms linux-headers-$(uname -r)

	You will need to run ./autorun.sh from the mounted disk image
- In the VirtualBox settings, go to advanced and enable "Bi-Directional clipboard sharing"
- In the VirtualBox settings, setup the shared drive to the Git repo and "Auto-mount" it
- Add yourself to the group: sudo usermod -a -G vboxsf anthony
   You'll need to reboot after this
- Install git: sudo apt install git
- Install AMBuild requires this: python3 -m pip install <full path to dependencies/ambuild>
- git config --global --add safe.directory /media/sf_JabronEZ/Dependencies/Sourcemod

TODOs:
	- Need to fix the issue where the engine configuration isn't packaged in the final distribution
	- Install the build dependencies from here in InstallBuildDependencies.sh: https://wiki.alliedmods.net/Building_SourceMod#Linux
	- Make a script to auto install package into srcds
	- Figure out how to disable gamedata autoupdater in sourcemod