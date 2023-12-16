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

long roll_rocks(MatrixXc &puzzle)
{

    long changes = 0;
    for (int row = 1; row < puzzle.rows(); ++row)
    {
        for (int col = 0; col < puzzle.cols(); ++col)
        {
            if (puzzle(row, col) == 'O' && puzzle(row - 1, col) == '.')
            {
                puzzle(row - 1, col) = 'O';
                puzzle(row, col) = '.';
                changes += 1;
            }
        }
    }

    return changes;
}

long sum_load(const MatrixXc &puzzle)
{
    long load = 0;
    for (int row = 0; row < puzzle.rows(); ++row)
    {
        for (int col = 0; col < puzzle.cols(); ++col)
        {
            if (puzzle(row, col) == 'O')
            {
                load += puzzle.rows() - row;
            }
        }
    }

    return load;
}

int main()
{
    long count = 0;
    std::ifstream infile("input.txt");
    MatrixXc puzzle = load_puzzle(infile);

    //std::cout << puzzle << "\n\n";
    while (roll_rocks(puzzle))
    {
    }
    //std::cout << puzzle << "\n\n";
    std::cout << sum_load(puzzle) << "\n";
}
