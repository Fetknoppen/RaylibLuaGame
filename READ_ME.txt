To build, compile, and run on linux:
	execute build.sh
	execute run.sh

To build, compile, and run on windows: (Requires MinGW)
	execute following commands in powershell
	Start from "RaylibCmake"
	- cmake -S . -B build -G "MinGW Makefiles"
	- cd build
	- mingw32-make
	- ./game
	