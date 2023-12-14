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

    std::vector<int> matches;
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

        matches.push_back(match_count);
    }

    std::vector<long> card_counts(matches.size(), 1);
    for (int card = 0; card < matches.size(); ++card)
    {
        //std::cout << "*** " << card << " - " << std::min((int)matches.size() - 1, card + matches.at(card)) << "\n";
        for (int match = card + 1; match <= std::min((int)matches.size() - 1, card + matches.at(card)); ++match)
        {
            card_counts.at(match) += card_counts.at(card);
        }

        //for (int c = 0; c < 10; ++c)
        //{
        //    std::cout << c << ": " << matches.at(c) << " - " << card_counts.at(c) << "\n";
        //}
    }

    long count = 0;
    for (long card_count : card_counts)
    {
        count += card_count;
    }
    std::cout << count << "\n";
}
