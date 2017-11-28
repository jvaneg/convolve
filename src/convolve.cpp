

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

    cout << "Loading " << dryFileName << "..." << endl;
    if(dryWav.loadFile(dryFileName))
    {
        cout << "...Loading complete!" << endl;
    }
    else
    {
        cout << "Error occurred loading " << dryFileName << "!" << endl;
        exit(-1);
    }
    
    cout << "Loading " << irFileName << "..." << endl;
    if(irWav.loadFile(irFileName))
    {
        cout << "...Loading complete!" << endl;
    }
    else
    {
        cout << "Error occurred loading " << irFileName << "!" << endl;
        exit(-1);
    }


    cout << "Convolving..." << endl;
    convolveWav(dryWav, irWav, outWav);
    cout << "...Convolution complete!" << endl;


    cout << "Writing result to " << outFileName << "..." << endl;
    if(outWav.writeFile(outFileName))
    {
        cout << "...Write successful!" << endl;
    }
    else
    {
        cout << "Error occurred writing result to " << irFileName << "!" << endl;
        exit(-1);
    }
    
    return 0;
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
void convolveWav(WaveFile& dryWav, WaveFile& irWav, WaveFile& outWav)
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

    //timeConvolve(drySignal, drySampleCount, impulseSignal, impulseSampleCount, outputSignal);
    convolveFFT(drySignal, drySampleCount, impulseSignal, impulseSampleCount, outputSignal, outputSampleCount);

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
Purpose: Converts a time domain signal into a frequency domain signal

Details: O(nlog(n)) so nice speed compared to time convolution
         I have no idea what this code is doing I copied it from a book.
         This code assumes the array starts at index 1, not 0, so subtract 1 when
         calling the routine (wtf???).
         Assumes time domain dignal has been padded to complex data form (real, imaginary) pairs

Input:
    data[]  -   the signal to be transformed, size must be n*2, in complex data form
    n  -    the size of the data array (kindof, data is twice as big because it is complex),
            must be a power of two
    isign   -   ??? +1 for an FFT, -1 for an Inverse FFT

*/
void fastFourierTransform(double data[], int n, int isign)
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
		theta = isign * (6.28318530717959 / (double) mmax);      //initialize the trigonometric recurrence //maybe casting a double is bad!!!!!!!!!!!!!
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


void convolveFFT(double drySignal[], uint32_t drySampleCount, double impulseSignal[], uint32_t impulseSampleCount, double outputSignal[], uint32_t outputSampleCount) //TODO in progress
{
    uint32_t freqSignalSize;
    uint32_t complexFreqSignalSize;
	double* dryFreqSignal;
	double* irFreqSignal;
    double* outFreqSignal;
    
    //ensures freq signal arrays are powers of 2 for the FFT
	freqSignalSize = 1;
    while (freqSignalSize < outputSampleCount) 
    {
		freqSignalSize *= 2;
    }
    
    complexFreqSignalSize = freqSignalSize*2; //because complex data comes in pairs (real, imaginary)
	
	dryFreqSignal = new double[complexFreqSignalSize]();  //initialized to zero
	irFreqSignal = new double[complexFreqSignalSize]();   //initialized to zero
    outFreqSignal  = new double[complexFreqSignalSize](); //initialized to zero
    
    fftPrep(dryFreqSignal, freqSignalSize, drySignal, drySampleCount);
    fftPrep(irFreqSignal, freqSignalSize, impulseSignal, impulseSampleCount);

	fastFourierTransform((dryFreqSignal - 1), freqSignalSize, NORMAL_FFT);
    fastFourierTransform((irFreqSignal - 1), freqSignalSize, NORMAL_FFT);

    complexMultiply(dryFreqSignal, irFreqSignal, outFreqSignal, complexFreqSignalSize);

    fastFourierTransform((outFreqSignal - 1), freqSignalSize, INVERSE_FFT);

    scaleFFT(outFreqSignal, complexFreqSignalSize);

    for (uint32_t i = 0; i < outputSampleCount; i++)
    { 
		outputSignal[i] = outFreqSignal[i*2];
    }

    delete[] dryFreqSignal;
    delete[] irFreqSignal;
    delete[] outFreqSignal;

	return;
}

/*
Name: complexMultiply
Purpose: Performs the frequency domain complex multiplication (occurs on real and imaginary sections of array)
         array structure [real i, imaginary i, real i+1, imaginary i+1, ..., real n, imaginary n]
Details: Assumes complexFreqSignalSize is a multiple of 2.
         Assumes all arrays are the same size.
Input:
    dryFreqSignal[] -   frequency domain dry input signal
    irFreqSignal[]  -   frequency domain impulse response signal (aka filter kernel)
    outFreqSignal[] -   frequency domain result signal
    complexFreqSignalSize   -   size of the arrays
*/
void complexMultiply(double dryFreqSignal[], double irFreqSignal[], double outFreqSignal[], uint32_t complexFreqSignalSize) 
{
    for (uint32_t  i = 0; i < complexFreqSignalSize; i += 2)  //complex multiplication
    { 
		outFreqSignal[i] = (dryFreqSignal[i] * irFreqSignal[i]) - (dryFreqSignal[i + 1] * irFreqSignal[i + 1]);         //RE_OUT[i] = RE_X[i]*RE_FR[i] - IM_X[i]*IM_FR[i]
		outFreqSignal[i + 1 ] = (dryFreqSignal[i] * irFreqSignal[i + 1]) + (dryFreqSignal[i + 1] * irFreqSignal[i]);    //IM_OUT[i] = RE_X[i]*IM_FR[i] + IM_X[i]*RE_FR[i]
	}
}

/*
Name: fftPrep
Purpose: prepares frequency signal array for fft by converting it to a complex number format
Details: Assumes input array freqSignal[] is zeroed

Input:
    freqSignal[]    -   array containing the frequency signal
    freqSize    -   size of the freqSignal array, assumed to be zeroed
    timeSignal[]    -   array containing the time signal
    timeSize    -   size of the timeSignal array
*/
void fftPrep(double freqSignal[], uint32_t freqSize, double timeSignal[], uint32_t timeSize) 
{
    uint32_t index;
    uint32_t complexIndex;
    for (index = 0, complexIndex = 0; index < timeSize; index++, complexIndex += 2) 
    {
		freqSignal[complexIndex] = timeSignal[index];
	}
}

/*
Name: scaleFFT
Purpose: not really sure what this does actually, I think its like normalizing

Input:
    signal[]    -   array containing the frequency signal
    signalSize    -   size of the signal array
*/
void scaleFFT(double signal[], uint32_t signalSize) 
{    
    for (uint32_t i = 0; i < signalSize; i++) 
    {
		signal[i] /= (double) signalSize;
    }
}