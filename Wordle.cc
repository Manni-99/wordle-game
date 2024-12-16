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
            return true; 
        }
    }
    return false;
}

Wordle::correct_fn::correct_fn(const letters_and_indices &idxs) : m{idxs} {}

bool Wordle::correct_fn::operator()(const std::string &c)
{
    for (const auto &[index, letter] : m)
    {
        if (index >= c.size() || c[index] != letter[0])
        {
            return false; 
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
    if (unique_words.empty())
    {
        std::cerr << "Error: No valid five-letter words found in the word list.\n";
    }

    return std::vector<std::string>(unique_words.begin(), unique_words.end());
}

bool Wordle::contains_any_of(const std::string &s, const std::string &cs)
{
    return std::any_of(cs.begin(), cs.end(), [&s](char c)
                       { return s.find(c) != std::string::npos; });
}

bool Wordle::contains_at(const std::string &s, char c, size_type pos)
{
    return pos < s.size() && s[pos] == c;
}

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

    while (stream >> letter >> index)
    {
        result[index] = std::string(1, letter); 
        if (!std::isalpha(letter))
        {
            std::cerr << "Error: Invalid letter '" << letter << "' in input.\n";
            continue;
        }
        // Skip comma and space between pairs, if present
        if (stream.peek() == ',')
        {
            stream.ignore();
        }
    }
    return result;
}

void Wordle::append(std::map<unsigned long, std::string>& map1, const std::map<unsigned long, std::string>& map2) {
    map1.insert(map2.begin(), map2.end());
}

std::tuple<std::string, letters_and_indices, letters_and_indices> Wordle::prompt()
{
    std::string wrong;
    std::cout << "enter wrong letters (gray):\n";
    std::getline(std::cin, wrong);

    std::string correct;
    std::cout << "enter correct letters and the letter index (green)*:\n";
    std::getline(std::cin, correct);
    auto corr = build_list(correct);

    std::string misplaced;
    std::cout << "enter misplaced letters and letter index (yellow)*:\n";
    std::getline(std::cin, misplaced);
    auto misp = build_list(misplaced);

    return {wrong, corr, misp};
}

// Function to filter candidates
std::vector<std::string> Wordle::filter_candidates(const std::vector<std::string> &candidates,
                                                   const std::string &wrong_letters,
                                                   const letters_and_indices &correct_positions,
                                                   const letters_and_indices &misplaced_positions)
{
    std::vector<std::string> filtered_candidates;

    auto wrong = wrong_fn(wrong_letters);
    auto correct = correct_fn(correct_positions);
    auto misplaced = misplaced_fn(misplaced_positions);

    for (const auto &candidate : candidates)
    {
        if (!wrong(candidate) && correct(candidate) && misplaced(candidate))
        {
            filtered_candidates.push_back(candidate);
        }
    }

    return filtered_candidates;
}
