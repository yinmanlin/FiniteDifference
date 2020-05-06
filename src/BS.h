#pragma once

class BS{

double spot {0.0};
double strike {0.0};
double r {0.0};
double dYield {0.0};
double maturity {0.0};
double vol {0.0};
bool optionType;

public:
BS(double spot, double strike, double r, double dYield, double maturity, double vol, bool o);
double optionPrice();
double d1();
double d2();
double delta();
double gamma();

};