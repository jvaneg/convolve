#include "waveFile.h"

using namespace std;

WaveFile::WaveFile()
{
    //todo
}

WaveFile::~WaveFile()
{
    //todo
}

bool WaveFile::openFile(string fileName)
{
    ifstream inFile;

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