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
#define NORMAL_FFT  1
#define INVERSE_FFT -1

/*
Name: fileExists
Purpose: check if a file exists
Input:  
    fileName  -   the name of the file being checked, type string, by reference

Returns: true if file exists, else false
*/
bool fileExists(const std::string& fileName);

/*
Name: timeConvolveWav
Purpose: Time convolution algorithm for 2 Wave files
Details: Very slow - O(n+m) ~= O(n^2)
Input:  
    dryWav  -   dry recording, type WaveFile, by reference
    irWav   -   impulse response recording, type WaveFile, by reference
Input/Output:
    outWav  -   the resulting convolved signal, type WaveFile, by reference
*/
void convolveWav(WaveFile& dryWav, WaveFile& irWav, WaveFile& outWav);

/*
Name: normalizeSignal
Purpose: normalizes an array of doubles to fall withing the range [-1.0,1.0]
Input:  
    signal  -   pointer to the array, type signal*
    signalSize  -   the number of elements in the signal array, type uint32_t
*/
void normalizeSignal(double* signal, uint32_t signalSize);

/*
Name: timeConvolve
Purpose: Convolves two signals, producing an output signal.
         The convolution is done in the time domain using the
         "Input Side Algorithm" (see Smith, p. 112-115).
Details: Very slow - O(n+m) ~= O(n^2)
         outputSignal[] must be of size (drySampleCount + impulseSampleCount - 1), it is assumed it is
Input:
    drySignal[] -   the signal to be convolved
    drySampleCount  -   size of dry signal array
    impulseSignal[] -   the impulse response
    impulseSampleCount  -   size of impulse signal array
    outputSignal[]  -   the output signal, the result of the convolution
*/
void timeConvolve(double drySignal[], uint32_t drySampleCount, double impulseSignal[], uint32_t impulseSampleCount, double outputSignal[]);

void fastFourierTransform(double data[], int n, int isign);

void convolveFFT(double drySignal[], uint32_t drySampleCount, double impulseSignal[], uint32_t impulseSampleCount, double outputSignal[], uint32_t outputSampleCount);

void complexMultiply(double dryFreqSignal[], double irFreqSignal[], double outFreqSignal[], uint32_t complexFreqSignalSize);

void fftPrep(double freqSignal[], uint32_t freqSize, double timeSignal[], uint32_t timeSize);

void scale(double X[], uint32_t N);


#endif