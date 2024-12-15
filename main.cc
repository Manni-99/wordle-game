#include "Wordle.h"
#include <fstream>
#include <tuple>
#include <string>
int main() {
    Wordle wordle;

    // Read candidates
    std::ifstream word_list_file("wordlist.txt");
    if (!word_list_file) {
        std::cerr << "Error: Could not open word list file.\n";
        return 1;
    }
    std::vector<std::string> candidates = wordle.read_candidates(word_list_file);
    std::cout << candidates.size() << std::endl;
    for(std::string s : candidates){
        std::cout << s << "\n" << std::endl;
    }
    if (candidates.empty()) {
        std::cerr << "Error: No valid candidates found in word list.\n";
        return 1;
    }

    // Here we have to pick a single word from our list of candidates

    // Interact with the user
    while (true) {
        auto [wrong, correct, misplaced] = wordle.prompt();

        std::vector<std::string> filtered = wordle.filter_candidates(candidates, wrong, correct, misplaced);
        
        for(std::string s : filtered){
        std::cout << s << "\n" << std::endl;
    } 
        
        if (filtered.empty()) {
            std::cout << "No solutions match the criteria. Check your input!\n";
            break;
        }

        std::cout << "Filtered candidates:\n";
        for (const auto &word : filtered) {
            std::cout << word << '\n';
        }

        if (filtered.size() <= 1) {
            std::cout << "Solution found or reduced enough.\n";
            break;
        }
    }

    return 0;
}
