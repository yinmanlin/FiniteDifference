#include "EnumClass.h"
#include "Solver.h"
#include "BS.h"
#include <iostream>


int main(int argc, char const *argv[])
{
 
    std::cout << "BS: \n";
    BS bs(100, 100, 0, 0.02, 1, 0.3, true);
    std::cout << bs.optionPrice() << '\n';
    std::cout << bs.delta() << '\n';
    std::cout << bs.gamma() << '\n';

    std::cout << "European : \n";
    Solver a1 = Solver(100, 100,
                      OptionExpiryType::European, true,
                      FiniteDifferenceType::Implicit,
                      100, 100, 1, 0.02, 0, 0.3, 5);

    std::vector<double> resA1 {a1.Solve(false)};
    for (auto e : resA1) std::cout << e << " ";
    std::cout << "American : \n";
    Solver b = Solver(100, 100,
                      OptionExpiryType::American, true,
                      FiniteDifferenceType::Implicit,
                      100, 100, 1, 0.02, 0, 0.3, 5);

    std::vector<double> resA {b.Solve(false)};
    for (auto e : resA) std::cout << e << " ";
    std::cout << "Bermuda : \n";
    std::vector<double> schedule{0.2,0.4,0.6,0.8,1.0};
    Solver c = Solver(100, 100,
                      OptionExpiryType::Bermuda, true,
                      FiniteDifferenceType::Implicit,
                      100, 100, 1, 0.02, 0, 0.3, 5, schedule);
    std::vector<double> resC {c.Solve(false)};
    for (auto e : resC) std::cout << e << " ";  
    return 0;
}


