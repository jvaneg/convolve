#include "convolve.h"

using namespace std;


int main(int argc, char *argv[])
{
    string dryFileName;
    string irFileName;
    string outFileName;
    ifstream dryFile;
    ifstream irFile;
    ofstream outFile;

    WaveFile dryWav;
    WaveFile irWav;

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
    
    //maybe try opening later
    /*
    outFile.open(outFileName.c_str(), ios::binary | ios::out);
    if(outFile.fail())
    {
        cout << "Failed to open output file!" << endl;
        exit(-1);
    }
    */

    dryWav.openFile(dryFileName);

    cout << "dryWav" << endl;
    cout << "File: " << dryWav.getOpenFileName() << endl;
    dryWav.printWaveHeader();
    dryWav.printDataChunkHeader();

    irWav.openFile(irFileName);

    cout << "irWav" << endl;
    cout << "File: " << irWav.getOpenFileName() << endl;
    irWav.printWaveHeader();
    irWav.printDataChunkHeader();


}





/**
 * Name: fileExists
 * 
 * Checks if a file exists
 * 
 * @param[in] fileName : name of the file to check, type string
 * 
 * @return true if file exists, else false
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