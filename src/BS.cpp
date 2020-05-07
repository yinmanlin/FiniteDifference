#include "BS.h"
#include "MathUtil.h"

BS::BS(double spot, double strike, double r, double dYield, double maturity, double vol, bool o) :
    spot(spot), strike(strike), r(r), dYield{dYield}, maturity(maturity), vol(vol), optionType(o) {}

double BS::optionPrice(){
    if (optionType == true){
        return (spot*exp(-dYield*maturity)*normalCDF(d1()) - strike*normalCDF(d2())*exp(-r*maturity));
    }else{
        return  strike*normalCDF(-d2())*exp(-r*maturity) - spot*normalCDF(-d1());
    }
}

double BS::d1(){
    return (log(spot/strike)+(r-dYield+0.5*vol*vol)*maturity)/(vol*sqrt(maturity));
}

double BS::d2(){
    return d1() - vol * sqrt(maturity);
}

double BS::delta(){
    if (optionType == true){
        return exp(-dYield*maturity)*normalCDF(d1());
    }else{
        return exp(-dYield*maturity)*(1-normalCDF(d1()));
    }
}

double BS::gamma(){
    return exp(-dYield*maturity)/spot/vol/sqrt(maturity)/sqrt(2*3.1415926)*exp(-d1()*d1()/2);
}