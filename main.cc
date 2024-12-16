#include "Wordle.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include <string>
#include <vector>

// Function declaration for do_filter
void do_filter(std::vector<std::string>& c, std::string wrong, 
               letters_and_indices green, letters_and_indices yellow);

int main(int argc, char *argv[])
{
    Wordle wordle;

    std::vector<std::string> candidates;

    // Load word list from file
    if (argc > 1)
    {
        std::ifstream word_list_file(argv[1]);
        if (!word_list_file)
        {
            std::cerr << "Error: Could not open file " << argv[1] << ".\n";
            return 1;
        }
        candidates = wordle.read_candidates(word_list_file);
    }

    if (candidates.empty())
    {
        std::cerr << "Error: No valid candidates found in the input word list.\n";
        return 1;
    }
    letters_and_indices green_letters;
    letters_and_indices yellow_letters;
    // Main game loop
    while (true)
    {
        // Prompt user for input
        auto [wrong, correct, misplaced] = wordle.prompt();

        // Merge green and yellow sets (if applicable)
        wordle.append(green_letters, correct);
        wordle.append(yellow_letters, misplaced);

        // Update the gray set by removing green/yellow letters
        for (const auto& [index, letter] : correct)
        {
            wrong.erase(std::remove(wrong.begin(), wrong.end(), letter[0]), wrong.end());
        }
        for (const auto& [index, letter] : misplaced)
        {
            wrong.erase(std::remove(wrong.begin(), wrong.end(), letter[0]), wrong.end());
        }
        
        // Call do_filter to filter candidates
        do_filter(candidates, wrong, correct, misplaced);

        // Check filtered candidates
        if (candidates.empty())
        {
            std::cout << "No solutions match the criteria. Check your input!\n";
            break;
        }

        // Display filtered candidates
        std::cout << "Filtered candidates:\n";
        for (const auto &word : candidates)
        {
            std::cout << word << '\n';
        }

        // Stop if a single solution is found
        if (candidates.size() == 1)
        {
            std::cout << "Solution found: " << candidates[0] << '\n';
            break;
        }
    }

    return 0;
}
