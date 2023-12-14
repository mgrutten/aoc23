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

void print_nums(const std::vector<long> nums, const int start)
{
    for (auto itr = std::next(nums.begin(), start); itr != nums.end(); ++itr)
    {
        std::cout << *itr << ":";
    }
    std::cout << "\n";
}

bool all_zeros(const std::vector<long> nums, const int start)
{
    return std::find_if_not(std::next(nums.begin(), start), nums.end(), [](long v)
                            { return v == 0; }) == nums.end();
}

int main()
{
    std::ifstream infile("input.txt");

    std::string line;
    long count = 0;
    while (std::getline(infile, line))
    {
        std::vector<std::string> num_strs = split(line, " ");
        std::vector<long> nums;
        std::transform(num_strs.begin(), num_strs.end(), std::back_inserter(nums), [](std::string str)
                       { return std::stoi(str); });

        print_nums(nums, 0);

        std::vector<long> last_elements;
        int start = 1;
        while (true)
        {
            last_elements.push_back(*std::prev(nums.end()));

            std::vector<long> diffs;
            std::adjacent_difference(nums.begin(), nums.end(), std::back_inserter(diffs));

            print_nums(diffs, start);

            if (all_zeros(diffs, start))
            {
                break;
            }

            // last_elements.push_back(*std::prev(diffs.end()));

            nums = diffs;
            start += 1;
        }

        std::cout << " - ";
        print_nums(last_elements, 0);
        count = std::accumulate(last_elements.begin(), last_elements.end(), count);

        std::cout << "\n";
    }

    std::cout << count << "\n";
}
