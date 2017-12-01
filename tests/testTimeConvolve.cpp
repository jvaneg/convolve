#include "convolve.h"
#include <gtest/gtest.h>

/*
Tests the time convolution method
Note: convolveFFT and timeConvolve has different results because convolveFFT does some scaling
*/

//tests positives
TEST(testTimeConvolve, testConvolution1)
{
    double x[4] = {0.5, 0.5, 1.0, 0.2};
    double h[3] = {1.0, 1.0, 1.0};
    double expectedy[6] = {0.5, 1, 2, 1.7, 1.2, 0.2};
    double y[6] = {0};

    timeConvolve(x, 4, h, 3, y);

    for(int i = 0; i < 6; i++)
    {
        ASSERT_DOUBLE_EQ(y[i], expectedy[i]);
    }
}

//tests all negatives
TEST(testTimeConvolve, testConvolution2)
{
    double x[4] = {-0.5, -0.5, -1.0, -0.2};
    double h[3] = {-1.0, -1.0, -1.0};
    double expectedy[6] = {0.5, 1, 2, 1.7, 1.2, 0.2};
    double y[6] = {0};

    timeConvolve(x, 4, h, 3, y);

    for(int i = 0; i < 6; i++)
    {
        ASSERT_DOUBLE_EQ(y[i], expectedy[i]);
    }
}

//tests mix of positives and negatives
TEST(testTimeConvolve, testConvolution3)
{
    double x[4] = {-0.5, 0.5, -1.0, 0.2};
    double h[3] = {-1.0, 1.0, -1.0};
    double expectedy[6] = {0.5, -1, 2, -1.7, 1.2, -0.2};
    double y[6] = {0};

    timeConvolve(x, 4, h, 3, y);

    for(int i = 0; i < 6; i++)
    {
        ASSERT_DOUBLE_EQ(y[i], expectedy[i]);
    }
}