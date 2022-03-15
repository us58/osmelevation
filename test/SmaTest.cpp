// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <gtest/gtest.h>
#include <vector>
#include <cstdint>
extern "C" {
#include "../../third_party/utsAlgorithms/sma.h"
}



// ____________________________________________________________________________
TEST(SMATEST, smaTest) {
    std::vector<double> result;
    result.resize(7);

    std::vector<double> elevations = { 10, 10, 10, 20, 10, 10, 10 };
    std::vector<double> distances = { 0, 20, 40, 60, 80, 100, 120 };
    
    int nodeCount = 7;
    double windowsHalfSize = 30;

    sma_linear(&elevations[0], &distances[0], &nodeCount, &result[0], &windowsHalfSize, &windowsHalfSize);

    ASSERT_FLOAT_EQ(10, result[0]);
    ASSERT_FLOAT_EQ(10.416667, result[1]);
    ASSERT_FLOAT_EQ(12.916667, result[2]);
    ASSERT_FLOAT_EQ(13.333333, result[3]);
    ASSERT_FLOAT_EQ(12.916667, result[4]);
    ASSERT_FLOAT_EQ(10.416667, result[5]);
    ASSERT_FLOAT_EQ(10, result[6]);
}
