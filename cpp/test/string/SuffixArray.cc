#include "string/SuffixArray.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace std;

TEST(SuffixArray, test) {
    vector<string> expected = {
        "a",
        "abra",
        "abracadabra",
        "acadabra",
        "adabra",
        "bra",
        "bracadabra",
        "cadabra",
        "dabra",
        "ra",
        "racadabra"
    };

    string s("abracadabra");
	SuffixArray sa(s);

    vector<string> sorted;
    for(size_t i=0; i<s.size(); i++) {
        sorted.push_back(sa.kth_suffix(i));
    }
    EXPECT_EQ(expected, sorted);
}
