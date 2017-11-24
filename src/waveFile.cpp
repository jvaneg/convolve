#include "waveFile.h"

using namespace std;

/*----------Public Functions----------*/

/*
Name: WaveFile
Class: WaveFile
Purpose: Default constructor for the WaveFile class
*/
WaveFile::WaveFile()
{
    //todo
}

/*
Name: ~WaveFile
Class: WaveFile
Purpose: Default destructor for the WaveFile class
*/
WaveFile::~WaveFile()
{
    delete[] sampleData;
    sampleData = NULL;
}

/*
Name: generate
Class: WaveFile
Purpose: Populates a WaveFile header and body
Input:  
    audioFormat -   compression type of audio format, type uint16_t
    numChannels -   number of channels in the wav, type uint16_t
    sampleRate  -   sample rate in Hz of wav, type uint32_t
    bitsPerSample   -   number of bits in each sample, type uint16_t
    dataChunkSize   -   size in bytes of the data chunk, type uint32_t
    sampleData  -   pointer to the array of sample data, type double*
*/
void WaveFile::generate(uint16_t audioFormat, uint16_t numChannels, uint32_t sampleRate, uint16_t bitsPerSample, uint32_t dataChunkSize, double* sampleData)
{
    waveHeader.chunkID[0] = 'R';
    waveHeader.chunkID[1] = 'I';
    waveHeader.chunkID[2] = 'F';
    waveHeader.chunkID[3] = 'F';

    waveHeader.chunkSize = (uint32_t) sizeof(WaveHeader) + dataChunkSize;

    waveHeader.format[0] = 'W';
    waveHeader.format[1] = 'A';
    waveHeader.format[2] = 'V';
    waveHeader.format[3] = 'E';

    waveHeader.fmtChunkID[0] = 'f';
    waveHeader.fmtChunkID[1] = 'm';
    waveHeader.fmtChunkID[2] = 't';
    waveHeader.fmtChunkID[3] = ' ';

    waveHeader.fmtChunkSize = 16;
    waveHeader.audioFormat = audioFormat;
    waveHeader.numChannels = numChannels;
    waveHeader.sampleRate = sampleRate;
    waveHeader.byteRate = sampleRate*numChannels*bitsPerSample/8;
    waveHeader.blockAlign = (uint16_t)(numChannels*bitsPerSample/8);
    waveHeader.bitsPerSample = bitsPerSample;

    dataHeader.dataChunkID[0] = 'd';
    dataHeader.dataChunkID[1] = 'a';
    dataHeader.dataChunkID[2] = 't';
    dataHeader.dataChunkID[3] = 'a';

    dataHeader.dataChunkSize = dataChunkSize;
    if(this->sampleData != NULL)
    {
        delete this->sampleData;
        this->sampleData = NULL;
    }
    this->sampleData = sampleData;

    sampleCount = dataHeader.dataChunkSize/(waveHeader.bitsPerSample/8);

    return;
}

/*
Name: loadFile
Class: WaveFile
Purpose: loads WaveFile data from a .wav file
Details: does not support multichannel wavs, wavs other than 16-bit sample size
Input:  
    fileName    -   the name of the file to load, type string

Returns: true if succeeded, false if failed
*/
bool WaveFile::loadFile(string fileName)
{
    ifstream inFile;
    uint16_t sample16Bit;

    inFile.open(fileName.c_str(), ios::binary | ios::in);
    if(inFile.fail())
    {
        cout << "Failed to open wav recording file!" << endl;
        return false;
    }

    inFile.read((char*) &waveHeader, sizeof(WaveHeader));
    if(inFile.fail())
    {
        cout << "Error: Could not read wav file header!" << endl;
        return false;
    }

    if((getHeaderChunkID() != "RIFF") || (getHeaderFormat() != "WAVE") || (getFmtChunkID() != "fmt ") ) //TODO constant
    {
        cout << "Error: Invalid wav header format!" << endl;
        return false;
    }

    //skip any extra bytes at the end of the fmt chunk
    inFile.seekg(waveHeader.fmtChunkSize - (sizeof(WaveHeader) - 20), inFile.cur); //TODO constant -> 20 is the amount of bytes up to and including fmtChunkSize

    inFile.read((char*) &dataHeader, sizeof(DataChunkHeader));
    if(inFile.fail())
    {
        cout << "Error: Could not read wav data chunk header!" << endl;
        return false;
    }
    

    if((getDataChunkID() != "data")) //TODO constant
    {
        cout << "Error: Invalid data chunk header format!" << endl;
        return false;
    }

    if(waveHeader.bitsPerSample == 16) //TODO constant
    {
        sampleCount = dataHeader.dataChunkSize/(waveHeader.bitsPerSample/8);
    }
    else
    {
        cout << "Error: Sample sizes other than 16-bit are not currently supported by this program!" << endl;
        return false;
    }

    sampleData = new double[sampleCount];
    for(uint32_t i = 0; i < sampleCount; i++)
    {
        inFile.read((char*) &sample16Bit, sizeof(uint16_t));
        sampleData[i] = (double) (sample16Bit/(pow(2,waveHeader.bitsPerSample-1)-1)); //TODO replace this formula with a constant
    }

    if(inFile.fail())
    {
        cout << "Error: Failed to read data from data chunk!" << endl;
        return false;
    }

    inFile.close();

    return true;
}

