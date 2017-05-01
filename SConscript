Import('*')
import os


INC = [ '#/vm/inc' ]
SOURCE = [ 'main.cpp', 'peripherals.cpp', 'ports.cpp']
LIBS = [ 'mingw32', 'SDL2main', 'SDL2', 'libminiat', 'SDL2_image', 'SDL2_mixer' ]

source_dir = os.getcwd().replace('\\', '/')

#os.system("g++.exe -std=c++11 -Wall -fexceptions -g -ISDL\include\SDL2 -ISDL_Image\include\SDL2 -Iminiat\out\vm\inc\miniat -c C:\Users\ikurz\OneDrive\Documents\drift_droids-repository\main.cpp -o obj\Debug\main.o")
buildProgram("drift_droids", SOURCE, CPPPATH=INC, CPPDEFINES=[ 'SOURCE_DIR=\\"%s\\"' % source_dir], LIBS=LIBS)
buildMash('drift.asm')
