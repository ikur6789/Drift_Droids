HOW TO INSTALL DRIFT DROIDS
Windows:
1. Make sure that all dependencies are satisfied - first, miniAT should be in your C drive.
a. miniAT.org has all of the necessary instructions for installation, in the “decode/execute” sections!
b. Specifically, you can go to www.miniat.org/getting-started/build#windows
c. This will also advise you on the necessary python and scons downloads.
2. You will also need SLD 2.0, SDL_image, and the SDL mixer installed properly, unless you intend to build from source with linked assets.
3. Get Sourcetree for windows, then set it up properly and download the source and assets for the project from https://www.bitbucket.org/SUNYPoly_driftdroids1/drift_droids-public.git 
4. Copy the contents of the repo into your miniAT folder (should be in C/miniat/system if properly installed)
5. Locate the SConscript file, open it using a text editor, and make sure line 7 includes the mingw32 library. 
6. Open up the windows command line, change directory into C/miniat. Next, run scons with no arguments. Both miniat and drift droids should be built after running scons.
7. Take the executable (.exe) out of C/miniat/out/exe and copy it into your drift droids folder located in C/miniat/system

Unix / Linux:
1. Make sure that all dependencies are satisfied - first, miniat should be installed according to the instructions on https://www.miniat.org/getting-started/build#ubuntu (if you’re using a different distribution, use a bit of experience and creativity to get it running)
2. Using your package manager, download and install SDL2.0, SDL_image, and the SDL mixer, along with their dev tools/versions if building from source. Also make sure that you have scons and the desired version of python (see miniat website) installed.
3. Have git installed and working.
4. Clone the repo into your miniAT folder (https://www.bitbucket.org/SUNYPoly_driftdroids1/drift_droids-public.git)
5. Locate the SConscript file, open it using a text editor, and make sure line 7 does NOT include the mingw32 library. 
6. Similar to windows, run scons in the correct directory and move the executable from out/exe into the system subfolder.

Drift Droids Command Line Options to Run Game
[friction map] [display map] [Car1 Image] [Music] [AT Robot 1] [Car2 Image] [AT Robot 2]

Missing any of these files may result in the game not launching. Please refer to the game’s documentation to make sure you map and car images are the correct size, otherwise the game will not start.
Happy Drifting!
