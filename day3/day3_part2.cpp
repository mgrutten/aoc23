#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <eigen3/Eigen/Eigen>

typedef Eigen::Matrix<char, Eigen::Dynamic, Eigen::Dynamic> MatrixXc;

struct Index
{
    int row;
    int col;

    friend std::ostream &operator<<(std::ostream &os, const Index &index)
    {
        os << "(" << index.row << "," << index.col << ")";
        return os;
    }

    void add(const Index &rhs, const int max_rows, const int max_cols)
    {
        row = std::max(0, std::min(max_rows, row + rhs.row));
        col = std::max(0, std::min(max_cols, col + rhs.col));
    }

    friend bool operator<(const Index &l, const Index &r)
    {
        return std::tie(l.row, l.col) < std::tie(r.row, r.col);
    }
};

bool is_symbol(const char c)
{
    return c == '*';
}

std::set<Index> get_neighbours(const MatrixXc &instructions, const int row, const int col)
{
    int rows = instructions.rows() - 1;
    int cols = instructions.cols() - 1;

    std::vector<Index> directions = {{-1, 0},
                                     {-1, +1},
                                     {0, +1},
                                     {+1, +1},
                                     {+1, 0},
                                     {+1, -1},
                                     {0, -1},
                                     {-1, -1}};
    std::set<Index> indices{};

    for (Index direction : directions)
    {
        Index index{row, col};
        index.add(direction, rows, cols);
        if (is_symbol(instructions(index.row, index.col)))
        {
            indices.insert(index);
        }
    }
    return indices;
}

int main()
{
    MatrixXc instructions;

    // Load instructions
    std::ifstream infile("input.txt");
    std::string line;
    while (std::getline(infile, line))
    {
        int curr_rows = instructions.rows();
        instructions.conservativeResize(curr_rows + 1, line.size());

        for (int c = 0; c < line.size(); ++c)
        {
            instructions(curr_rows, c) = line.at(c);
        }
    }

    std::cout << instructions.rows() << "x" << instructions.cols() << std::endl;
    // std::cout << instructions << std::endl;

    // Loop over each element, looking for neighbours
    std::vector<int> nums{};
    std::vector<std::set<Index>> num_connections{};
    for (int row = 0; row < instructions.rows(); ++row)
    {
        int num = 0;
        std::set<Index> connections{};
        for (int col = 0; col < instructions.cols(); ++col)
        {
            if (std::isdigit(instructions(row, col)))
            {
                num = 10 * num + instructions(row, col) - '0';
                for (Index neighbour : get_neighbours(instructions, row, col))
                {
                    connections.insert(neighbour);
                }
            }
            else
            {
                if (num > 0)
                {
                    nums.push_back(num);
                    num_connections.push_back(connections);
                }
                num = 0;
                connections.clear();
            }
        }
        if (num > 0)
        {
            nums.push_back(num);
            num_connections.push_back(connections);
        }
    }

    int count = 0;
    for (int n = 0; n < nums.size(); ++n)
    {
        if (num_connections.at(n).size() > 0)
        {
            for (int i = n + 1; i < nums.size(); ++i)
            {
                if (num_connections.at(i).find(*(num_connections.at(n).begin())) != num_connections.at(i).end())
                {
                    std::cout << nums.at(n) << " - " << nums.at(i) << std::endl;
                    count += nums.at(n) * nums.at(i);
                }
            }
            // std::cout << nums.at(n) << " - " << *(num_connections.at(n).begin()) << std::endl;
        }
    }
    std::cout << count << std::endl;
}
