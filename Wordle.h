#ifndef WORDLE_H
#define WORDLE_H

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <cctype>

using size_type = std::string::size_type;
using letters_and_indices = std::map<size_type, std::string>;

class Wordle
{
public:

    struct wrong_fn
    {
        wrong_fn(const std::string &letters);
        bool operator()(const std::string &c);

    private:
        std::string l;
    };

    struct correct_fn
    {
        correct_fn(const letters_and_indices &idxs);
        bool operator()(const std::string &c);

    private:
        letters_and_indices m;
    };

    struct misplaced_fn
    {
        misplaced_fn(const letters_and_indices &idxs) : m{idxs} {}
        bool operator()(const std::string &c);

    private:
        letters_and_indices m;
    };

    struct exclude_word
    {
        exclude_word(const std::string &wrong,
                     const letters_and_indices &correct,
                     const letters_and_indices &misplaced);
        bool operator()(const std::string &w);

    private:
        wrong_fn wrong;
        correct_fn correct;
        misplaced_fn misplaced;
    };

    std::vector<std::string> read_candidates(std::istream &input);
    std::vector<std::string> filter_candidates(const std::vector<std::string> &candidates,
                                                       const std::string &wrong_letters,
                                                       const letters_and_indices &correct_positions,
                                                       const letters_and_indices &misplaced_positions);
                                                       
    void do_filter(std::vector<std::string> &c, std::string wrong,
                   letters_and_indices green, letters_and_indices yellow);

    void append(letters_and_indices &dest, const letters_and_indices &src);
    std::tuple<std::string, letters_and_indices, letters_and_indices> prompt();

    bool contains_any_of(const std::string &s, const std::string &cs);
    bool contains_at(const std::string &s, char c, size_type pos);
    static bool contains_but_not_at(const std::string &s, char c, size_type pos);
    letters_and_indices build_list(const std::string &line);
};
    
//private:
    

#endif
