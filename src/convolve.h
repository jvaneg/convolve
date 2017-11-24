#ifndef CONVOLVE_H
#define CONVOLVE_H

#include <iostream>
#include <fstream>
#include <stdlib.h>	
#include <string>
#include <sys/stat.h>	//stat
#include <stdint.h>      //fixed int sizes

#include "waveFile.h"

bool fileExists(const std::string& fileName);

void convolveWav(WaveFile& dryWav, WaveFile& irWav, WaveFile& outWav);


#endif