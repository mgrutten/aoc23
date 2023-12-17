#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <eigen3/Eigen/Eigen>

typedef Eigen::Matrix<char, Eigen::Dynamic, Eigen::Dynamic> MatrixXc;

MatrixXc load_puzzle(std::ifstream &infile)
{
    MatrixXc input;

    std::string line;
    while (std::getline(infile, line) && !line.empty())
    {
        int curr_rows = input.rows();
        input.conservativeResize(curr_rows + 1, line.size());

        for (int c = 0; c < line.size(); ++c)
        {
            input(curr_rows, c) = line.at(c);
        }
    }

    return input;
}

enum Direction
{
    RIGHT,
    UP,
    LEFT,
    DOWN
};

const std::map<Direction, std::string> direction_str = {
    {RIGHT, "RIGHT"},
    {UP, "UP"},
    {LEFT, "LEFT"},
    {DOWN, "DOWN"}};

struct Index
{
    long row;
    long col;

    char charat(const MatrixXc &data) const
    {
        return data(row, col);
    }

    void move(const Direction d, const MatrixXc &data)
    {
        switch (d)
        {
        case RIGHT:
            col = std::min(data.cols() - 1, col + 1);
            break;
        case UP:
            row = std::max(0L, row - 1);
            break;
        case LEFT:
            col = std::max(0L, col - 1);
            break;
        case DOWN:
            row = std::min(data.rows() - 1, row + 1);
            break;
        }
    }

    bool isedge(const Direction d, const MatrixXc &data)
    {
        switch (d)
        {
        case RIGHT:
            return col == data.cols() - 1;
        case UP:
            return row == 0;
        case LEFT:
            return col == 0;
        case DOWN:
            return row == data.rows() - 1;
        }
        return false;
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

std::set<std::pair<Index, Direction>> explored;

void project_ray(const Index &start, const Direction dirn, const MatrixXc &data, MatrixXc &energized)
{
    // Step to mirror/split
    Index curr = start;
    // std::cout << curr << std::endl;
    energized(curr.row, curr.col) = '#';

    while (!curr.isedge(dirn, data) && curr.charat(data) == '.')
    {
        curr.move(dirn, data);
        energized(curr.row, curr.col) = '#';
    }
    // std::cout << "current " << curr << "\n";

    switch (curr.charat(data))
    {
    case '|':
        if (dirn == RIGHT || dirn == LEFT)
        {
            if (explored.count({curr, UP}) == 0 && !curr.isedge(UP, data))
            {
                explored.insert({curr, UP});
                // std::cout << "project UP"
                //           << "\n";
                Index ind(curr);
                ind.move(UP, data);
                project_ray(ind, UP, data, energized);
            }
            if (explored.count({curr, DOWN}) == 0 && !curr.isedge(DOWN, data))
            {
                explored.insert({curr, DOWN});
                // std::cout << "project DOWN"
                //           << "\n";
                Index ind(curr);
                ind.move(DOWN, data);
                project_ray(ind, DOWN, data, energized);
            }
        }
        else if (explored.count({curr, dirn}) == 0 && !curr.isedge(dirn, data))
        {
            explored.insert({curr, dirn});
            // std::cout << "continue " << direction_str.at(dirn) << "\n";
            Index ind(curr);
            ind.move(dirn, data);
            project_ray(ind, dirn, data, energized);
        }
        break;
    case '-':
        if (dirn == UP || dirn == DOWN)
        {
            if (explored.count({curr, LEFT}) == 0 && !curr.isedge(LEFT, data))
            {
                explored.insert({curr, LEFT});
                // std::cout << "project LEFT"
                //           << "\n";
                Index ind(curr);
                ind.move(LEFT, data);
                project_ray(ind, LEFT, data, energized);
            }
            if (explored.count({curr, RIGHT}) == 0 && !curr.isedge(RIGHT, data))
            {
                explored.insert({curr, RIGHT});
                // std::cout << "project RIGHT"
                //           << "\n";
                Index ind(curr);
                ind.move(RIGHT, data);
                project_ray(ind, RIGHT, data, energized);
            }
        }
        else if (explored.count({curr, dirn}) == 0 && !curr.isedge(dirn, data))
        {
            explored.insert({curr, dirn});
            // std::cout << "continue " << direction_str.at(dirn) << "\n";
            Index ind(curr);
            ind.move(dirn, data);
            project_ray(ind, dirn, data, energized);
        }
        break;
    case '\\':
        switch (dirn)
        {
        case RIGHT:
            if (!curr.isedge(DOWN, data))
            {
                // std::cout << "project DOWN"
                //           << "\n";
                Index ind(curr);
                ind.move(DOWN, data);
                project_ray(ind, DOWN, data, energized);
            }
            break;
        case UP:
            if (!curr.isedge(LEFT, data))
            {
                // std::cout << "project LEFT"
                //           << "\n";
                Index ind(curr);
                ind.move(LEFT, data);
                project_ray(ind, LEFT, data, energized);
            }
            break;
        case LEFT:
            if (!curr.isedge(UP, data))
            {
                // std::cout << "project UP"
                //           << "\n";
                Index ind(curr);
                ind.move(UP, data);
                project_ray(ind, UP, data, energized);
            }
            break;
        case DOWN:
            if (!curr.isedge(RIGHT, data))
            {
                // std::cout << "project RIGHT"
                //           << "\n";
                Index ind(curr);
                ind.move(RIGHT, data);
                project_ray(ind, RIGHT, data, energized);
            }
            break;
        }
        break;
    case '/':
        switch (dirn)
        {
        case RIGHT:
            if (!curr.isedge(UP, data))
            {
                // std::cout << "project UP"
                //           << "\n";
                Index ind(curr);
                ind.move(UP, data);
                project_ray(ind, UP, data, energized);
            }
            break;
        case UP:
            if (!curr.isedge(RIGHT, data))
            {
                // std::cout << "project RIGHT"
                //           << "\n";
                Index ind(curr);
                ind.move(RIGHT, data);
                project_ray(ind, RIGHT, data, energized);
            }
            break;
        case LEFT:
            if (!curr.isedge(DOWN, data))
            {
                // std::cout << "project DOWN"
                //           << "\n";
                Index ind(curr);
                ind.move(DOWN, data);
                project_ray(ind, DOWN, data, energized);
            }
            break;
        case DOWN:
            if (!curr.isedge(LEFT, data))
            {
                // std::cout << "project LEFT"
                //           << "\n";
                Index ind(curr);
                ind.move(LEFT, data);
                project_ray(ind, LEFT, data, energized);
            }
            break;
        }
        break;
    }
}

int main()
{
    std::ifstream infile("input.txt");
    MatrixXc puzzle = load_puzzle(infile);

    Index curr = {0, 0};
    MatrixXc energized(puzzle.rows(), puzzle.cols());
    energized.fill('.');

    project_ray(curr, RIGHT, puzzle, energized);
    // std::cout << energized << "\n";
    long hash_count = (energized.array() == '#').count();
    std::cout << "Hash count: " << hash_count << "\n";
}
