#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "Wordle.h"

using namespace std;

struct Wordle{
    vector<string> words;

    Wordle(){

    };

};

TEST(WordleTest, TestingForUniqueWords){
    Wordle wordle;
    EXPECT_EQ(0, wordle.words.size());
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}