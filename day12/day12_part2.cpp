#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <vector>
#include <algorithm>
#include <numeric>

std::vector<std::string> split(const std::string str, const std::string regex_str)
{
    std::regex regexz(regex_str);
    std::vector<std::string> list(std::sregex_token_iterator(str.begin(), str.end(), regexz, -1),
                                  std::sregex_token_iterator());
    return list;
}

std::ostream &operator<<(std::ostream &os, const std::vector<int> &vector)
{
    for (auto itr = vector.begin(); itr != vector.end(); ++itr)
    {
        os << *itr << ",";
    }
    return os;
}

long combinations(const std::string &chars,
                  const std::vector<int> &damaged,
                  bool in_sequence,
                  std::map<std::tuple<std::string, std::vector<int>, bool>, long> &remaining)
{
    // std::cout << "New problem: " << chars << " x " << damaged << "\n";

    // End of pattern, either OK (1) or not (0)
    if (chars.empty())
    {
        if (damaged.empty())
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    // Check for previous test of this problem
    std::tuple<std::string, std::vector<int>, bool> key = std::make_tuple(chars, damaged, in_sequence);
    auto rem = remaining.find(key);
    if (rem != remaining.end())
    {
        return rem->second;
    }

    // Expecting hash
    if (in_sequence && !(chars.at(0) == '#' || chars.at(0) == '?'))
    {
        return 0;
    }

    // Dot character.  Just move to next position
    if (!in_sequence && (chars.at(0) == '.'))
    {
        long count = combinations(chars.substr(1), damaged, false, remaining);
        remaining.insert({key, count});
        return count;
    }

    // Question. Place a '.' at current position
    long dot_combinations = 0;
    if (!in_sequence && (chars.at(0) == '?'))
    {
        dot_combinations = combinations(chars.substr(1), damaged, false, remaining);
    }

    // Either actual #, or ? assumed # at current position
    long hash_combinations = 0;
    if (!damaged.empty())
    {
        if (damaged.at(0) == 1)
        {
            if (chars.at(1) == '?' || chars.at(1) == '.')
            {
                std::string new_chars = chars.substr(1);
                std::vector<int> new_damaged;
                std::copy(std::next(damaged.begin()), damaged.end(), std::back_inserter(new_damaged));
                new_chars.at(0) = '.';
                hash_combinations = combinations(new_chars, new_damaged, false, remaining);
            }
        }
        else
        {
            std::vector<int> new_damaged(damaged);
            new_damaged.at(0) -= 1;
            hash_combinations = combinations(chars.substr(1), new_damaged, true, remaining);
        }
    }

    remaining.insert({key, dot_combinations + hash_combinations});
    return dot_combinations + hash_combinations;
}

int main()
{
    std::ifstream infile("input.txt");

    std::string line;
    long count = 0;
    while (std::getline(infile, line))
    {
        std::vector<std::string> pair = split(line, " ");

        std::string chars = pair.at(0);
        for (int i = 0; i < 4; ++i)
        {
            chars += "?" + pair.at(0);
        }
        chars += '.';

        std::string damaged_rep = pair.at(1);
        for (int i = 0; i < 4; ++i)
        {
            damaged_rep += "," + pair.at(1);
        }

        // std::string chars = pair.at(0).append(".");
        std::cout << chars << "::" << damaged_rep << "\n";

        std::vector<std::string> damaged_str = split(damaged_rep, ",");
        std::vector<int> damaged;
        std::transform(damaged_str.begin(), damaged_str.end(), std::back_inserter(damaged), [](std::string str)
                       { return std::stoi(str); });

        std::map<std::tuple<std::string, std::vector<int>, bool>, long> remaining;
        long combs = combinations(chars, damaged, false, remaining);
        std::cout << combs << "\n";

        count += combs;
    }

    std::cout << "Final count: " << count << "\n";
}
