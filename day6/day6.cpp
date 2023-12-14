
#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
#include <cmath>

int main()
{
    std::cout << std::setprecision(std::numeric_limits<double>::max_digits10);

    // std::vector<int> time{44, 80, 65, 72};
    // std::vector<int> distance{208, 1581, 1050, 1102};
    std::vector<long> time{44806572};
    std::vector<long> distance{208158110501102};
    // std::vector<int> time{7, 15, 30};
    // std::vector<int> distance{9, 40, 200};

    // c * (t - c) > distance
    // c^2 - ct + distance < 0
    // -b±√(b²-4ac))/(2a)

    long diff_prod = 1;
    for (int race = 0; race < time.size(); ++race)
    {
        double b = -time.at(race);
        double c = distance.at(race);

        double disc = std::sqrt(b * b - 4.0 * c);
        long low = std::ceil(0.5 * (-b - disc) + 1e-6);
        long high = std::floor(0.5 * (-b + disc) - 1e-6);
        long diff = high - low + 1.0;

        std::cout << 0.5 * (-b - disc) << " : " << low << "\n";
        std::cout << 0.5 * (-b + disc) << " : " << high << "\n";
        std::cout << diff << "\n";

        diff_prod *= diff;
    }

    std::cout << "final: " << diff_prod << "\n";
}
