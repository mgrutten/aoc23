#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <regex>

int main()
{

    const std::vector<std::string> names = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

    std::ifstream infile("part1_input.txt");

    std::string line;
    int total = 0;
    while (std::getline(infile, line))
    {
        int first_ind = 1000;
        int first_name;
        int last_ind = -1;
        int last_name;
        for (int i = 0; i < names.size(); ++i)
        {
            int first = line.find(names[i]);
            int last = line.rfind(names[i]);

            if (first >= 0 && first < first_ind)
            {
                first_ind = first;
                first_name = i;
            }

            if (last >= 0 && last > last_ind)
            {
                last_ind = last;
                last_name = i;
            }
        }

        int first = (first_name % 9) + 1;
        int last = (last_name % 9) + 1;
        total += 10 * first + last;
    }

    std::cout << total << std::endl;
}
