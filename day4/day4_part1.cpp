#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <set>

std::vector<std::string> split(const std::string str, const std::string regex_str)
{
    std::regex regexz(regex_str);
    std::vector<std::string> list(std::sregex_token_iterator(str.begin(), str.end(), regexz, -1),
                                  std::sregex_token_iterator());
    return list;
}

int main()
{
    std::ifstream infile("input.txt");

    int count = 0;
    std::string line;
    while (std::getline(infile, line))
    {
        std::string winner_str = line.substr(10, 30);
        // int line_num = std::stoi(game[0].substr(5));
        std::vector<int> winners;
        for (int i = 0; i < 10; ++i)
        {
            winners.push_back(std::stoi(winner_str.substr(3 * i, 2)));
            // std::cout << std::stoi(winner_str.substr(3*i, 2)) << "-";
        }
        // std::cout << " | ";

        std::string number_str = line.substr(42, 74);
        std::set<int> numbers;
        // int line_num = std::stoi(game[0].substr(5));
        for (int i = 0; i < 25; ++i)
        {
            numbers.insert(std::stoi(number_str.substr(3 * i, 2)));
            // std::cout << std::stoi(number_str.substr(3 * i, 2)) << "-";
        }
        // std::cout << "\n";

        int match_count = 0;
        for (int winner : winners)
        {
            if (numbers.find(winner) != numbers.end())
            {
                match_count += 1;
            }
        }

        if (match_count > 0)
        {
            //std::cout << match_count << " - " << (1 << (match_count - 1)) << "\n";
            count += (1 << (match_count - 1));
        }
    }

    std::cout << count << "\n";
}
