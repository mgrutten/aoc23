#include <iostream>
#include <string>
#include <fstream>
#include <eigen3/Eigen/Eigen>

Eigen::MatrixXi load_puzzle(std::ifstream &infile)
{
    Eigen::MatrixXi input;

    std::string line;
    int p_index = 0;
    while (std::getline(infile, line) && !line.empty())
    {
        int curr_rows = input.rows();
        input.conservativeResize(curr_rows + 1, line.size());

        for (int c = 0; c < line.size(); ++c)
        {
            input(curr_rows, c) = line.at(c) == '#' ? 1 : 0;
        }
    }

    return input;
}

int vertical_reflection(Eigen::MatrixXi puzzle)
{
    int flip_index = 0;
    for (int flip = 1; flip < puzzle.cols(); ++flip)
    {
        int flip_width = std::min(flip, (int)puzzle.cols() - flip);
        // std::cout << flip_width << "\n";

        Eigen::MatrixXi lhs = puzzle.middleCols(flip - flip_width, flip_width);
        Eigen::MatrixXi rhs = puzzle.middleCols(flip, flip_width).rowwise().reverse();
        Eigen::MatrixXi lhs_or_rhs = lhs + rhs;

        // LHS and RHS differ by 1 and "OR" of LHS and RHS equals max
        // i.e. LHS has one extra element (or vice-versa)
        if (std::abs(lhs.sum() - rhs.sum()) == 1 && lhs_or_rhs.count() == std::max(lhs.sum(), rhs.sum()))
        {
            flip_index = flip;
            break;
        }
    }

    return flip_index;
}

int main()
{
    long count = 0;
    std::ifstream infile("input.txt");
    while (true)
    {
        Eigen::MatrixXi puzzle = load_puzzle(infile);
        if (puzzle.rows() == 0)
        {
            break;
        }

        if (int flip = vertical_reflection(puzzle))
        {
            count += flip;
        }
        else
        {
            count += 100 * vertical_reflection(puzzle.transpose());
        }

    }

    std::cout << count << "\n";
}
