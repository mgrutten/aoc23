#include <iostream>
#include <string>
#include <fstream>
#include <eigen3/Eigen/Eigen>

typedef Eigen::Matrix<char, Eigen::Dynamic, Eigen::Dynamic> MatrixXc;

int main()
{
    MatrixXc input;

    // Load instructions
    std::ifstream infile("input.txt");
    std::string line;
    while (std::getline(infile, line))
    {
        int curr_rows = input.rows();
        input.conservativeResize(curr_rows + 1, line.size());

        bool all_empty = true;
        for (int c = 0; c < line.size(); ++c)
        {
            input(curr_rows, c) = line.at(c);
            all_empty = all_empty & (line.at(c) == '.');
        }

        // Add an extra row of empty space
        if (all_empty)
        {
            int curr_rows = input.rows();
            input.conservativeResize(curr_rows + 1, line.size());
            for (int c = 0; c < input.cols(); ++c)
            {
                input(curr_rows, c) = '.';
            }
        }
    }

    // Add extra columns
    for (int c = 0; c < input.cols(); ++c)
    {
        bool all_empty = true;
        for (int r = 0; r < input.rows(); ++r)
        {
            all_empty = all_empty & (input(r, c) == '.');
        }

        if (all_empty)
        {
            MatrixXc new_input(input.rows(), input.cols() + 1);
            new_input.setConstant(input.rows(), input.cols() + 1, '.');
            new_input.leftCols(c) = input.leftCols(c);
            new_input.rightCols(input.cols() - c) = input.rightCols(input.cols() - c);

            input = new_input;
            c += 1;
        }
    }

    std::cout << input.rows() << "x" << input.cols() << "\n";
    // std::cout << input << "\n";

    // Find all galaxies
    std::vector<std::pair<int, int>> galaxies;
    for (int r = 0; r < input.rows(); ++r)
    {
        for (int c = 0; c < input.cols(); ++c)
        {
            if (input(r, c) == '#')
            {
                galaxies.push_back({r, c});
            }
        }
    }

    std::cout << "Galaxy count: " << galaxies.size() << "\n";

    long cum_dist = 0;
    for (int p1 = 0; p1 < galaxies.size(); ++p1)
    {
        for (int p2 = p1 + 1; p2 < galaxies.size(); ++p2)
        {
            // std::cout << "pair: (" << galaxies.at(p1).first << "," << galaxies.at(p1).second << ") - ("
            //           << galaxies.at(p2).first << "," << galaxies.at(p2).second << ")\n";
            cum_dist += std::abs(galaxies.at(p1).first - galaxies.at(p2).first) +
                        std::abs(galaxies.at(p1).second - galaxies.at(p2).second);
        }
    }
    std::cout << "Distance sum: " << cum_dist << "\n";
}
