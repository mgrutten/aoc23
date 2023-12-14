#include <iostream>
#include <string>
#include <fstream>
#include <eigen3/Eigen/Eigen>

typedef Eigen::Matrix<char, Eigen::Dynamic, Eigen::Dynamic> MatrixXc;

bool between(int left, int right, int val)
{
    return ((right > left) & (val > left) & (val < right)) |
           ((right < left) & (val < left) & (val > right)) |
           ((right == left) & (left == val));
}

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

        for (int c = 0; c < line.size(); ++c)
        {
            input(curr_rows, c) = line.at(c);
        }
    }
    std::cout << input.rows() << "x" << input.cols() << "\n";

    // Find empty rows
    std::vector<int> empty_rows;
    for (int r = 0; r < input.rows(); ++r)
    {
        bool all_empty = true;
        for (int c = 0; c < input.cols(); ++c)
        {
            all_empty = all_empty & (input(r, c) == '.');
        }
        if (all_empty)
        {
            empty_rows.push_back(r);
        }
    }
    std::cout << "Empty rows: " << empty_rows.size() << "\n";

    // Find empty columns
    std::vector<int> empty_cols;
    for (int c = 0; c < input.cols(); ++c)
    {
        bool all_empty = true;
        for (int r = 0; r < input.rows(); ++r)
        {
            all_empty = all_empty & (input(r, c) == '.');
        }
        if (all_empty)
        {
            empty_cols.push_back(c);
        }
    }
    std::cout << "Empty cols: " << empty_cols.size() << "\n";

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

    // Sum up distances
    long cum_dist = 0;
    long multiplier = 1000000;
    for (int p1 = 0; p1 < galaxies.size(); ++p1)
    {
        for (int p2 = p1 + 1; p2 < galaxies.size(); ++p2)
        {
            int extra_rows = 0;
            for (int row : empty_rows)
            {
                if (between(galaxies.at(p1).first, galaxies.at(p2).first, row))
                {
                    extra_rows += 1;
                }
            }

            int extra_cols = 0;
            for (int col : empty_cols)
            {
                if (between(galaxies.at(p1).second, galaxies.at(p2).second, col))
                {
                    extra_cols += 1;
                }
            }

            long dist = std::abs(galaxies.at(p1).first - galaxies.at(p2).first) +
                        std::abs(galaxies.at(p1).second - galaxies.at(p2).second) +
                        (extra_rows + extra_cols) * (multiplier - 1);
            // std::cout << "pair: (" << galaxies.at(p1).first << "," << galaxies.at(p1).second << ") - ("
            //           << galaxies.at(p2).first << "," << galaxies.at(p2).second << "): "
            //           << extra_rows << " : " << extra_cols << " : " << dist << "\n";
            cum_dist += dist;
        }
    }
    std::cout << "Distance sum: " << cum_dist << "\n";
}
