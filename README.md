# FiniteDifference

Usage:
<pre>
To initialize a solver object, provide following parameters. <br>

    int gridS {0};                         // grid size on stock price <br>
    int gridT {0};                         // grid size on time <br>
    OptionExpiryType oType;                // option type: defined in OptionExpiryType enum class<br>
    bool callFlag;                         // true indicates call, false put<br>
    FiniteDifferenceType dType;            // Finite difference method: defined in FiniteDifferenceType enum class<br>
    double spot {0.0};                     // spot stock price <br>
    double strike {0.0};                   // strike price<br>
    double maturity {0.0};                 // time to maturity for option, equals T-t <br>
    double dYield {0.0};                   // continuous dividend yield <br>
    double r {0.0};                        // risk free rate <br>
    double vol {0.0};                      // stock price vol <br>
    double maxVolMultiplier {0.0};         // maximum range for stock price in differencing process <br>
    std::vector<double> exerciseTimes {};  // exercise schedule for Bermuda type option, can be omitted for European and American options when initialization<br>


Call .Solve() on a solver object will return the option premium, delta and gamma in a std::vector.

Price European option: <br>

Solver a1 = Solver(100, 100, <br>
                  OptionExpiryType::European, true,<br>
                  FiniteDifferenceType::Implicit,<br>
                  100, 90, 1, 0, 0, 0.3, 5);<br>
std::vector<double> resA1 {a1.Solve(false)};<br>


Price American option: <br>

Solver b = Solver(100, 100,<br>
                  OptionExpiryType::American, true,<br>
                  FiniteDifferenceType::Implicit,<br>
                  100, 110, 1, 0.2, 0, 0.3, 5);<br>
std::vector<double> resA {b.Solve(false)};<br>


Price Bermuda option with exercise schedule input:<br>

std::vector<double> schedule{0.25,0.5,0.75,1.0};<br>
Solver c = Solver(100, 100,<br>
                  OptionExpiryType::Bermuda, true,<br>
                  FiniteDifferenceType::Implicit,<br>
                  100, 110, 1, 0.2, 0, 0.3, 5, schedule);<br>
std::vector<double> resC {c.Solve(false)};<br>
<pre>
