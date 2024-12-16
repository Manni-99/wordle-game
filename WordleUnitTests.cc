#include <gtest/gtest.h>
#include "Wordle.h"
#include <sstream>

class WordleUnitTests : public ::testing::Test {
protected:
    Wordle wordle;
};

TEST_F(WordleUnitTests, ContainsAnyOf_FindsLetters) {
    // Test cases for contains_any_of
    EXPECT_TRUE(wordle.contains_any_of("apple", "ae")); // 'a' and 'e' in "apple"
    EXPECT_FALSE(wordle.contains_any_of("apple", "xyz")); // None of 'x', 'y', 'z' in "apple"
    EXPECT_TRUE(wordle.contains_any_of("apple", "lp")); // 'l' and 'p' in "apple"
    EXPECT_FALSE(wordle.contains_any_of("", "ae")); // Empty string
}

TEST_F(WordleUnitTests, ContainsAt_FindsCorrectPosition) {
    // Test cases for contains_at
    EXPECT_TRUE(wordle.contains_at("apple", 'a', 0)); // 'a' at position 0
    EXPECT_TRUE(wordle.contains_at("apple", 'p', 1)); // 'p' at position 1
    EXPECT_FALSE(wordle.contains_at("apple", 'p', 0)); // 'p' not at position 0
    EXPECT_FALSE(wordle.contains_at("apple", 'z', 3)); // 'z' not in "apple"
    EXPECT_FALSE(wordle.contains_at("apple", 'e', 10)); // Out-of-bounds index
}

TEST_F(WordleUnitTests, ContainsButNotAt_FindsMismatchedPosition) {
    // Test cases for contains_but_not_at
    EXPECT_TRUE(wordle.contains_but_not_at("apple", 'a', 1)); // 'a' is in "apple" but not at position 1
    EXPECT_FALSE(wordle.contains_but_not_at("apple", 'a', 0)); // 'a' is at position 0
    EXPECT_FALSE(wordle.contains_but_not_at("apple", 'z', 2)); // 'z' is not in "apple"
    EXPECT_TRUE(wordle.contains_but_not_at("apple", 'p', 0)); // 'p' is in "apple" but not at position 0
}

TEST_F(WordleUnitTests, BuildList_CreatesCorrectMapping) {
    // Test cases for build_list
    auto result = wordle.build_list("a 0, b 1, c 2");
    EXPECT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "b");
    EXPECT_EQ(result[2], "c");

    result = wordle.build_list("x 3, y 4");
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[3], "x");
    EXPECT_EQ(result[4], "y");

    result = wordle.build_list(""); // Empty input
    EXPECT_EQ(result.size(), 0);
}

TEST_F(WordleUnitTests, ReadCandidates_FiltersFiveLetterWords) {
    // Mock input stream
    std::istringstream input("apple\nbanana\nberry\nkiwi\nmelon\npeach\n");
    auto candidates = wordle.read_candidates(input);
    EXPECT_EQ(candidates.size(), 4);
    EXPECT_NE(std::find(candidates.begin(), candidates.end(), "apple"), candidates.end());
    EXPECT_NE(std::find(candidates.begin(), candidates.end(), "melon"), candidates.end());
    EXPECT_NE(std::find(candidates.begin(), candidates.end(), "peach"), candidates.end());
}

TEST_F(WordleUnitTests, FilterCandidates_RemovesInvalidWords) {
    std::vector<std::string> candidates = {"apple", "berry", "melon", "grape"};

    // Case 1: Filter out words with wrong letters
    std::string wrong_letters = "a";
    letters_and_indices correct_positions;
    letters_and_indices misplaced_positions;
    auto filtered = wordle.filter_candidates(candidates, wrong_letters, correct_positions, misplaced_positions);
    EXPECT_EQ(filtered.size(), 2); // "apple" should be removed
    EXPECT_NE(std::find(filtered.begin(), filtered.end(), "berry"), filtered.end());
    EXPECT_NE(std::find(filtered.begin(), filtered.end(), "melon"), filtered.end());
    //EXPECT_NE(std::find(filtered.begin(), filtered.end(), "grape"), filtered.end());

    // Case 2: Keep only words with correct positions
    correct_positions[0] = "b";
    filtered = wordle.filter_candidates(candidates, "", correct_positions, misplaced_positions);
    EXPECT_EQ(filtered.size(), 1); 
    EXPECT_EQ(filtered[0], "berry");

    // Case 3: Words with misplaced letters
    misplaced_positions[1] = "r"; 
    filtered = wordle.filter_candidates(candidates, "", correct_positions, misplaced_positions);
    EXPECT_EQ(filtered.size(), 1); 
    EXPECT_EQ(filtered[0], "berry");
}

TEST_F(WordleUnitTests, Append_MergesMapsCorrectly) {
    letters_and_indices map1{{0, "a"}, {1, "b"}};
    letters_and_indices map2{{2, "c"}, {3, "d"}};

    wordle.append(map1, map2);

    EXPECT_EQ(map1.size(), 4);
    EXPECT_EQ(map1[0], "a");
    EXPECT_EQ(map1[1], "b");
    EXPECT_EQ(map1[2], "c");
    EXPECT_EQ(map1[3], "d");
}
