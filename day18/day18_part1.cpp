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

struct Index
{
    long row;
    long col;

    void move(const Index &dirn, int dist)
    {
        row += dirn.row * dist;
        col += dirn.col * dist;
    }

    friend bool operator<(const Index &l, const Index &r)
    {
        return std::tie(l.row, l.col) < std::tie(r.row, r.col);
    }

    friend std::ostream &operator<<(std::ostream &os, const Index &index)
    {
        os << "(" << index.row << "," << index.col << ")";
        return os;
    }
};

const std::map<char, Index> dirn_map({{'R', {0, 1}},
                                      {'U', {-1, 0}},
                                      {'L', {0, -1}},
                                      {'D', {1, 0}}});

int main()
{
    std::ifstream infile("input.txt");

    std::vector<Index> corners;
    Index curr{0, 0};
    corners.push_back(curr);

    std::string line;
    long perimeter = 0;
    while (std::getline(infile, line))
    {
        std::vector<std::string> step = split(line, " ");
        char dirn = step.at(0).at(0);
        int dist = std::stoi(step.at(1));

        curr.move(dirn_map.at(dirn), dist);
        corners.push_back(curr);

        perimeter += dist;
    }

    // Use https://en.wikipedia.org/wiki/Shoelace_formula#Triangle_form,_determinant_form
    // Last coordinate is repeated
    long area = 0;
    for (int c = corners.size() - 2; c >= 0; --c)
    {
        area += corners.at(c).col * corners.at(c + 1).row - corners.at(c + 1).col * corners.at(c).row;
    }
    std::cout << "Area: " << (area + perimeter) / 2 + 1 << "\n";
}