/*
Name: writeFile
Class: WaveFile
Purpose: writes WaveFile data to a .wav file
Details: does not support multichannel wavs, wavs other than 16-bit sample size
Input:  
    fileName    -   the name of the file to write to, type string

Returns: true if succeeded, false if failed
*/
bool WaveFile::writeFile(string fileName)
{
    ofstream outFile;
    uint8_t emptyByte = 0;
    uint16_t sample16Bit;

    outFile.open(fileName.c_str(), ios::binary | ios::out);
    if(outFile.fail())
    {
        cout << "Failed to open output file!" << endl;
        return false;
    }

    outFile.write((char*) &waveHeader, sizeof(WaveHeader));

    if(waveHeader.fmtChunkSize > 16)
    {
        for(uint8_t i = 0; i < (waveHeader.fmtChunkSize - (sizeof(WaveHeader) - 20)); i++)
        {
            outFile.write((char*) &emptyByte, sizeof(uint8_t));
        }
    }

    outFile.write((char*) &dataHeader, sizeof(DataChunkHeader));
    if(sampleData != NULL)
    {
        if(waveHeader.bitsPerSample == 16)
        {
            for(uint32_t i = 0; i < sampleCount; i++)
            {
                sample16Bit = (uint16_t) (sampleData[i] * (pow(2,waveHeader.bitsPerSample-1)-1)); //TODO replace this formula with constant
                outFile.write((char*) &sample16Bit, sizeof(uint16_t));
            }
        }
        else
        {
            cout << "Error: this program currently only supports 16-bit wavs!" << endl;
            return false;
        }
    }

    return true;
}


/*
Name: printWaveHeader
Class: WaveFile
Purpose: prints out the wave header information, mainly for debug
*/
void WaveFile::printWaveHeader()
{
    cout << "ChunkID : " << getHeaderChunkID() << endl
    << "ChunkSize : " << waveHeader.chunkSize << endl
    << "Format : " << getHeaderFormat() << endl
    << "Subchunk1ID : " << getFmtChunkID() << endl
    << "Subchunk1Size: " << waveHeader.fmtChunkSize << endl
    << "AudioFormat: " << waveHeader.audioFormat << endl
    << "NumChannels: " << waveHeader.numChannels << endl
    << "SampleRate: " << waveHeader.sampleRate << endl
    << "ByteRate: " << waveHeader.byteRate << endl
    << "BlockAlign: " << waveHeader.blockAlign << endl
    << "BitsPerSample: " << waveHeader.bitsPerSample << endl;
}

/*
Name: printDataChunkHeader
Class: WaveFile
Purpose: prints out the data chunk header information, mainly for debug
*/
void WaveFile::printDataChunkHeader()
{
    cout << "Subchunk2ID : " << getDataChunkID() << endl
    << "Subchunk2Size : " << dataHeader.dataChunkSize << endl;
}

/*
Name: getAudioFormat
Class: WaveFile
Purpose: returns the audio format
Returns: the audio format, type uint16_t
*/
uint16_t WaveFile::getAudioFormat()
{
    return waveHeader.audioFormat;
}

/*
Name: getNumChannels
Class: WaveFile
Purpose: returns the number of channels
Returns: the number of channels, type uint16_t
*/
uint16_t WaveFile::getNumChannels()
{
    return waveHeader.numChannels;
}

/*
Name: getSampleRate
Class: WaveFile
Purpose: returns the sample rate in Hz
Returns: the sample rate, type uint32_t
*/
uint32_t WaveFile::getSampleRate()
{
    return waveHeader.sampleRate;
}

/*
Name: getNumChannels
Class: WaveFile
Purpose: returns the number of bits per sample
Returns: the number of bits per sample, type uint16_t
*/
uint16_t WaveFile::getBitsPerSample()
{
    return waveHeader.bitsPerSample;
}

/*
Name: getDataChunkSize
Class: WaveFile
Purpose: returns the data chunk in bytes
Returns: the size of the data chunk in bytes, type uint32_t
*/
uint32_t WaveFile::getDataChunkSize()
{
    return dataHeader.dataChunkSize;
}

/*
Name: getSampleData
Class: WaveFile
Purpose: returns the a pointer to the array of samples
Warning!!!!!: Be careful with me, the contents of this pointer get deleted in the constructor, so if youre not careful with this
              it can cause a segfault!
              This is bad practice but the size of the data is really big and I don't want to duplicate it
Returns: pointer to the array of samples, type double*
*/
double* WaveFile::getSampleData()
{
    return sampleData;
}

/*
Name: getSampleCount
Class: WaveFile
Purpose: returns the number of samples in the file
Returns: the number of samples, type uint32_t
*/
uint32_t WaveFile::getSampleCount()
{
    return sampleCount;
}


/*----------Private Functions----------*/


/*
Name: getHeaderChunkID
Class: WaveFile
Purpose: returns the headerID of the wave header chunk
Details: should be "RIFF"
Returns: the wave header ID, type string
*/
string WaveFile::getHeaderChunkID()
{
    string chunkID((char*) waveHeader.chunkID, sizeof(waveHeader.chunkID));

    return chunkID;
}

/*
Name: getHeaderFormat
Class: WaveFile
Purpose: returns the format of the wave header
Details: should be "WAVE"
Returns: the wave header format, type string
*/
string WaveFile::getHeaderFormat()
{
    string format((char*) waveHeader.format, sizeof(waveHeader.format));

    return format;
}

/*
Name: getfmtChunkID
Class: WaveFile
Purpose: returns the fmt chunk ID of the wave header
Details: should be "fmt "
Returns: the wave header fmt chunk ID, type string
*/
string WaveFile::getFmtChunkID()
{
    string fmtChunkID((char*) waveHeader.fmtChunkID, sizeof(waveHeader.fmtChunkID));

    return fmtChunkID;
}

/*
Name: getDataChunkID
Class: WaveFile
Purpose: returns the data chunk ID of the data chunk header
Details: should be "data"
Returns: the data chunk header's ID, type string
*/
string WaveFile::getDataChunkID()
{
    string dataChunkID((char*) dataHeader.dataChunkID, sizeof(dataHeader.dataChunkID));

    return dataChunkID;
}