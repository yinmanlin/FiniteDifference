# FiniteDifference

Usage:

std::cout << "European: \n";
Solver a1 = Solver(100, 100,
                  OptionExpiryType::European, true,
                  FiniteDifferenceType::Implicit,
                  100, 90, 1, 0, 0, 0.3, 5);
std::vector<double> resA1 {a1.Solve(false)};
for (auto e : resA1) std::cout << e << " ";
std::cout << '\n';

std::cout << "American: \n";
Solver b = Solver(100, 100,
                  OptionExpiryType::American, true,
                  FiniteDifferenceType::Implicit,
                  100, 110, 1, 0.2, 0, 0.3, 5);
std::vector<double> resA {b.Solve(false)};
for (auto e : resA) std::cout << e << " ";
std::cout << '\n';

std::cout << "Bermuda: \n";
std::vector<double> schedule{0.25,0.5,0.75,1.0};
Solver c = Solver(100, 100,
                  OptionExpiryType::Bermuda, true,
                  FiniteDifferenceType::Implicit,
                  100, 110, 1, 0.2, 0, 0.3, 5, schedule);
std::vector<double> resC {c.Solve(false)};
for (auto e : resC) std::cout << e << " ";  
std::cout << '\n';
