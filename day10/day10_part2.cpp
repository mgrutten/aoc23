#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <eigen3/Eigen/Eigen>

struct Index
{
    int row;
    int col;

    friend std::ostream &operator<<(std::ostream &os, const Index &index)
    {
        os << "(" << index.row << "," << index.col << ")";
        return os;
    }

    friend bool operator<(const Index &l, const Index &r)
    {
        return std::tie(l.row, l.col) < std::tie(r.row, r.col);
    }

    friend bool operator==(const Index &lhs, const Index &rhs)
    {
        return std::tie(lhs.row, lhs.col) == std::tie(rhs.row, rhs.col);
    }

    friend bool operator!=(const Index &lhs, const Index &rhs)
    {
        return !(lhs == rhs);
    }

    Index &operator+=(const Index &rhs)
    {
        row += rhs.row;
        col += rhs.col;
        return *this; // return the result by reference
    }

    friend Index operator+(Index lhs, const Index &rhs)
    {
        lhs += rhs;
        return lhs;
    }

    Index &operator-=(const Index &rhs)
    {
        row -= rhs.row;
        col -= rhs.col;
        return *this; // return the result by reference
    }

    friend Index operator-(Index lhs, const Index &rhs)
    {
        lhs -= rhs;
        return lhs;
    }

    Index operator-()
    {
        return {-row, -col};
    }
};

struct Node
{
    Index connect1;
    Index connect2;

    friend std::ostream &operator<<(std::ostream &os, const Node &node)
    {
        os << node.connect1 << " | " << node.connect2;
        return os;
    }

    friend bool operator==(const Node &lhs, const Node &rhs)
    {
        return std::tie(lhs.connect1, lhs.connect2) == std::tie(rhs.connect1, rhs.connect2);
    }

    friend bool operator!=(const Node &lhs, const Node &rhs)
    {
        return !(lhs == rhs);
    }
};

const std::map<char, Node> node_map = {
    {'|', {{-1, 0}, {1, 0}}},
    {'-', {{0, -1}, {0, 1}}},
    {'L', {{-1, 0}, {0, 1}}},
    {'J', {{-1, 0}, {0, -1}}},
    {'7', {{1, 0}, {0, -1}}},
    {'F', {{1, 0}, {0, 1}}},
    {'.', {{0, 0}, {0, 0}}},
    {'S', {{2, 2}, {2, 2}}}};

typedef Eigen::Matrix<Node, Eigen::Dynamic, Eigen::Dynamic> MatrixXn;
typedef Eigen::Matrix<char, Eigen::Dynamic, Eigen::Dynamic> MatrixXc;

int main()
{
    MatrixXn tiles;

    // Load instructions
    std::ifstream infile("test.txt");
    std::string line;
    while (std::getline(infile, line))
    {
        int curr_rows = tiles.rows();
        tiles.conservativeResize(curr_rows + 1, line.size());

        for (int c = 0; c < line.size(); ++c)
        {
            tiles(curr_rows, c) = node_map.at(line.at(c));
        }
    }

    std::cout << tiles.rows() << "x" << tiles.cols() << std::endl;

    // Find the starting point
    Index start;
    for (int r = 0; r < tiles.rows(); ++r)
    {
        for (int c = 0; c < tiles.cols(); ++c)
        {
            if (tiles(r, c) == node_map.at('S'))
            {
                start = {r, c};
                break;
            }
        }
    }

    std::cout << "Start: " << start << "\n";
    std::vector<Index> steps = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    // Take first step
    Index curr = start;
    int dirn = 0;
    for (Index step : steps)
    {
        curr = curr + step;
        std::cout << curr << " - " << step << " - " << -step << "\n";
        Node val = tiles(curr.row, curr.col);
        if (val.connect1 == -step)
        {
            std::cout << "match1: " << val << " - " << -step << "\n";
            dirn = 2;
            break;
        }
        if (val.connect2 == -step)
        {
            // std::cout << "match2: " << neighbour << "\n";
            dirn = 1;
            break;
        }
    }
    std::cout << "One step: " << curr << " - " << dirn << "\n";

    // Take steps until back at S
    MatrixXc visited(tiles.rows(), tiles.cols());
    visited.setConstant(tiles.rows(), tiles.cols(), '.');
    visited(curr.row, curr.col) = '*';
    long step_count = 0;
    while (tiles(curr.row, curr.col) != node_map.at('S'))
    {
        Node node = tiles(curr.row, curr.col);
        if (dirn == 1)
        {
            curr += node.connect1;
            Node curr_node = tiles(curr.row, curr.col);

            if (curr_node.connect1 == -node.connect1)
            {
                dirn = 2;
            }
            if (curr_node.connect2 == -node.connect1)
            {
                dirn = 1;
            }
        }
        else
        {
            curr += node.connect2;
            Node curr_node = tiles(curr.row, curr.col);

            if (curr_node.connect1 == -node.connect2)
            {
                dirn = 2;
            }
            if (curr_node.connect2 == -node.connect2)
            {
                dirn = 1;
            }
        }
        std::cout << "One step: " << curr << " - " << dirn << "\n";
        visited(curr.row, curr.col) = '*';

        step_count += 1;
    }

    std::cout << "Furthest point: " << (step_count + 1) / 2 << "\n";

    std::cout << visited << "\n";
}
