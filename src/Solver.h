#pragma once

#include "EnumClass.h"
#include <Eigen/Dense>
#include <vector>

/* Uses a square grid
 */

class Solver{
    int gridS {0};
    int gridT {0};
    OptionExpiryType oType;
    bool callFlag;
    FiniteDifferenceType dType;
    double spot {0.0};
    double strike {0.0};
    double maturity {0.0};
    double dYield {0.0};
    double r {0.0};
    double vol {0.0};
    double maxVolMultiplier {0.0};
    // matrix x-axis is grid over t, y-axis is grid over S
    Eigen::MatrixXd leftCoefSparseMat;
    Eigen::MatrixXd rightCoefSparseMat;
public:
    Solver();
    Solver(int grid1,
           int grid2,
           OptionExpiryType oType,
           bool callFlag,
           FiniteDifferenceType dType,
           double spot,
           double strike,
           // maturity is time to maturity(tau), equals T-t
           double maturity,
           double dYield,
           double r,
           double vol,
           double maxMul);
    //setter
    void setMaxVolMultiplier(double m) { maxVolMultiplier = m;}
    void setDiffType(FiniteDifferenceType m) { dType = m;}
    void setOptionType(OptionExpiryType m) { oType = m;}

    // price related
    Eigen::VectorXd optionPayoff();
    void generateCoefSparseMats();
    std::vector<double> Solve(bool calcVega);
};