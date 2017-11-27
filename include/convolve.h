#ifndef CONVOLVE_H
#define CONVOLVE_H

#include <iostream>
#include <fstream>
#include <stdlib.h>	
#include <string>
#include <sys/stat.h>	//stat
#include <stdint.h>      //fixed int sizes
#include <cmath>        //abs

#include "waveFile.h"

#define SWAP(a,b)  tempr=(a);(a)=(b);(b)=tempr

/*
Name: fileExists
Purpose: check if a file exists
Input:  
    fileName  -   the name of the file being checked, type string, by reference

Returns: true if file exists, else false
*/
bool fileExists(const std::string& fileName);

/*
Name: convolveWav
Purpose: Time convolution algorithm for 2 Wave files
Details: Very slow - O(n+m) ~= O(n^2)
Input:  
    dryWav  -   dry recording, type WaveFile, by reference
    irWav   -   impulse response recording, type WaveFile, by reference
Input/Output:
    outWav  -   the resulting convolved signal, type WaveFile, by reference
*/
void timeConvolveWav(WaveFile& dryWav, WaveFile& irWav, WaveFile& outWav);

void normalizeSignal(double* signal, uint32_t signalSize);

void timeConvolve(double drySignal[], uint32_t drySampleCount, double impulseSignal[], uint32_t impulseSampleCount, double outputSignal[]);

void fastFourierTransform(double data[], int n, int isign);


#endif