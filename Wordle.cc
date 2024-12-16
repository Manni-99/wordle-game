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
void Wordle::do_filter(std::vector<std::string>& candidates, 
               std::string wrong, 
               letters_and_indices green, 
               letters_and_indices yellow) 
{
    Wordle::wrong_fn wrong_functor(wrong);
    Wordle::correct_fn green_functor(green);
    Wordle::misplaced_fn yellow_functor(yellow);

    // Remove invalid candidates from the vector using the functors
    candidates.erase(std::remove_if(candidates.begin(), candidates.end(),
                                    [&](const std::string& candidate) {
                                        return wrong_functor(candidate) || 
                                               !green_functor(candidate) || 
                                               !yellow_functor(candidate);  
                                    }),
                     candidates.end());
}

void Wordle::append(std::map<unsigned long, std::string>& map1, const std::map<unsigned long, std::string>& map2) {
    map1.insert(map2.begin(), map2.end());
}

std::string to_lowercase(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::tuple<std::string, letters_and_indices, letters_and_indices> Wordle::prompt() {
    std::string wrong;
    std::cout << "Enter wrong letters (gray):\n";
    std::getline(std::cin, wrong);
    wrong = to_lowercase(wrong);  // Convert the input to lowercase

    std::string correct;
    std::cout << "Enter correct letters and the letter index (green)*:\n";
    std::getline(std::cin, correct);
    correct = to_lowercase(correct);  // Convert the input to lowercase
    auto corr = build_list(correct);

    std::string misplaced;
    std::cout << "Enter misplaced letters and letter index (yellow)*:\n";
    std::getline(std::cin, misplaced);
    misplaced = to_lowercase(misplaced);  // Convert the input to lowercase
    auto misp = build_list(misplaced);

    return {wrong, corr, misp};
}

