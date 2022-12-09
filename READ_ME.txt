Linux:
	Requirements
		Install required tools
			sudo apt install cmake
			sudo apt install build-essential git
		Install required libraries
			sudo apt install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
	To build and run on linux:
		execute build.sh
		execute run.sh

Windows:
	To build and run on windows: (Requires MinGW)
		Start from directory "RaylibCmake"
		execute following commands in powershell
			cmake -S . -B build -G "MinGW Makefiles"
			cd build
			mingw32-make
			./game
	