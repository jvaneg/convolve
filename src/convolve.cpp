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

    dryFile.open(dryFileName.c_str(), ios::binary | ios::in);
    if(dryFile.fail())
    {
        cout << "Failed to open dry recording file!" << endl;
        exit(-1);
    }

    irFile.open(irFileName.c_str(), ios::binary | ios::in);
    if(irFile.fail())
    {
        cout << "Failed to open impulse response file!" << endl;
        exit(-1);
    }

    if(fileExists(outFileName))
    {
        cout << "Error: Output file already exists!" << endl;
        exit(-1);
    }
    
    //maybe try opening later
    outFile.open(outFileName.c_str(), ios::binary | ios::out);
    if(outFile.fail())
    {
        cout << "Failed to open output file!" << endl;
        exit(-1);
    }

    cout << dryFileName << " " << irFileName << " " << outFileName << endl; //debug



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