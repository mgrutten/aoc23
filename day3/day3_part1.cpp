#include <iostream>
#include <string>
#include <fstream>
#include <eigen3/Eigen/Eigen>

typedef Eigen::Matrix<char, Eigen::Dynamic, Eigen::Dynamic> MatrixXc;

bool is_symbol(const char c)
{
    return !std::isdigit(c) & !(c == '.');
}

bool has_neighbour(const MatrixXc &instructions, const int row, const int col)
{
    int rows = instructions.rows() - 1;
    int cols = instructions.cols() - 1;
    return is_symbol(instructions(std::max(0, row - 1), col)) ||
           is_symbol(instructions(std::max(0, row - 1), std::min(cols, col + 1))) ||
           is_symbol(instructions(row, std::min(cols, col + 1))) ||
           is_symbol(instructions(std::min(rows, row + 1), std::min(cols, col + 1))) ||
           is_symbol(instructions(std::min(rows, row + 1), col)) ||
           is_symbol(instructions(std::min(rows, row + 1), std::max(0, col - 1))) ||
           is_symbol(instructions(row, std::max(0, col - 1))) ||
           is_symbol(instructions(std::max(0, row - 1), std::max(0, col - 1)));
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
    std::vector<bool> part_nums{};
    for (int row = 0; row < instructions.rows(); ++row)
    {
        int num = 0;
        bool part_num = false;
        for (int col = 0; col < instructions.cols(); ++col)
        {
            if (std::isdigit(instructions(row, col)))
            {
                num = 10 * num + instructions(row, col) - '0';
                if (has_neighbour(instructions, row, col))
                {
                    part_num = true;
                }
            }
            else
            {
                if (num > 0)
                {
                    nums.push_back(num);
                    part_nums.push_back(part_num);
                }
                num = 0;
                part_num = false;
            }
        }
        if (num > 0)
        {
            nums.push_back(num);
            part_nums.push_back(part_num);
        }
    }

    int part_sum = 0;
    for (int n = 0; n < nums.size(); ++n)
    {
        part_sum += part_nums.at(n) ? nums.at(n) : 0;
        // std::cout << nums.at(n) << " - " << part_nums.at(n) << std::endl;
    }
    std::cout << part_sum << std::endl;
}
