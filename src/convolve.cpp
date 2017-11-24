

#include "convolve.h"

using namespace std;


int main(int argc, char *argv[])
{
    string dryFileName;
    string irFileName;
    string outFileName;

    WaveFile dryWav;
    WaveFile irWav;
    WaveFile outWav;

    if(argc != 4)
	{
		cout << "Input format: convolve [dry recording file] [impulse response file] [desired output file name]" << endl;
		exit(-1);
    }
    else
    {
        dryFileName = argv[1];
        irFileName = argv[2];
        outFileName = argv[3];
    }

    if(!fileExists(dryFileName))
    {
        cout << "Error: " << dryFileName << " does not exist!" << endl;
        exit(-1);
    }

    if(!fileExists(irFileName))
    {
        cout << "Error: " << irFileName << " does not exist!" << endl;
        exit(-1);
    }

    if(fileExists(outFileName))
    {
        cout << "Error: " << outFileName << " already exists!" << endl;
        exit(-1);
    }

    dryWav.loadFile(dryFileName);

    //debug
    cout << "-----dryWav-----" << endl;
    dryWav.printWaveHeader();
    dryWav.printDataChunkHeader();
    

    irWav.loadFile(irFileName);

    //debug
    cout << "-----irWav-----" << endl;
    irWav.printWaveHeader();
    irWav.printDataChunkHeader();


    cout << "convolving..." << endl;
    convolveWav(dryWav, irWav, outWav);
    cout << "...convolve complete!" << endl;

    //debug
    cout << "-----outWav-----" << endl;
    irWav.printWaveHeader();
    irWav.printDataChunkHeader();


    cout << "writing to file" << endl;
    outWav.writeFile(outFileName);
    cout << "write successful!" << endl;


}


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
void convolveWav(WaveFile& dryWav, WaveFile& irWav, WaveFile& outWav)
{
    double* drySignal = dryWav.getSampleData();
    uint32_t drySampleCount = dryWav.getSampleCount();
    double* impulseSignal = irWav.getSampleData();
    uint32_t impulseSampleCount = irWav.getSampleCount();
    double* outputSignal = NULL;
    uint32_t outputSampleCount = 0;
    uint32_t outputSignalSize = 0;
    uint32_t i;
    uint32_t j;

    outputSampleCount = drySampleCount + impulseSampleCount - 1;
    outputSignal = new double[outputSampleCount](); //initializes array to 0
    outputSignalSize = (dryWav.getBitsPerSample()/8) * outputSampleCount; 

    for(i = 0; i < drySampleCount; i++)
    {
        for(j = 0; j < impulseSampleCount; j++)
        {
            outputSignal[i+j] += drySignal[i] * impulseSignal[j];
        }
    }
    
    outWav.generate(dryWav.getAudioFormat(), dryWav.getNumChannels(), dryWav.getSampleRate(), dryWav.getBitsPerSample(), outputSignalSize, outputSignal);

    return;
}




/*
Name: fileExists
Purpose: check if a file exists
Input:  
    fileName  -   the name of the file being checked, type string, by reference

Returns: true if file exists, else false
*/
bool fileExists(const std::string& fileName)
{
    struct stat buf;
    if (stat(fileName.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}