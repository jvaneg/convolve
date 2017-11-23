#include "waveFile.h"

using namespace std;

WaveFile::WaveFile()
{
    //todo
    sampleCount = 0;
    fileOpen = false;
}

WaveFile::~WaveFile()
{
    delete[] sampleData;
    sampleData = NULL;
}

bool WaveFile::openFile(string fileName)
{
    ifstream inFile;

    wavFileName = fileName;
    fileOpen = true;

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

    sampleData = new uint16_t[sampleCount];
    for(uint32_t i = 0; i < sampleCount; i++)
    {
        inFile.read((char*) &sampleData[i], sizeof(uint16_t));
    }

    if(inFile.fail())
    {
        cout << "Error: Failed to read data from data chunk!" << endl;
        return false;
    }

    inFile.close();

    return true;
}





string WaveFile::getHeaderChunkID()
{
    string chunkID((char*) waveHeader.chunkID, sizeof(waveHeader.chunkID));

    return chunkID;
}

string WaveFile::getHeaderFormat()
{
    string format((char*) waveHeader.format, sizeof(waveHeader.format));

    return format;
}

string WaveFile::getFmtChunkID()
{
    string fmtChunkID((char*) waveHeader.fmtChunkID, sizeof(waveHeader.fmtChunkID));

    return fmtChunkID;
}

string WaveFile::getDataChunkID()
{
    string dataChunkID((char*) dataHeader.dataChunkID, sizeof(dataHeader.dataChunkID));

    return dataChunkID;
}

string WaveFile::getOpenFileName() //TODO change this to do exception handling or something
{
    if(fileOpen)
    {
        return wavFileName;
    }
    else
    {
        return "no file open"; //lol
    }
}

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

void WaveFile::printDataChunkHeader()
{
    cout << "Subchunk2ID : " << getDataChunkID() << endl
    << "Subchunk2Size : " << dataHeader.dataChunkSize << endl;
}