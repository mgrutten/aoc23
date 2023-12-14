#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

int main()
{

    std::ifstream infile("part1_input.txt");

    std::string line;
    int total = 0;
    while (std::getline(infile, line))
    {

        int first;
        for (int i = 0; i < line.length(); ++i)
        {
            if (std::isdigit(line.at(i)))
            {
                first = line.at(i) - '0';
                break;
            }
        }

        int last;
        for (int i = line.length() - 1; i >= 0; --i)
        {
            if (std::isdigit(line.at(i)))
            {
                last = line.at(i) - '0';
                break;
            }
        }

        total += 10 * first + last;
    }

    std::cout << total << std::endl;
}
