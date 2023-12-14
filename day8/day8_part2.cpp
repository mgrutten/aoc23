#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <numeric>

struct Node
{
    std::string name;
    std::string left;
    std::string right;

    friend std::ostream &operator<<(std::ostream &os, const Node &node)
    {
        os << node.name << " = (" << node.left << ", " << node.right << ")";
        return os;
    }
};

long gcd(long a, long b)
{
    while (b)
    {
        long t = b;
        b = a % b;
        a = t;
    }

    return a;
}

int main()
{
    std::ifstream infile("input.txt");

    // Directions
    std::string directions;
    std::getline(infile, directions);
    // std::cout << directions << std::endl;

    // Blank line
    std::string line;
    std::getline(infile, line);

    // Load in nodes
    std::map<std::string, Node> nodes;
    while (std::getline(infile, line))
    {
        std::string name = line.substr(0, 3);
        std::string left = line.substr(7, 3);
        std::string right = line.substr(12, 3);
        // std::cout << name << " - " << left << " - " << right << std::endl;

        nodes.insert({name, {name, left, right}});
    }

    // Find all "A" nodes
    std::vector<Node> starts;
    for (std::pair<std::string, Node> start : nodes)
    {
        if (start.first.at(2) == 'A')
        {
            starts.push_back(start.second);
        }
    }

    for (Node node : starts)
    {
        std::cout << node << std::endl;
    }

    // Follow directions from each start point
    long multiple = 1;
    for (Node curr : starts)
    {
        int step = 0;
        while (curr.name.at(2) != 'Z')
        {
            char dirn = directions.at(step % directions.size());
            if (dirn == 'L')
            {
                curr = nodes.at(curr.left);
            }
            else
            {
                curr = nodes.at(curr.right);
            }

            step += 1;
        }

        std::cout << step << " : " << step / directions.size() << std::endl;
        multiple *= step / directions.size();
    }

    std::cout << multiple * directions.size() << std::endl;
}
