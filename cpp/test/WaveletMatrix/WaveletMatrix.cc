#include "WaveletMatrix/WaveletMatrix.h"

#include <gtest/gtest.h>
#include <vector>

using namespace std;

TEST(WaveletMatrix, select){
    vector<int> vec = {3, 7, 2, 7, 3, 8, 4, 5, 5, 7, 8};
    WaveletMatrix wm(vec);

    EXPECT_EQ(0, wm.select(0, 3));
    EXPECT_EQ(1, wm.select(0, 7));
    EXPECT_EQ(2, wm.select(0, 2));
    EXPECT_EQ(3, wm.select(1, 7));
    EXPECT_EQ(4, wm.select(1, 3));
    EXPECT_EQ(5, wm.select(0, 8));
}
