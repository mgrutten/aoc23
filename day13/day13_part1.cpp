#include <iostream>
#include <string>
#include <fstream>
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

int vertical_reflection(MatrixXc puzzle)
{
    int flip_index = 0;
    for (int flip = 1; flip < puzzle.cols(); ++flip)
    {
        int flip_width = std::min(flip, (int)puzzle.cols() - flip);
        // std::cout << flip_width << "\n";

        MatrixXc lhs = puzzle.middleCols(flip - flip_width, flip_width);
        MatrixXc rhs = puzzle.middleCols(flip, flip_width).rowwise().reverse();

        // std::cout << lhs << "\n\n";
        // std::cout << rhs << "\n";
        if (lhs == rhs)
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
        MatrixXc puzzle = load_puzzle(infile);
        if (puzzle.rows() == 0)
        {
            break;
        }

        //std::cout << puzzle << "\n";
        //std::cout << vertical_reflection(puzzle) << "\n";
        //std::cout << horizontal_reflection(puzzle) << "\n";

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
