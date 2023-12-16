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

long hash(const std::string &step)
{
    long curr = 0;
    for (char c : step)
    {
        curr += c;
        curr *= 17;
        curr %= 256;
    }

    return curr;
}

int main()
{
    std::ifstream infile("input.txt");
    std::string line;
    std::getline(infile, line);

    std::vector<std::string> steps = split(line, ",");
    long sum = 0;
    for (std::string step : steps)
    {
        sum += hash(step);
    }
    std::cout << sum << "\n";
}