#include "Solver.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

Solver::Solver(int grid1,
               int grid2,
               OptionExpiryType oType,
               bool callFlag,
               FiniteDifferenceType dType,
               double spot,
               double strike,
               double maturity,
               double dYield,
               double r,
               double vol,
               double maxMul) : gridS(grid1), gridT(grid2), 
               oType(oType), callFlag(callFlag), dType(dType), spot(spot), strike(strike), 
               maturity(maturity), dYield(dYield), r(r), vol(vol), 
               maxVolMultiplier(maxMul) {}

Solver::Solver(int grid1,
               int grid2,
               OptionExpiryType oType,
               bool callFlag,
               FiniteDifferenceType dType,
               double spot,
               double strike,
               double maturity,
               double dYield,
               double r,
               double vol,
               double maxMul,
               const std::vector<double>& schedule) : gridS(grid1), gridT(grid2), 
               oType(oType), callFlag(callFlag), dType(dType), spot(spot), strike(strike), 
               maturity(maturity), dYield(dYield), r(r), vol(vol), 
               maxVolMultiplier(maxMul), exerciseTimes {schedule} {}

Eigen::VectorXd Solver::optionPayoff()
{
    double x_max = vol*sqrt(maturity)*maxVolMultiplier; 
    Eigen::VectorXd payoff = Eigen::VectorXd::LinSpaced(gridS+1, -x_max, x_max);
    
    if (callFlag == true)
    {
        std::for_each(payoff.data(), payoff.data()+payoff.size(), 
                [this](double& d){ 
                    if (exp(d)*spot - strike > 0) d = exp(d)*spot - strike; 
                    else d= 0.0;
                    });
    }
    // put option
    else if(callFlag == false)
    {
        std::for_each(payoff.data(), payoff.data()+payoff.size(), 
                [this](double& d){
                    if (strike - exp(d)*spot > 0) d = strike - exp(d)*spot;
                    else d = 0.0;
                    });
    }
    
    return payoff;
}

void Solver::generateCoefSparseMats()
{
    double mu = (r-dYield-0.5*vol*vol);
    double x_max = vol*sqrt(maturity)*5;
    double dx = 2*x_max/gridS;
    double dt = maturity/gridT;
    double h = dt/dx;
    double k = h/dx;

    // Explicit method is DVj+1 = Vj
    // left matrix is identity
    if (dType == FiniteDifferenceType::Implicit)
    { 
        double a = - 0.5*(k*vol*vol+h*mu); //coeff for Vn+1
        double b = 1+k*vol*vol+r*dt;       //coeff for Vn
        double c = -0.5*(k*vol*vol-h*mu);  //coeff for Vn-1

        rightCoefSparseMat = b*Eigen::MatrixXd::Identity(gridS+1, gridT+1);
        // set the diagonal above diagonal
        rightCoefSparseMat.diagonal(1).array() += a;
        rightCoefSparseMat.diagonal(-1).array() += c;
        leftCoefSparseMat = Eigen::MatrixXd::Identity(gridS+1,gridT+1);
    }
    // Explicit method is Vj+1 = CVj
    // right matrix is identity
    else if (dType == FiniteDifferenceType::Explicit)
    {
        double a = 0.5*(k*vol*vol+h*mu);   //coeff for Vn+1
        double b = 1-k*vol*vol-r*dt;       //coeff for Vn
        double c = 0.5*(k*vol*vol-h*mu);   //coeff for Vn-1

        leftCoefSparseMat = b*Eigen::MatrixXd::Identity(gridS+1, gridT+1);
        leftCoefSparseMat.diagonal(1).array() += a;
        leftCoefSparseMat.diagonal(-1).array() += c;
        rightCoefSparseMat = Eigen::MatrixXd::Identity(gridS+1,gridT+1);
    }
}

std::vector<double> Solver::Solve(bool calcVega)
{
    generateCoefSparseMats();
    Eigen::VectorXd payoff = optionPayoff();
    double dt = maturity/gridT;
    //assert (static_cast<int>(payoff.rows()) == 1);
    //assert (payoff.cols() == gridS + 1);
    double x_max = vol*sqrt(maturity)*5;
    double dx = 2*x_max/gridS;
    std::vector<int> et = generateBermudaET();
    Eigen::VectorXd exerciseCheck {payoff};
    if (dType == FiniteDifferenceType::Implicit)
    {
        Eigen::MatrixXd dInv = rightCoefSparseMat.inverse();
        for (int i = 0; i <= gridT; i++)
        {
            payoff = dInv*payoff;
            if (oType == OptionExpiryType::European)
            {
                // reinforce fixed boundary conditions
                payoff[0] = 0;
                payoff[gridS] = spot*exp(x_max) - strike*exp(-r*i*dt);
            }
            else if(oType == OptionExpiryType::American)
            {
                // reinforce early exercise boundary conditions
                payoff = payoff.cwiseMax(exerciseCheck);
            }
            else if(oType == OptionExpiryType::Bermuda){
                auto it = std::find(et.begin(),et.end(),i);
                if (it != et.end()){
                    payoff = payoff.cwiseMax(exerciseCheck);
                }
            }
        }
    }
    else if (dType == FiniteDifferenceType::Explicit)
    {
        for (int i = 0; i <= gridT; i++)
        {
            payoff = leftCoefSparseMat*payoff;
            if (oType == OptionExpiryType::European)
            {
                // reinforce fixed boundary conditions
                payoff[0] = 0;
                payoff[gridS] = spot*exp(x_max) - strike*exp(-r*i*dt);
            }
            else if(oType == OptionExpiryType::American)
            {
                // reinforce early exercise boundary conditions
                payoff = payoff.cwiseMax(exerciseCheck);
            }
            else if(oType == OptionExpiryType::Bermuda){
                auto it = std::find(et.begin(),et.end(),i);
                if (it != et.end()){
                    payoff = payoff.cwiseMax(exerciseCheck);
                }
            }
        }
    }

    // Calc greeks
    int n_mid = gridS/2;
    double sU = spot*exp(dx);
    double sD = spot*exp(-dx);
    double premium = payoff[n_mid];
    double delta = (payoff[n_mid+1]-payoff[n_mid-1])/(sU-sD);
    // ref: https://www.csie.ntu.edu.tw/~lyuu/finance1/2016/20160330.pdf
    double gamma = ((payoff[n_mid+1]-payoff[n_mid])/(sU-spot) - (payoff[n_mid]-payoff[n_mid-1])/(spot-sD))/((sU-sD)/2);
    std::vector<double> res;
    res.push_back(premium);
    res.push_back(delta);
    res.push_back(gamma);
    return res;
}

std::vector<int> Solver::generateBermudaET(){
    std::vector<int> et {};
    if (exerciseTimes.size() > 0){
        double dt = maturity/gridT;
        for (auto &e : exerciseTimes){
            et.push_back(std::ceil(e/dt));
        }
    }
    return et;
}