#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <climits>

struct RangeMap
{
    long destination;
    long source;
    long length;

    long map_val(long val)
    {
        if (val >= source && val <= (source + length - 1))
        {
            return destination + val - source;
        }
        return -1;
    }

    friend std::ostream &operator<<(std::ostream &os, const RangeMap &range)
    {
        os << "(" << range.destination << ", " << range.source << ", " << range.length << ")";
        return os;
    }
};

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

    std::string seed_line;
    std::getline(infile, seed_line);
    std::vector<std::string> seed_strs = split(seed_line.substr(7), " ");
    std::vector<long> seeds;
    for (std::string seed_str : seed_strs)
    {
        // std::cout << "- " << seed_str << "\n";
        seeds.push_back(std::stol(seed_str));
    }

    std::vector<std::vector<RangeMap>> maps;
    std::string line;
    std::getline(infile, line);
    while (true)
    {
        std::string label_line;
        if (!std::getline(infile, label_line))
        {
            break;
        }
        // std::cout << label_line << "\n";

        std::vector<RangeMap> ranges;
        std::string data_line;
        std::getline(infile, data_line);
        while (!data_line.empty())
        {
            std::vector<std::string> data = split(data_line, " ");
            ranges.push_back({std::stol(data[0]), std::stol(data[1]), std::stol(data[2])});
            std::getline(infile, data_line);
        }
        maps.push_back(ranges);
    }

    long min_location = LONG_MAX;
    for (long seed : seeds)
    {
        long val = seed;
        for (std::vector<RangeMap> map : maps)
        {
            for (RangeMap range : map)
            {
                // std::cout << range << "\n";
                long mapped_val = range.map_val(val);
                if (mapped_val >= 0)
                {
                    val = mapped_val;
                    break;
                }
            }
        }
        std::cout << seed << ": " << val << "\n";
        min_location = std::min(min_location, val);
    }
    std::cout << min_location << "\n";
}
