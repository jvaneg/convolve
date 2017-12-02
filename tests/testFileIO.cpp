#include "convolve.h"
#include <gtest/gtest.h>

/*
Tests the read and write methods
*/

//tests reading the file
TEST(testFileIO, testRead)
{
    string coolname = "coolTestRead.wav";

    WaveFile coolIn;

    double* coolSignalExpected = new double[5]();
    double* coolSignalIn;
    coolSignalExpected[0] = -1.0;
    coolSignalExpected[1] = -0.5;
    coolSignalExpected[2] = 0;
    coolSignalExpected[3] = 0.5;
    coolSignalExpected[4] = 1.0;

    coolIn.loadFile(coolname);

    coolSignalIn = coolOut.getSampleData();

    for(int i = 0; i < 5; i++)
    {
        ASSERT_DOUBLE_EQ(coolSignalExpected[i], coolSignalIn[i]);
    }
}

//tests writing the file, relies on reading to work
TEST(testFFTConvolve, testWrite)
{
    string coolname = "coolTestWrite.wav";

    WaveFile coolOut;

    double* coolSignalOut = new double[5]();
    double* coolSignalIn;
    coolSignalOut[0] = -1.0;
    coolSignalOut[1] = -0.5;
    coolSignalOut[2] = 0;
    coolSignalOut[3] = 0.5;
    coolSignalOut[4] = 1.0;

    coolOut.generate(1, 1, 44100, 16, 10, coolSignalOut);

    coolOut.writeFile(coolname);

    coolOut.loadFile(coolname);

    coolSignalIn = coolOut.getSampleData();

    for(int i = 0; i < 5; i++)
    {
        ASSERT_DOUBLE_EQ(coolSignalOut[i], coolSignalIn[i]);
    }
}