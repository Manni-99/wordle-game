#include "Wordle.h"
#include <algorithm>
#include <unordered_set>
#include <sstream>
#include <iostream>

Wordle::wrong_fn::wrong_fn(const std::string &letters) : l{letters} {}

bool Wordle::wrong_fn::operator()(const std::string &c)
{
    for (char letter : l)
    {
        if (c.find(letter) != std::string::npos)
        {
            return true; // If any letter in "l" exists in "c", it's a mismatch
        }
    }
    return false;
}

Wordle::correct_fn::correct_fn(const letters_and_indices &idxs) : m{idxs} {}

// Implementation of correct_fn::operator()
bool Wordle::correct_fn::operator()(const std::string &c)
{
    for (const auto &[index, letter] : m)
    {
        if (index >= c.size() || c[index] != letter[0])
        {
            return false; // If a letter at a specific index doesn't match, fail
        }
    }
    return true;
}

bool Wordle::misplaced_fn::operator()(const std::string &c)
{
    for (const auto &[index, letter] : m)
    {
        if (!contains_but_not_at(c, letter[0], index))
        {
            return false;
        }
    }
    return true;
}

std::vector<std::string> Wordle::read_candidates(std::istream &input)
{
    std::unordered_set<std::string> unique_words;

    std::string word;
    while (input >> word)
    {
        if (word.length() == 5)
        {
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            unique_words.insert(word);
        }
    }

    return std::vector<std::string>(unique_words.begin(), unique_words.end());
}

// Private helper function: contains_any_of
bool Wordle::contains_any_of(const std::string &s, const std::string &cs)
{
    return std::any_of(cs.begin(), cs.end(), [&s](char c)
                       { return s.find(c) != std::string::npos; });
}

// Private helper function: contains_at
bool Wordle::contains_at(const std::string &s, char c, size_type pos)
{
    return pos < s.size() && s[pos] == c;
}

// Private helper function: contains_but_not_at
bool Wordle::contains_but_not_at(const std::string &s, char c, size_type pos)
{
    return s.find(c) != std::string::npos && (pos >= s.size() || s[pos] != c);
}

letters_and_indices Wordle::build_list(const std::string &line)
{
    letters_and_indices result;
    std::istringstream stream(line);
    char letter;
    size_type index;

    // Parse letter-index pairs (e.g., "a 0, b 1")
    while (stream >> letter >> index) {
        result[index] = std::string(1, letter);  // Insert letter at specified index
        
        // Skip comma and space between pairs, if present
        if (stream.peek() == ',') {
            stream.ignore();
        }
    }
    return result;
}

std::tuple<std::string, letters_and_indices, letters_and_indices> Wordle::prompt()
{
    std::string wrong;
    std::cout << "enter wrong letters:\n";
    std::getline(std::cin, wrong);

    std::string correct;
    std::cout << "enter correct letters (letter index)*:\n";
    std::getline(std::cin, correct);
    auto corr = build_list(correct);

    std::string misplaced;
    std::cout << "enter misplaced letters (letter index)*:\n";
    std::getline(std::cin, misplaced);
    auto misp = build_list(misplaced);

    // Debugging output to verify input parsing
    std::cout << "Wrong letters: " << wrong << std::endl;
    std::cout << "Correct letters with positions: ";
    for (const auto& [index, letter] : corr) {
        std::cout << letter << " " << index << ", ";
    }
    std::cout << std::endl;
    
    std::cout << "Misplaced letters with positions: ";
    for (const auto& [index, letter] : misp) {
        std::cout << letter << " " << index << ", ";
    }
    std::cout << std::endl;

    return {wrong, corr, misp};
}

// Function to filter candidates
std::vector<std::string> Wordle::filter_candidates(const std::vector<std::string> &candidates,
                                                   const std::string &wrong_letters,
                                                   const letters_and_indices &correct_positions,
                                                   const letters_and_indices &misplaced_positions)
{
    std::vector<std::string> filtered_candidates;

    for (const auto &candidate : candidates)
    {
        // Check wrong letters
        if (wrong_fn(wrong_letters)(candidate))
        {
            continue;
        }

        // Check correct positions
        if (!correct_fn(correct_positions)(candidate))
        {
            continue;
        }

        // Check misplaced positions
        bool valid = true;
        for (const auto &[index, letter] : misplaced_positions)
        {
            if (!contains_but_not_at(candidate, letter[0], index))
            {
                valid = false;
                break;
            }
        }

        if (valid)
        {
            filtered_candidates.push_back(candidate);
        }
    }

    return filtered_candidates;
}