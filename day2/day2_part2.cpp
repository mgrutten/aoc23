#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <algorithm>

std::vector<std::string> split(const std::string str, const std::string regex_str)
{
    std::regex regexz(regex_str);
    std::vector<std::string> list(std::sregex_token_iterator(str.begin(), str.end(), regexz, -1),
                                  std::sregex_token_iterator());
    return list;
}


int main()
{
    const std::vector<std::string> colours = {"red", "green", "blue"};
    const std::vector<int> thresholds = {12, 13, 14};

    std::ifstream infile("input.txt");

    std::string line;
    int counter = 0;
    while (std::getline(infile, line))
    {
        std::vector<std::string> game = split(line, ":");
        int line_num = std::stoi(game[0].substr(5));
        // std::cout << line_num << std::endl;

        std::vector<int> col_max = {0, 0, 0};

        std::vector<std::string> rounds = split(game[1], ";");
        for (std::string round : rounds)
        {
            std::vector<std::string> cols = split(round, ",");
            for (std::string col : cols)
            {
                std::vector<std::string> values = split(col, " ");
                // std::cout << values[1] << " - " << values[2] << std::endl;

                int count = std::stoi(values[1]);
                for (int c = 0; c < colours.size(); ++c)
                {
                    if (values[2] == colours[c])
                    {
                        col_max[c] = std::max(count, col_max[c]);
                    }
                }
            }
        }

        counter += col_max[0] * col_max[1] * col_max[2];
    }

    std::cout << counter << std::endl;
}
