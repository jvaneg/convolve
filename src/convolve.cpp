

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
    timeConvolveWav(dryWav, irWav, outWav);
    cout << "...convolve complete!" << endl;

    //debug
    cout << "-----outWav-----" << endl;
    outWav.printWaveHeader();
    outWav.printDataChunkHeader();


    cout << "writing to file" << endl;
    outWav.writeFile(outFileName);
    cout << "write successful!" << endl;


}


/*
Name: convolveWav
Purpose: convolution algorithm for 2 Wave files
Input:  
    dryWav  -   dry recording, type WaveFile, by reference
    irWav   -   impulse response recording, type WaveFile, by reference
Input/Output:
    outWav  -   the resulting convolved signal, type WaveFile, by reference
*/
void timeConvolveWav(WaveFile& dryWav, WaveFile& irWav, WaveFile& outWav)
{
    double* drySignal = dryWav.getSampleData();
    uint32_t drySampleCount = dryWav.getSampleCount();
    double* impulseSignal = irWav.getSampleData();
    uint32_t impulseSampleCount = irWav.getSampleCount();
    double* outputSignal = NULL;
    uint32_t outputSampleCount = 0;
    uint32_t outputSignalSize = 0;

    outputSampleCount = drySampleCount + impulseSampleCount - 1;
    outputSignal = new double[outputSampleCount](); //initializes array to 0
    outputSignalSize = (dryWav.getBitsPerSample()/8) * outputSampleCount;

    timeConvolve(drySignal, drySampleCount, impulseSignal, impulseSampleCount, outputSignal);

    normalizeSignal(outputSignal, outputSampleCount);

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


/*
Name: normalizeSignal
Purpose: normalizes an array of doubles to fall withing the range [-1.0,1.0]
Input:  
    signal  -   pointer to the array, type signal*
    signalSize  -   the number of elements in the signal array, type uint32_t
*/
void normalizeSignal(double* signal, uint32_t signalSize)
{
    double largest = 0;

    for(uint32_t i = 0; i < signalSize; i++)
    {
        if(abs(signal[i]) > largest)
        {
            largest = abs(signal[i]);
        }
    }

    for(uint32_t j = 0; j < signalSize; j++)
    {
        signal[j] /= largest;
    }
}


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
void timeConvolve(double drySignal[], uint32_t drySampleCount, double impulseSignal[], uint32_t impulseSampleCount, double outputSignal[])
{
    uint32_t i;
    uint32_t j;

    for(i = 0; i < drySampleCount; i++)
    {
        for(j = 0; j < impulseSampleCount; j++)
        {
            outputSignal[i+j] += drySignal[i] * impulseSignal[j];
        }
    }
}

/*
    The four1 FFT from Numerical Recipes in C,
    p. 507 - 508.
    Note:  changed float data types to double.
    nn must be a power of 2, and use +1 for
    isign for an FFT, and -1 for the Inverse FFT.
    The data is complex, so the array size must be
    n*2. This code assumes the array starts
    at index 1, not 0, so subtract 1 when
    calling the routine (see main() below).

Name: fastFourierTransform
Purpose:

Details: I have no idea what this code is doing I copied it from a book.
         This code assumes the array starts at index 1, not 0, so subtract 1 when
         calling the routine (wtf???)

Input:
    data[]  -   the signal to be transformed, size must be n*2
    n  -    the size of the data array (kindof, data is twice as big because it is complex),
            must be a power of two
    isign   -   ??? +1 for an FFT, -1 for an Inverse FFT

*/
void fastFourierTransform(double data[], int n, int isign) //TODO: change this so it isnt 1 indexed (is this worth the headache?)
{
	uint64_t nn, mmax, m, j, istep, i;
	double wtemp, wr, wpr, wpi, wi, theta;
	double tempr, tempi;

	nn = n << 1;
	j = 1;

    for (i = 1; i < nn; i += 2)     //The bit reversal section of the routine
    {
        if (j > i) 
        {
			SWAP(data[j], data[i]);             //Exchange two complex numbers
			SWAP(data[j + 1], data[i + 1]);     //
		}
		m = n;
        while (m >= 2 && j > m) 
        {
			j -= m;
			m >>= 1;
		}
		j += m;
	}

    //Danielson-Lanczos section of the routine
	mmax = 2;
    while (nn > mmax)   //Outer loop executed log2(n) times
    {
		istep = mmax << 1;
		theta = isign * (6.28318530717959 / mmax);      //initialize the trigonometric recurrence
		wtemp = sin(0.5 * theta);
		wpr = -2.0 * wtemp * wtemp;
		wpi = sin(theta);
		wr = 1.0;
		wi = 0.0;
        for (m = 1; m < mmax; m += 2)   //the two nested inner loops
        {
            for (i = m; i <= nn; i += istep) 
            {
                //The Danielson-Lanczos formula
				j = i + mmax;
				tempr = wr * data[j] - wi * data[j + 1];
				tempi = wr * data[j + 1] + wi * data[j];
				data[j] = data[i] - tempr;
				data[j + 1] = data[i + 1] - tempi;
				data[i] += tempr;
				data[i + 1] += tempi;
			}
			wr = (wtemp = wr) * wpr - wi * wpi + wr;    //Trigonometric recurrence
			wi = wi * wpr + wtemp * wpi + wi;
		}
		mmax = istep;
	}
}