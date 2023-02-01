SJF.cpp and SRTF.cpp must be compiled using g++ 

SJF only needs input file, while SRTF needs input file and time slice value as well

Output is displayed on the screen, but can be redirected. 

COMMANDS TO BE RUN
*********************

g++ SJF.cpp -o sjf
g++ SRTF.cpp -o srtf

sjf.exe <input file> > output.txt
srtf.ext <input file> <time slice value(should be integer)> > output.txt