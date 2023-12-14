#include <iostream>
#include <string>
#include <fstream>
#include <map>

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

int main()
{
    std::ifstream infile("input.txt");

    // Directions
    std::string directions;
    std::getline(infile, directions);
    //std::cout << directions << std::endl;

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

    // Follow directions
    Node curr = nodes.at("AAA");
    int step = 0;
    while (curr.name != "ZZZ")
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

    std::cout << step << std::endl;
}
