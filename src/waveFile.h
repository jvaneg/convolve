#ifndef WAVEFILE_H
#define WAVEFILE_H

#include <stdint.h>     //fixed int sizes
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

typedef struct
{
    uint8_t chunkID[4];         // 4 bytes - "RIFF"
    uint32_t chunkSize;         // 4 bytes - Size of remaining file in bytes (36 + data chunk size)
    uint8_t format[4];          // 4 bytes - "WAVE"
    uint8_t fmtChunkID[4];      // 4 bytes - "fmt "
    uint32_t fmtChunkSize;      // 4 bytes - Size of the fmt sub chunk (should be 16, may be slightly more)
    uint16_t audioFormat;       // 2 bytes - 1 = PCM
    uint16_t numChannels;       // 2 bytes - 1 = mono, 2 = stereo, etc          
    uint32_t sampleRate;        // 4 bytes - Samples per second (eg 44100 Hz)    
    uint32_t byteRate;          // 4 bytes - Avg number of bytes per second (sampleRate * numChannels * bitsPerSample/8)  
    uint16_t blockAlign;        // 2 bytes - Frame size in bytes (numChannels * bitsPerSample/8)
    uint16_t bitsPerSample;     // 2 bytes - Sample size in bits (eg 16)
} WaveHeader;

typedef struct
{
    uint8_t dataChunkID[4];     // 4 bytes - "data"
    uint32_t dataChunkSize;     // 4 bytes - Size of sample data in bytes
} DataChunkHeader;


class WaveFile
{
public:
    WaveFile();
    ~WaveFile();
    bool openFile(std::string fileName);
    bool writeFile(std::string fileName);
    void generate(uint16_t audioFormat, uint16_t numChannels, uint32_t sampleRate, uint16_t bitsPerSample, uint32_t dataChunkSize, double* sampleData);
    void printWaveHeader();
    void printDataChunkHeader();

    uint16_t getAudioFormat();
    uint16_t getNumChannels();
    uint32_t getSampleRate();
    uint16_t getBitsPerSample();
    uint32_t getDataChunkSize();
    double* getSampleData(); //be careful with me i can cause segfaults! im only here to avoid massive memory usage!
    uint32_t getSampleCount();


private:
    std::string getHeaderChunkID();
    std::string getHeaderFormat();
    std::string getFmtChunkID();
    std::string getDataChunkID();

    WaveHeader waveHeader;
    DataChunkHeader dataHeader;
    double* sampleData = NULL;          //will become an array
    uint32_t sampleCount;


    
    


};



#endif