#include "convolve.h"
#include <gtest/gtest.h>

/*
Tests the fft convolution method
Note: convolveFFT and timeConvolve has different results because convolveFFT does some scaling
*/

//tests positives
TEST(testFFTConvolve, testConvolution1)
{
    double x[4] = {0.5, 0.5, 1.0, 0.2};
    double h[3] = {1.0, 1.0, 1.0};
    double expectedy[6] = {0.25, 0.5, 1, 0.85, 0.6, 0.1};
    double y[6] = {0};

    convolveFFT(x, 4, h, 3, y, 6);

    for(int i = 0; i < 6; i++)
    {
        ASSERT_DOUBLE_EQ(y[i], expectedy[i]);
    }
}

//tests all negatives
TEST(testFFTConvolve, testConvolution2)
{
    double x[4] = {-0.5, -0.5, -1.0, -0.2};
    double h[3] = {-1.0, -1.0, -1.0};
    double expectedy[6] = {0.25, 0.5, 1, 0.85, 0.6, 0.1};
    double y[6] = {0};

    convolveFFT(x, 4, h, 3, y, 6);

    for(int i = 0; i < 6; i++)
    {
        ASSERT_DOUBLE_EQ(y[i], expectedy[i]);
    }
}

//tests mix of positives and negatives
TEST(testFFTConvolve, testConvolution3)
{
    double x[4] = {-0.5, 0.5, -1.0, 0.2};
    double h[3] = {-1.0, 1.0, -1.0};
    double expectedy[6] = {0.25, -0.5, 1, -0.85, 0.6, -0.1};
    double y[6] = {0};

    convolveFFT(x, 4, h, 3, y, 6);

    for(int i = 0; i < 6; i++)
    {
        ASSERT_DOUBLE_EQ(y[i], expectedy[i]);
    }
}