#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
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

MatrixXc rotate_clockwise(const MatrixXc &m)
{
    MatrixXc rm(m.cols(), m.rows());

    for (int r = 0; r < m.rows(); ++r)
    {
        rm.col(m.rows() - r - 1) = m.row(r);
    }

    return rm;
}

MatrixXc single_cycle(const MatrixXc &m)
{
    MatrixXc puzzle = m;
    while (roll_rocks(puzzle))
    {
    }
    puzzle = rotate_clockwise(puzzle);

    while (roll_rocks(puzzle))
    {
    }
    puzzle = rotate_clockwise(puzzle);

    while (roll_rocks(puzzle))
    {
    }
    puzzle = rotate_clockwise(puzzle);

    while (roll_rocks(puzzle))
    {
    }
    puzzle = rotate_clockwise(puzzle);

    return puzzle;
}

// https://github.com/ethz-asl/map_api/blob/master/map-api-common/include/map-api-common/eigen-hash.h
namespace std
{

    template <typename Scalar, int Rows, int Cols>
    struct hash<Eigen::Matrix<Scalar, Rows, Cols>>
    {
        // https://wjngkoh.wordpress.com/2015/03/04/c-hash-function-for-eigen-matrix-and-vector/
        size_t operator()(const Eigen::Matrix<Scalar, Rows, Cols> &matrix) const
        {
            size_t seed = 0;
            for (size_t i = 0; i < matrix.size(); ++i)
            {
                Scalar elem = *(matrix.data() + i);
                seed ^=
                    std::hash<Scalar>()(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };

} // namespace std

int main()
{
    long count = 0;
    std::ifstream infile("input.txt");
    MatrixXc puzzle = load_puzzle(infile);

    // std::cout << puzzle << "\n\n";

    std::unordered_map<MatrixXc, long> puzzle_store;

    const long max_iterations = 1000000000;
    for (long i = 0; i < max_iterations; ++i)
    {
        MatrixXc new_puzzle = single_cycle(puzzle);
        if (puzzle_store.find(new_puzzle) == puzzle_store.end())
        {
            puzzle_store.insert({new_puzzle, i});
        }
        else
        {
            long index = puzzle_store.at(new_puzzle);
            if ((max_iterations - i) % (index - i) == 0)
            {
                break;
            }
        }

        puzzle = new_puzzle;
        // std::cout << puzzle << "\n\n";
    }
    // std::cout << puzzle << "\n\n";
    std::cout << sum_load(puzzle) << "\n";
}
