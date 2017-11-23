#ifndef WAVEFILE_H
#define WAVEFILE_H

#include <stdint.h>     //fixed int sizes
#include <string>
#include <iostream>
#include <fstream>

typedef struct
{
    uint8_t chunkID[4];         // 4 bytes - "RIFF"
    uint32_t chunkSize;         // 4 bytes - Size of remaining file in bytes (36 + data chunk size)
    uint8_t format[4];          // 4 bytes - "WAVE"
    uint8_t fmtChunkID[4];      // 4 bytes - "fmt "
    uint32_t fmtChunkSize;      // 4 bytes - should be 16
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

    std::string getHeaderChunkID();
    std::string getHeaderFormat();
    std::string getFmtChunkID();
    std::string getDataChunkID();
    void printWaveHeader();
    void printDataChunkHeader();

private:
    WaveHeader waveHeader;
    DataChunkHeader dataHeader;
    float* sampleData;          //will become an array


    
    


};



#endif