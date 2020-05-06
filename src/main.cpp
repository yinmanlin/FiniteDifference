#include "EnumClass.h"
#include "Solver.h"
#include "BS.h"
#include <iostream>


int main(int argc, char const *argv[])
{
 
    Solver a = Solver(100, 100,
                      OptionExpiryType::European, false,
                      FiniteDifferenceType::Implicit,
                      100, 90, 1, 0, 0, 0.3, 5);
    //a.generateCoefSparseMats();
    std::vector<double> res {a.Solve(false)};
    for (auto e : res) std::cout << e << " ";
    std::cout << '\n';
    BS bs(100, 90, 0, 0, 1, 0.3, false);
    std::cout << bs.optionPrice() << '\n';
    std::cout << bs.delta() << '\n';
    std::cout << bs.gamma() << '\n';

    Solver b = Solver(100, 100,
                      OptionExpiryType::American, true,
                      FiniteDifferenceType::Implicit,
                      100, 110, 1, 0.2, 0, 0.3, 5);

    std::vector<double> resA {b.Solve(false)};
    for (auto e : resA) std::cout << e << " ";
    return 0;
}


